#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <vector>

/**
 * @typedef BYTE
 * @brief Defines a type representing a byte.
 */
typedef unsigned char BYTE;

/**
 * @typedef uint
 * @brief Defines a type representing an unsigned integer.
 */
typedef unsigned int uint;

/**
 * @typedef CRATE
 * @brief Represents a 3D matrix containing BYTE elements.
 */
typedef std::vector<std::vector<std::vector<BYTE>>> CRATE;

/**
 * @class Image
 * @brief Represents an image object with functionalities for basic manipulations and file IO.
 */
class Image {
private:
  uint _width, _height, _channels; /**< Image dimensions and number of channels. */
  CRATE _crate;                    /**< 3D container to store image data. */
  
  /** 
   * @brief Normalizes the image data.
   */
  void _normalize();

  /** 
   * @brief Denormalizes the image data.
   */
  void _denormalize();

public:
  /** 
   * @brief Default constructor.
   */
  Image() {}

  /**
   * @brief Parameterized constructor to initialize image with given dimensions.
   * @param width Image width.
   * @param height Image height.
   * @param channels Number of color channels.
   */
  Image(uint width, uint height, uint channels);

  /**
   * @brief Assignment operator.
   */
  Image &operator=(const Image &rhs);

  /**
   * @brief Addition operator to add two images.
   */
  Image operator+(const Image &rhs);

  /**
   * @brief Subtraction operator to subtract two images.
   */
  Image operator-(const Image &rhs);

  /**
   * @brief Addition operator to add a constant value to an image.
   */
  Image operator+(const int &rhs);

  /**
   * @brief Addition assignment operator.
   */
  Image operator+=(const int &rhs);

  /**
   * @brief Subtraction operator to subtract a constant value from an image.
   */
  Image operator-(const int &rhs);

  /**
   * @brief Subtraction assignment operator.
   */
  Image operator-=(const int &rhs);

  /**
   * @brief Multiplication operator to multiply image with a constant.
   */
  Image operator*(const int &rhs);

  /**
   * @brief Returns the width of the image.
   */
  uint width();

  /**
   * @brief Returns the height of the image.
   */
  uint height();

  /**
   * @brief Returns the number of channels of the image.
   */
  uint channels();

  /**
   * @brief Gets the value at a specific location in the image matrix.
   * @param i Row index.
   * @param j Column index.
   * @param k Channel index.
   */
  BYTE get(uint i, uint j, uint k);

  /**
   * @brief Sets a value at a specific location in the image matrix.
   * @param i Row index.
   * @param j Column index.
   * @param k Channel index.
   * @param val Value to be set.
   */
  void set(uint i, uint j, uint k, BYTE val);

  /**
   * @brief Finds the maximum value in the image data.
   */
  int max();

  /**
   * @brief Finds the minimum value in the image data.
   */
  int min();

  /**
   * @brief Reads a JPG image from the specified file path.
   * @param filename Path to the JPG image.
   */
  void readJpg(const std::string &filename);

  /**
   * @brief Writes the image data to a JPG file.
   * @param filename Path to save the JPG image.
   * @param quality Quality of the saved JPG image (default is 75).
   */
  void writeJpg(const std::string &filename, int quality = 75);

  /**
   * @brief Creates a new image with the same dimensions but without data.
   */
  Image like();

  /**
   * @brief Converts a RGB image to grayscale.
   */
  Image rgb_2_gray();
};

#endif
