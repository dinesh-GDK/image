#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <vector>

typedef unsigned char BYTE;
typedef std::vector<std::vector<std::vector<BYTE>>> CRATE;

class Image {
private:
  unsigned int _width, _height, _channels;
  CRATE _crate;

  void _normalize();
  void _denormalize();

public:
  Image() {}
  Image(unsigned int width, unsigned int height, unsigned int channels) {
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
  Image &operator=(const Image &rhs);
  Image operator+(const Image &rhs);
  Image operator-(const Image &rhs);
  Image operator+(const int &rhs);
  Image operator+=(const int &rhs);
  Image operator-(const int &rhs);
  Image operator-=(const int &rhs);
  Image operator*(const int &rhs);
  unsigned int width();
  unsigned int height();
  unsigned int channels();
  BYTE get(unsigned int i, unsigned int j, unsigned int k);
  void set(unsigned int i, unsigned int j, unsigned int k, BYTE val);
  int max();
  int min();
  void readJpg(const std::string &filename);
  void writeJpg(const std::string &filename, int quality = 75);
  Image like();
  Image rgb_2_gray();
};

#endif