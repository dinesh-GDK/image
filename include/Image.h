#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <vector>

typedef unsigned char BYTE;
typedef unsigned int uint;
typedef std::vector<std::vector<std::vector<BYTE>>> CRATE;

class Image {
private:
  uint _width, _height, _channels;
  CRATE _crate;
  void _normalize();
  void _denormalize();

public:
  Image() {}
  Image(uint width, uint height, uint channels) {
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
  uint width();
  uint height();
  uint channels();
  BYTE get(uint i, uint j, uint k);
  void set(uint i, uint j, uint k, BYTE val);
  int max();
  int min();
  void readJpg(const std::string &filename);
  void writeJpg(const std::string &filename, int quality = 75);
  Image like();
  Image rgb_2_gray();
};

#endif