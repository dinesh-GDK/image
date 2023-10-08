#ifndef ERROR_DIFFUSION_H
#define ERROR_DIFFUSION_H

#include "Image.h"
#include "diffusion_kernel.h"
#include <map>
#include <vector>

/**
 * @typedef uint
 * @brief Alias for unsigned integer type.
 */
typedef unsigned int uint;

/**
 * @typedef VECTOR_DOUBLE_2D
 * @brief Represents a 2D matrix with double elements.
 */
typedef std::vector<std::vector<double>> VECTOR_DOUBLE_2D;

/**
 * @typedef VECTOR_DOUBLE_3D
 * @brief Represents a 3D matrix with double elements.
 */
typedef std::vector<VECTOR_DOUBLE_2D> VECTOR_DOUBLE_3D;

/**
 * @typedef VECTOR_BYTE
 * @brief Represents a vector of BYTE elements.
 */
typedef std::vector<BYTE> VECTOR_BYTE;

/**
 * @brief Finds the nearest vertex for given RGB values based on a specific MBVQ type.
 * 
 * @param mbvq The MBVQ type for vertex quantization.
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 * @return COLOR The nearest vertex color.
 */
COLOR get_nearest_vertex(const MBVQ mbvq, const float R, const float G,
                         const float B);

/**
 * @brief Determines the MBVQ type based on the given RGB values.
 * 
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 * @return MBVQ The MBVQ type corresponding to the RGB values.
 */
MBVQ get_mbvq(BYTE R, BYTE G, BYTE B);

/**
 * @brief Retrieves the color for a specific pixel in the 3D matrix based on a threshold.
 * 
 * @param ip_crate 3D matrix containing color data.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param threshold Threshold for deciding pixel color.
 * @return VECTOR_BYTE Vector containing the RGB values for the pixel.
 */
VECTOR_BYTE get_color(VECTOR_DOUBLE_3D &ip_crate, uint x, uint y,
                      double threshold);

/**
 * @brief Retrieves the color for a specific pixel in the 3D matrix based on MBVQ technique.
 * 
 * @param ip_crate 3D matrix containing color data.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @return VECTOR_BYTE Vector containing the RGB values for the pixel.
 */
VECTOR_BYTE get_mbvq_color(VECTOR_DOUBLE_3D &ip_crate, uint x, uint y);

/**
 * @brief Flips the 2D kernel matrix horizontally (left-to-right).
 * 
 * @param kernel 2D matrix representing the kernel.
 * @return VECTOR_DOUBLE_2D Flipped kernel.
 */
VECTOR_DOUBLE_2D fliplr(VECTOR_DOUBLE_2D kernel);

/**
 * @brief Performs error diffusion on the provided image.
 * 
 * @param image Image to be processed.
 * @param kernel_type Type of diffusion kernel to be used.
 * @param isMBVQ Flag to determine if MBVQ technique is used.
 * @param threshold Threshold for the error diffusion.
 * @return Image Processed image after error diffusion.
 */
Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type, bool isMBVQ,
                      double threshold);

#endif
