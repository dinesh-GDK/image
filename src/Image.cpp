#include "Image.h"
#include <assert.h>
#include <cstdio>
#include <jpeglib.h>
#include <string>
#include <vector>

typedef unsigned int uint;

/** 
 * @brief Default constructor.
 */
Image::Image(uint width, uint height, uint channels) {
  this->_width = width;
  this->_height = height;
  this->_channels = channels;
  this->_crate.resize(height);
  for (int i = 0; i < height; i++) {
    this->_crate[i].resize(width);
    for (int j = 0; j < width; j++) {
      this->_crate[i][j].resize(channels);
    }
  }
}

/**
 * @brief Creates a new image with the same dimensions but without data.
 */
Image Image::like() {
  Image new_image = Image(this->_width, this->_height, this->_channels);
  return new_image;
}

/**
 * @brief Assignment operator.
 */
Image &Image::operator=(const Image &rhs) {
  this->_width = rhs._width;
  this->_height = rhs._height;
  this->_channels = rhs._channels;
  this->_crate = rhs._crate;
  return *this;
}

/**
 * @brief Addition operator to add two images.
 */
Image Image::operator+(const Image &rhs) {
  assert(this->_width == rhs._width && this->_height == rhs._height &&
         this->_channels == rhs._channels);
  Image result;
  result._width = this->_width;
  result._height = this->_height;
  result._channels = this->_channels;
  result._crate.resize(this->_height);
  for (int i = 0; i < this->_height; i++) {
    result._crate[i].resize(this->_width);
    for (int j = 0; j < this->_width; j++) {
      result._crate[i][j].resize(this->_channels);
      for (int k = 0; k < this->_channels; k++) {
        result._crate[i][j][k] = this->_crate[i][j][k] + rhs._crate[i][j][k];
      }
    }
  }
  return result;
}

/**
 * @brief Addition operator to add a constant value to an image.
 */
Image Image::operator+(const int &rhs) {
  Image result;
  result._width = this->_width;
  result._height = this->_height;
  result._channels = this->_channels;
  result._crate.resize(this->_height);
  for (int i = 0; i < this->_height; i++) {
    result._crate[i].resize(this->_width);
    for (int j = 0; j < this->_width; j++) {
      result._crate[i][j].resize(this->_channels);
      for (int k = 0; k < this->_channels; k++) {
        result._crate[i][j][k] += this->_crate[i][j][k] + rhs;
      }
    }
  }
  return result;
}

/**
 * @brief Addition assignment operator.
 */
Image Image::operator+=(const int &rhs) { return *this = *this + rhs; }

/**
 * @brief Subtraction operator to subtract two images.
 */
Image Image::operator-(const int &rhs) { return *this = *this + (-rhs); }

/**
 * @brief Subtraction assignment operator.
 */
Image Image::operator-=(const int &rhs) { return *this = *this - rhs; }

/**
 * @brief Multiplication operator to multiply image with a constant.
 */
Image Image::operator*(const int &rhs) {
  Image result;
  result._width = this->_width;
  result._height = this->_height;
  result._channels = this->_channels;
  result._crate.resize(this->_height);
  for (int i = 0; i < this->_height; i++) {
    result._crate[i].resize(this->_width);
    for (int j = 0; j < this->_width; j++) {
      result._crate[i][j].resize(this->_channels);
      for (int k = 0; k < this->_channels; k++) {
        result._crate[i][j][k] += this->_crate[i][j][k] * rhs;
      }
    }
  }
  return result;
}

/**
 * @brief Finds the minimum value in the image data.
 */
int Image::min() {
  BYTE res = 255;
  for (int i = 0; i < this->_height; i++) {
    for (int j = 0; j < this->_width; j++) {
      for (int k = 0; k < this->_channels; k++) {
        res = std::min(res, this->_crate[i][j][k]);
      }
    }
  }
  return res;
}

/**
 * @brief Finds the maximum value in the image data.
 */
int Image::max() {
  BYTE res = 0;
  for (int i = 0; i < this->_height; i++) {
    for (int j = 0; j < this->_width; j++) {
      for (int k = 0; k < this->_channels; k++) {
        res = std::max(res, this->_crate[i][j][k]);
      }
    }
  }
  return res;
}

