#ifndef ERROR_DIFFUSION_H
#define ERROR_DIFFUSION_H

#include "Image.h"
#include "diffusion_kernel.h"
#include <map>
#include <vector>

typedef unsigned int uint;
typedef std::vector<std::vector<double>> VECTOR_DOUBLE_2D;
typedef std::vector<VECTOR_DOUBLE_2D> VECTOR_DOUBLE_3D;
typedef std::vector<BYTE> VECTOR_BYTE;

COLOR get_nearest_vertex(const MBVQ mbvq, const float R, const float G,
                         const float B);

MBVQ get_mbvq(BYTE R, BYTE G, BYTE B);

VECTOR_BYTE get_color(VECTOR_DOUBLE_3D &ip_crate, uint x, uint y,
                      double threshold);

VECTOR_BYTE get_mbvq_color(VECTOR_DOUBLE_3D &ip_crate, uint x, uint y);

VECTOR_DOUBLE_2D fliplr(VECTOR_DOUBLE_2D kernel);

Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type, bool isMBVQ,
                      double threshold);

#endif