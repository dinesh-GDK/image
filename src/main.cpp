#include "Image.h"
#include "dithering.h"
#include "error_diffusion.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

typedef unsigned int unit;

namespace po = boost::program_options;

void cerr(const std::string &error, const std::string log_level = "ERROR") {
  std::cerr << "[" + log_level + "] " + error << std::endl;
}

void validate_argument(const std::string &type, const uint &value,
                       const std::vector<uint> &allowed) {
  std::string error = "Invalid argument for " + type + "; Allowed values: | ";
  for (const uint &allow : allowed) {
    error += std::to_string(allow) + " | ";
    if (allow == value) {
      return;
    }
  }
  throw std::invalid_argument(error);
}

bool parse_arguments(int argc, char **argv, po::variables_map &vm,
                     po::options_description &desc) {
  desc.add_options()("help", "help message")("input", po::value<std::string>(),
                                             "input image path (only jpg)")(
      "output", po::value<std::string>(), "output image path (only jpg)")(
      "op", po::value<uint>(),
      "operation to perform DITHERING=1 / ERROR_DIFFUSION=2")(
      "bw", po::value<bool>(), "convert image to black and white (default 0)")(
      "size", po::value<uint>(),
      "dimension of dithering matrix for DITHERING (default 8)")(
      "kernel", po::value<uint>(),
      "FLOYD_STEINBERG=1 / JARVIS_JUDICE_NINKE=2 / STUCKI=3 (default 1)")(
      "threshold", po::value<uint>(),
      "threshold for ERROR_DIFFUSION (default 127)")(
      "mbvq", po::value<bool>(),
      "use MBVQ technique for ERROR_DIFFUSION (default 0)");
  bool status = true;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (const std::exception &e) {
    cerr(e.what());
    status = false;
  }
  return status;
}

int main(int argc, char *argv[]) {
  // read CLI
  po::variables_map vm;
  po::options_description desc("Allowed options");
  if (!parse_arguments(argc, argv, vm, desc)) {
    return 1;
  }
  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    std::cout << "Sample usage" << std::endl;
    std::string usage;
    usage = "./image_print --input=<input-image-path> "
            "--output=<output-image-path> --op=ERROR_DIFFUSION "
            "--kernel=FLOYD_STEINBERG --threshold=127 --mbvq=1 --bw=1";
    std::cout << usage << std::endl;
    usage = "./image_print --input=<input-image-path> "
            "--output=<output-image-path> --op=DITHERING --size=16 --bw=1";
    std::cout << usage << std::endl;
    return 0;
  }
  try {
    // validate necessary arguments
    if (!vm.count("input") || !vm.count("output") || !vm.count("op")) {
      cerr("Arguments `input`, `output` and `op` are required");
      return 1;
    }
    std::string input_file = vm["input"].as<std::string>();
    std::string output_file = vm["output"].as<std::string>();
    uint op = vm["op"].as<uint>();
    validate_argument("op", op, {1, 2});
    // create and load image
    Image image;
    image.readJpg(input_file);
    // if bw convert image to black and white
    bool bw = vm.count("bw") && vm["bw"].as<bool>() == true;
    if (bw) {
      image = image.rgb_2_gray();
    }
    // process operations
    if (op == 1) {
      uint size = vm.count("size") ? vm["size"].as<uint>() : 8;
      // check if size is in power of 2
      if (!(size > 0 && (size & (size - 1)) == 0)) {
        cerr("Invalid value for argument `size`; Should be in powers of 2");
        return 1;
      }
      image = dithering(image, size);
    } else {
      uint kernel_idx = vm.count("kernel") ? vm["kernel"].as<uint>() : 1;
      uint threshold = vm.count("threshold") ? vm["threshold"].as<uint>() : 127;
      bool is_mbvq = vm.count("mbvq") && vm["mbvq"].as<bool>() ? true : false;
      validate_argument("kernel", kernel_idx, {1, 2, 3});
      if (threshold < 0 || threshold > 255) {
        throw std::invalid_argument(
            "Argument `threshold` should be within 0 and 255");
      }
      DIFFUSION_KERNEL kernel = static_cast<DIFFUSION_KERNEL>(kernel_idx);
      image = error_diffusion(image, kernel, is_mbvq, threshold);
    }
    image.writeJpg(output_file);
  } catch (const std::exception &e) {
    cerr(e.what());
    return 1;
  }
  return 0;
}