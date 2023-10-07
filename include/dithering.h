#ifndef DITHERING_H
#define DITHERING_H

#include "Image.h"
#include <assert.h>
#include <vector>

typedef unsigned char BYTE;
typedef std::vector<std::vector<BYTE>> mCRATE;

mCRATE dithering_matrix(unsigned int dim) {
  assert(dim >= 2);
  mCRATE res(dim, std::vector<unsigned char>(dim));
  if (dim == 2) {
    res = {{1, 2}, {3, 0}};
  } else {
    int ndim = dim / 2;
    mCRATE part = dithering_matrix(ndim);
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

mCRATE threshold_matrix(mCRATE dithering_matrix) {
  assert(dithering_matrix.size() == dithering_matrix[0].size());
  const int dim = dithering_matrix.size();
  mCRATE res(dim, std::vector<unsigned char>(dim));

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      res[i][j] = ((dithering_matrix[i][j] + 0.5) / (dim * dim)) * 255;
    }
  }

  return res;
}

Image dithering(Image image, unsigned int dim) {
  mCRATE dithering = dithering_matrix(dim);
  mCRATE threshold = threshold_matrix(dithering);

  for (int i = 0; i < image.height(); i++) {
    for (int j = 0; j < image.width(); j++) {
      for (int k = 0; k < image.channels(); k++) {
        int tx = i % dim > 0 ? i % dim : dim - 1;
        int ty = j % dim > 0 ? j % dim : dim - 1;
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

#endif