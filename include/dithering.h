#ifndef DITHERING_H
#define DITHERING_H

#include "Image.h"
#include <vector>

typedef unsigned char BYTE;
typedef std::vector<std::vector<BYTE>> mCRATE;

mCRATE dithering_matrix(unsigned int dim);

mCRATE threshold_matrix(mCRATE dithering_matrix);

Image dithering(Image image, unsigned int dim);

#endif