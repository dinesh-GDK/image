#include "Image.h"
#include <assert.h>
#include <map>
#include <vector>

#include "diffusion_kernel.h"

typedef unsigned int uint;
typedef std::vector<std::vector<double>> VECTOR_DOUBLE_2D;
typedef std::vector<VECTOR_DOUBLE_2D> VECTOR_DOUBLE_3D;
typedef std::vector<BYTE> VECTOR_BYTE;

// Map containing the diffusion kernel matrices for various algorithms
std::map<DIFFUSION_KERNEL, VECTOR_DOUBLE_2D> DIFFUSION_KERNELS = {
    {DIFFUSION_KERNEL::FLOYD_STEINBERG,
     {{0. / 16, 0. / 16, 0. / 16},
      {0. / 16, 0. / 16, 7. / 16},
      {3. / 16, 5. / 16, 1. / 16}}},
    {DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE,
     {
         {0. / 48, 0. / 48, 0. / 48, 0. / 48, 0. / 48},
         {0. / 48, 0. / 48, 0. / 48, 0. / 48, 0. / 48},
         {0. / 48, 0. / 48, 0. / 48, 7. / 48, 5. / 48},
         {3. / 48, 5. / 48, 7. / 48, 5. / 48, 3. / 48},
         {1. / 48, 3. / 48, 5. / 48, 3. / 48, 1. / 48},
     }},
    {DIFFUSION_KERNEL::STUCKI,
     {{
          0. / 42,
          0. / 42,
          0. / 42,
          0. / 42,
          0. / 42,
      },
      {
          0. / 42,
          0. / 42,
          0. / 42,
          0. / 42,
          0. / 42,
      },
      {
          0. / 42,
          0. / 42,
          0. / 42,
          8. / 42,
          4. / 42,
      },
      {
          2. / 42,
          4. / 42,
          8. / 42,
          4. / 42,
          2. / 42,
      },
      {
          1. / 42,
          2. / 42,
          4. / 42,
          2. / 42,
          1. / 42,
      }}}};

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
                         const float B) {
  COLOR vertex;
  if (mbvq == MBVQ::CMYW) {
    vertex = COLOR::WHITE;
    if (B < 0.5) {
      if (B <= R) {
        if (B <= G) {
          vertex = COLOR::YELLOW;
        }
      }
    }
    if (G < 0.5) {
      if (G <= B) {
        if (G <= R) {
          vertex = COLOR::MAGENTA;
        }
      }
    }
    if (R < 0.5) {
      if (R <= B) {
        if (R <= G) {
          vertex = COLOR::CYAN;
        }
      }
    }
  }

  if (mbvq == MBVQ::MYGC) {
    vertex = COLOR::MAGENTA;
    if (G >= B) {
      if (R >= B) {
        if (R >= 0.5) {
          vertex = COLOR::YELLOW;
        } else {
          vertex = COLOR::GREEN;
        }
      }
    }
    if (G >= R) {
      if (B >= R) {
        if (B >= 0.5) {
          vertex = COLOR::CYAN;
        } else {
          vertex = COLOR::GREEN;
        }
      }
    }
  }

  if (mbvq == MBVQ::RGMY) {
    if (B > 0.5) {
      if (R > 0.5) {
        if (B >= G) {
          vertex = COLOR::MAGENTA;
        } else {
          vertex = COLOR::YELLOW;
        }
      } else {
        if (G > B + R) {
          vertex = COLOR::GREEN;
        } else {
          vertex = COLOR::MAGENTA;
        }
      }
    } else {
      if (R >= 0.5) {
        if (G >= 0.5) {
          vertex = COLOR::YELLOW;
        } else {
          vertex = COLOR::RED;
        }
      } else {
        if (R >= G) {
          vertex = COLOR::RED;
        } else {
          vertex = COLOR::GREEN;
        }
      }
    }
  }

  if (mbvq == MBVQ::KRGB) {
    vertex = COLOR::BLACK;
    if (B > 0.5) {
      if (B >= R) {
        if (B >= G) {
          vertex = COLOR::BLUE;
        }
      }
    }
    if (G > 0.5) {
      if (G >= B) {
        if (G >= R) {
          vertex = COLOR::GREEN;
        }
      }
    }
    if (R > 0.5) {
      if (R >= B) {
        if (R >= G) {
          vertex = COLOR::RED;
        }
      }
    }
  }

  if (mbvq == MBVQ::RGBM) {
    vertex = COLOR::GREEN;
    if (R > G) {
      if (R >= B) {
        if (B < 0.5) {
          vertex = COLOR::RED;
        } else {
          vertex = COLOR::MAGENTA;
        }
      }
    }
    if (B > G) {
      if (B >= R) {
        if (R < 0.5) {
          vertex = COLOR::BLUE;
        } else {
          vertex = COLOR::MAGENTA;
        }
      }
    }
  }

  if (mbvq == MBVQ::CMGB) {
    if (B > 0.5) {
      if (R > 0.5) {
        if (G >= R) {
          vertex = COLOR::CYAN;
        } else {
          vertex = COLOR::MAGENTA;
        }
      } else {
        if (G > 0.5) {
          vertex = COLOR::CYAN;
        } else {
          vertex = COLOR::BLUE;
        }
      }
    } else {
      if (R > 0.5) {
        if (R - G + B >= 0.5) {
          vertex = COLOR::MAGENTA;
        } else {
          vertex = COLOR::GREEN;
        }
      } else {
        if (G >= B) {
          vertex = COLOR::GREEN;
        } else {
          vertex = COLOR::BLUE;
        }
      }
    }
  }
  return vertex;
}

/**
 * @brief Determines the MBVQ type based on the given RGB values.
 * 
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 * @return MBVQ The MBVQ type corresponding to the RGB values.
 */
