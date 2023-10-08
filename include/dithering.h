#ifndef DITHERING_H
#define DITHERING_H

#include "Image.h"
#include <vector>

/**
 * @typedef BYTE
 * @brief Represents a single byte data type, often used to represent pixel intensities.
 */
typedef unsigned char BYTE;

/**
 * @typedef mCRATE
 * @brief Represents a two-dimensional matrix of BYTE elements, used for dithering.
 */
typedef std::vector<std::vector<BYTE>> mCRATE;

/**
 * @brief Generates a dithering matrix of specified dimensions.
 *
 * The dithering matrix is often used for ordered dithering techniques where 
 * the matrix defines the threshold values for dithering.
 *
 * @param dim The dimension (rows and columns) of the dithering matrix.
 * @return mCRATE The generated dithering matrix.
 */
mCRATE dithering_matrix(unsigned int dim);

/**
 * @brief Converts a dithering matrix to a threshold matrix.
 *
 * A threshold matrix helps in deciding the pixel value based on a particular 
 * threshold during the dithering process.
 *
 * @param dithering_matrix The source dithering matrix.
 * @return mCRATE The generated threshold matrix.
 */
mCRATE threshold_matrix(mCRATE dithering_matrix);

/**
 * @brief Performs dithering operation on an image.
 *
 * This function processes the input image using dithering with the specified 
 * dithering matrix dimension.
 *
 * @param image The input image to be dithered.
 * @param dim The dimension of the dithering matrix to be used.
 * @return Image The dithered output image.
 */
Image dithering(Image image, unsigned int dim);

#endif