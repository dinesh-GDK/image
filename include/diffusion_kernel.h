#ifndef DIFFUSION_KERNEL_H
#define DIFFUSION_KERNEL_H

/**
 * @enum MBVQ
 * @brief Defines various modified barycentric vertex quantization (MBVQ) types.
 *
 * These are commonly used in the error diffusion algorithms when quantizing
 * colors in an image to determine the vertex set based on the original pixel
 * color and its quantized version.
 */
enum MBVQ {
  CMYW,      ///< Cyan, Magenta, Yellow, White vertex.
  MYGC,      ///< Magenta, Yellow, Green, Cyan vertex.
  RGMY,      ///< Red, Green, Magenta, Yellow vertex.
  KRGB,      ///< Black, Red, Green, Blue vertex.
  RGBM,      ///< Red, Green, Blue, Magenta vertex.
  CMGB       ///< Cyan, Magenta, Green, Blue vertex.
};

/**
 * @enum COLOR
 * @brief Defines various primary and secondary color names.
 *
 * These colors can be used throughout the program to refer to specific color
 * components or channels.
 */
enum COLOR {
  RED,       ///< Represents the Red color component.
  GREEN,     ///< Represents the Green color component.
  BLUE,      ///< Represents the Blue color component.
  WHITE,     ///< Represents the White color.
  MAGENTA,   ///< Represents the Magenta color.
  CYAN,      ///< Represents the Cyan color.
  YELLOW,    ///< Represents the Yellow color.
  BLACK      ///< Represents the Black color.
};

/**
 * @enum DIFFUSION_KERNEL
 * @brief Defines various error diffusion kernel types.
 *
 * These kernels dictate the error distribution pattern in error diffusion
 * algorithms like Floyd-Steinberg dithering.
 */
enum DIFFUSION_KERNEL {
  FLOYD_STEINBERG = 1,           ///< Represents the Floyd-Steinberg diffusion kernel.
  JARVIS_JUDICE_NINKE = 2,       ///< Represents the Jarvis-Judice-Ninke diffusion kernel.
  STUCKI = 3                     ///< Represents the Stucki diffusion kernel.
};

#endif