MBVQ get_mbvq(BYTE R, BYTE G, BYTE B) {
  MBVQ res;
  if (R + G > 255) {
    if (G + B > 255) {
      if (R + G + B > 510) {
        res = MBVQ::CMYW;
      } else {
        res = MBVQ::MYGC;
      }
    } else {
      res = MBVQ::RGMY;
    }
  } else {
    if (~(G + B > 255)) {
      if (~(R + G + B > 255)) {
        res = MBVQ::KRGB;
      } else {
        res = MBVQ::RGBM;
      }
    } else {
      res = MBVQ::CMGB;
    }
  }
  return res;
}

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
                      double threshold) {
  VECTOR_BYTE color(ip_crate[0][0].size());
  for (size_t ch = 0; ch < ip_crate[0][0].size(); ++ch) {
    color[ch] = ip_crate[x][y][ch] >= threshold ? 255 : 0;
  }
  return color;
}

/**
 * @brief Retrieves the color for a specific pixel in the 3D matrix based on MBVQ technique.
 * 
 * @param ip_crate 3D matrix containing color data.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @return VECTOR_BYTE Vector containing the RGB values for the pixel.
 */
VECTOR_BYTE get_mbvq_color(const VECTOR_DOUBLE_3D &ip_crate, uint x, uint y) {
  const MBVQ mbvq =
      get_mbvq(ip_crate[x][y][0], ip_crate[x][y][1], ip_crate[x][y][2]);
  const COLOR vertex =
      get_nearest_vertex(mbvq, ip_crate[x][y][0] / 255.,
                         ip_crate[x][y][1] / 255., ip_crate[x][y][2] / 255.);

  unsigned char R = 0, G = 0, B = 0;

  if (vertex == COLOR::RED || vertex == COLOR::MAGENTA ||
      vertex == COLOR::YELLOW || vertex == COLOR::WHITE) {
    R = 255;
  }

  if (vertex == COLOR::GREEN || vertex == COLOR::CYAN ||
      vertex == COLOR::YELLOW || vertex == COLOR::WHITE) {
    G = 255;
  }

  if (vertex == COLOR::BLUE || vertex == COLOR::CYAN ||
      vertex == COLOR::MAGENTA || vertex == COLOR::WHITE) {
    B = 255;
  }
  return {R, G, B};
}

/**
 * @brief Flips the 2D kernel matrix horizontally (left-to-right).
 * 
 * @param kernel 2D matrix representing the kernel.
 * @return VECTOR_DOUBLE_2D Flipped kernel.
 */
VECTOR_DOUBLE_2D fliplr(VECTOR_DOUBLE_2D kernel) {
  assert(kernel.size() == kernel[0].size() && kernel.size() % 2 == 1);
  const size_t rows = kernel.size(), cols = kernel[0].size();
  for (size_t i = 0; i < cols / 2; ++i) {
    for (size_t j = 0; j < rows; ++j) {
      std::swap(kernel[j][i], kernel[j][cols - 1 - i]);
    }
  }
  return kernel;
}

/**
 * @brief Performs error diffusion on the provided image.
 * 
 * @param image Image to be processed.
 * @param kernel_type Type of diffusion kernel to be used.
 * @param isMBVQ Flag to determine if MBVQ technique is used.
 * @param threshold Threshold for the error diffusion.
 * @return Image Processed image after error diffusion.
 */
Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type,
                      bool isMBVQ = false, double threshold = 127.) {
  assert(isMBVQ && image.channels() == 3 || !isMBVQ);
  VECTOR_DOUBLE_2D kernel = DIFFUSION_KERNELS[kernel_type];
  const int si = kernel.size() / 2;
  Image ret = image.like();
  VECTOR_DOUBLE_3D ip_crate(
      image.height(),
      VECTOR_DOUBLE_2D(image.width(), std::vector<double>(image.channels())));
  for (size_t i = 0; i < image.height(); ++i) {
    for (size_t j = 0; j < image.width(); ++j) {
      for (size_t ch = 0; ch < image.channels(); ++ch) {
        ip_crate[i][j][ch] = (double)image.get(i, j, ch);
      }
    }
  }
  for (size_t x = 0; x < image.height(); ++x) {
    size_t begin, end;
    int inc;
    VECTOR_DOUBLE_2D curr_kernel;
    // error-diffusion is like convolution but changes direction
    // eg. for the first row it moves from right to left (flipped kernel)
    // and for the next row it moves from left to right (non-flipped kernel)
    if (x % 2 == 0) {
      begin = image.width() - 1;
      end = 0;
      inc = -1;
      curr_kernel = fliplr(kernel);
    } else {
      begin = 0;
      end = image.width() - 1;
      inc = 1;
      curr_kernel = kernel;
    }
    for (size_t y = begin; y != end; y += inc) {
      VECTOR_BYTE color;
      if (isMBVQ) {
        color = get_mbvq_color(ip_crate, x, y);
      } else {
        color = get_color(ip_crate, x, y, threshold);
      }
      for (size_t ch = 0; ch < image.channels(); ++ch) {
        ret.set(x, y, ch, color[ch]);
        double error = ip_crate[x][y][ch] - ret.get(x, y, ch);
        for (int i = -1 * si; i <= si; ++i) {
          for (int j = -1 * si; j <= si; ++j) {
            int newX = x + i, newY = y + j;
            bool inside = newX >= 0 && newY >= 0 && newX < image.height() &&
                          newY < image.width();
            if (inside) {
              ip_crate[newX][newY][ch] = ip_crate[newX][newY][ch] +
                                         error * curr_kernel[i + si][j + si];
            }
          }
        }
      }
    }
  }
  return ret;
}