/**
 * @brief Returns the width of the image.
 */
uint Image::width() { return this->_width; }

/**
 * @brief Returns the height of the image.
 */
uint Image::height() { return this->_height; }

/**
 * @brief Returns the number of channels of the image.
 */
uint Image::channels() { return this->_channels; }

/**
 * @brief Gets the value at a specific location in the image matrix.
 * @param i Row index.
 * @param j Column index.
 * @param k Channel index.
 */
BYTE Image::get(uint i, uint j, uint k) { return this->_crate[i][j][k]; }

/**
 * @brief Sets a value at a specific location in the image matrix.
 * @param i Row index.
 * @param j Column index.
 * @param k Channel index.
 * @param val Value to be set.
 */
void Image::set(uint i, uint j, uint k, BYTE val) {
  this->_crate[i][j][k] = val;
}

/**
 * @brief Reads a JPG image from the specified file path.
 * @param filename Path to the JPG image.
 */
void Image::readJpg(const std::string &filename) {
  FILE *file = fopen(filename.c_str(), "rb");
  if (!file) {
    std::cerr << "[Error] Could not open file " << filename << std::endl;
    return;
  }
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  this->_width = cinfo.output_width;
  this->_height = cinfo.output_height;
  this->_channels = cinfo.output_components;
  std::vector<BYTE> data(this->_width * this->_height * this->_channels);
  JSAMPROW rowPointer[1];
  while (cinfo.output_scanline < cinfo.output_height) {
    rowPointer[0] =
        &data[(cinfo.output_scanline) * this->_width * this->_channels];
    jpeg_read_scanlines(&cinfo, rowPointer, 1);
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(file);
  this->_crate.resize(this->_height);
  for (int i = 0; i < this->_height; i++) {
    this->_crate[i].resize(this->_width);
    for (int j = 0; j < this->_width; j++) {
      this->_crate[i][j].resize(this->_channels);
      for (int k = 0; k < this->_channels; k++) {
        this->_crate[i][j][k] =
            data[(i * this->_width + j) * this->_channels + k];
      }
    }
  }
}

/**
 * @brief Writes the image data to a JPG file.
 * @param filename Path to save the JPG image.
 * @param quality Quality of the saved JPG image (default is 75).
 */
void Image::writeJpg(const std::string &filename, int quality) {
  FILE *file = fopen(filename.c_str(), "wb");
  if (!file) {
    std::cerr << "[Error] Could not open file " << filename << std::endl;
    return;
  }
  if (this->channels() == 1) {
    this->_channels = 3;
    for (int i = 0; i < this->_height; i++) {
      for (int j = 0; j < this->_width; j++) {
        this->_crate[i][j].resize(this->_channels);
        for (int k = 1; k < this->_channels; k++) {
          this->_crate[i][j][k] = this->_crate[i][j][0];
        }
      }
    }
  }
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, file);
  cinfo.image_width = this->_width;
  cinfo.image_height = this->_height;
  cinfo.input_components = this->_channels;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  std::vector<unsigned char> data(this->_width * this->_height *
                                  this->_channels);
  for (int i = 0; i < this->_height; i++) {
    for (int j = 0; j < this->_width; j++) {
      for (int k = 0; k < this->_channels; k++) {
        data[(i * this->_width + j) * this->_channels + k] =
            this->_crate[i][j][k];
      }
    }
  }
  JSAMPROW rowPointer[1];
  while (cinfo.next_scanline < cinfo.image_height) {
    rowPointer[0] = &data[cinfo.next_scanline * this->_width * this->_channels];
    jpeg_write_scanlines(&cinfo, rowPointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  fclose(file);
}

/**
 * @brief Converts a RGB image to grayscale.
 */
Image Image::rgb_2_gray() {
  Image grayscale = Image(this->width(), this->height(), 1);
  // Y = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
  for (int i = 0; i < this->height(); i++) {
    for (int j = 0; j < this->width(); j++) {
      grayscale.set(i, j, 0,
                    0.257 * this->get(i, j, 0) + 0.504 * this->get(i, j, 1) +
                        0.098 * this->get(i, j, 2) + 16);
    }
  }
  return grayscale;
}