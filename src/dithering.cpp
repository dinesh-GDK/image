#include "Image.h"
#include <assert.h>
#include <vector>

// Define byte and matrix crate type aliases
typedef unsigned char BYTE;
typedef std::vector<std::vector<BYTE>> mCRATE;

/**
 * Generates a dithering matrix of specified dimensions.
 * @param dim Dimension of the dithering matrix (should be power of 2).
 * @return A 2D dithering matrix.
 */
mCRATE dithering_matrix(unsigned int dim) {
  // Ensure dimension is valid
  assert(dim >= 2);

  // Initialize a matrix with the given dimensions
  mCRATE res(dim, std::vector<BYTE>(dim));

  // Base case: 2x2 matrix
  if (dim == 2) {
    res = {{1, 2}, {3, 0}};
  } else {
    // Recursive case: split the matrix
    int ndim = dim / 2;
    mCRATE part = dithering_matrix(ndim);

    // Populate the resulting matrix by combining sub-matrices
    for (int i = 0; i < ndim; i++) {
      for (int j = 0; j < ndim; j++) {
        res[i][j] = 4 * part[i][j] + 1;
        res[i][j + ndim] = 4 * part[i][j] + 2;
        res[i + ndim][j] = 4 * part[i][j] + 3;
        res[i + ndim][j + ndim] = 4 * part[i][j];
      }
    }
  }
  return res;
}

/**
 * Converts a dithering matrix to a threshold matrix.
 * @param dithering_matrix The input dithering matrix.
 * @return A 2D threshold matrix.
 */
mCRATE threshold_matrix(mCRATE dithering_matrix) {
  // Ensure matrix is square
  assert(dithering_matrix.size() == dithering_matrix[0].size());

  const int dim = dithering_matrix.size();
  mCRATE res(dim, std::vector<BYTE>(dim));

  // Calculate threshold values based on dithering matrix values
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      res[i][j] = ((dithering_matrix[i][j] + 0.5) / (dim * dim)) * 255;
    }
  }
  return res;
}

/**
 * Apply dithering to an image using the specified matrix dimension.
 * @param image The input image to dither.
 * @param dim Dimension of the dithering matrix.
 * @return Dithered image.
 */
Image dithering(Image image, unsigned int dim) {
  // Generate dithering and threshold matrices
  mCRATE dithering = dithering_matrix(dim);
  mCRATE threshold = threshold_matrix(dithering);

  // Loop through each pixel of the image
  for (int i = 0; i < image.height(); i++) {
    for (int j = 0; j < image.width(); j++) {
      for (int k = 0; k < image.channels(); k++) {
        // Determine threshold indices for the current pixel
        int tx = i % dim > 0 ? i % dim : dim - 1;
        int ty = j % dim > 0 ? j % dim : dim - 1;

        // Set the pixel value based on the threshold value
        if (image.get(i, j, k) <= threshold[tx][ty]) {
          image.set(i, j, k, 0);
        } else {
          image.set(i, j, k, 255);
        }
      }
    }
  }
  return image;
}
