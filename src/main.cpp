#include "Image.h"
#include "dithering.h"
#include "error_diffusion.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace po = boost::program_options;

std::tuple<bool, std::string>
validate(const std::string &type, const std::string &value,
         const std::vector<std::string> &allowed) {
  std::string error = "Invalid argument for " + type + "\nAllowed values: | ";
  for (const std::string &allow : allowed) {
    error += allow + " | ";
    if (allow == value) {
      return {true, ""};
    }
  }
  return {false, error};
}

int main(int argc, char *argv[]) {

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "bw", boost::program_options::value<bool>(),
      "convert image to black and white")("mbvq",
                                          boost::program_options::value<bool>(),
                                          "MBVQ technique for ERROR_DIFFUSION")(
      "size", boost::program_options::value<unsigned int>(),
      "dimension of dithering matrix")(
      "kernel", boost::program_options::value<std::string>(),
      "FLOYD_STEINBERG / JARVIS_JUDICE_NINKE / STUCKI")(
      "threshold", boost::program_options::value<unsigned int>(),
      "threshold for ERROOR_DIFFUSION")(
      "option", boost::program_options::value<std::string>(),
      "DITHERING / ERROR_DIFFUSION")(
      "input", boost::program_options::value<std::string>(), "input path")(
      "output", boost::program_options::value<std::string>(), "output path");

  boost::program_options::variables_map vm;
  try {
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
  } catch (const boost::program_options::invalid_command_line_syntax &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  std::string input_file = vm["input"].as<std::string>();
  std::string output_file = vm["output"].as<std::string>();
  std::string option = vm["option"].as<std::string>();
  std::tuple<bool, std::string> is_valid =
      validate("option", option, {"DITHERING", "ERROR_DIFFUSION"});
  if (!std::get<0>(is_valid)) {
    std::cerr << std::get<1>(is_valid) << std::endl;
    return 1;
  }

  bool bw = false;
  if (vm.count("bw")) {
    bw = vm["bw"].as<bool>();
  }

  Image image;
  image.readJpg(input_file);

  if (bw) {
    image = image.rgb_2_gray();
  }

  if (option == "DITHERING") {
    unsigned int size = vm["size"].as<unsigned int>();
    if (size > 0 && (size & (size - 1)) == 0) {
      image = dithering(image, size);
    } else {
      std::cerr << "Invalid value for argument size\nShpule be in powers of 2"
                << std::endl;
      return 1;
    }
  } else {
    std::string type = vm["kernel"].as<std::string>();
    is_valid = validate("kernel", type,
                        {"FLOYD_STEINBERG", "JARVIS_JUDICE_NINKE", "STUCKI"});
    if (!std::get<0>(is_valid)) {
      std::cerr << std::get<1>(is_valid) << std::endl;
      return 1;
    }

    unsigned int threshold = vm["threshold"].as<unsigned int>();
    bool is_mbvq = vm.count("mbvq") && vm["mbvq"].as<bool>() ? true : false;
    image = error_diffusion(image, DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE,
                            is_mbvq, threshold);
  }

  image.writeJpg(output_file);

  return 0;
}