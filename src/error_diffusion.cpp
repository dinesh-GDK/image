
#include "Image.h"
#include "diffusion_kernel.h"
#include <map>
#include <vector>

COLOR get_nearest_vertex(const MBVQ mbvq, const float R, const float G,
                         const float B);

MBVQ get_mbvq(BYTE R, BYTE G, BYTE B);

std::vector<BYTE>
get_color(std::vector<std::vector<std::vector<double>>> &ip_crate,
          unsigned int x, unsigned int y, double threshold);

std::vector<BYTE>
get_mbvq_color(std::vector<std::vector<std::vector<double>>> &ip_crate,
               unsigned int x, unsigned int y);

std::vector<std::vector<double>>
fliplr(std::vector<std::vector<double>> kernel);

Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type, bool isMBVQ,
                      double threshold);
