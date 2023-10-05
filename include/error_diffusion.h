#ifndef ERROR_DIFFUSION_H
#define ERROR_DIFFUSION_H

#include "Image.h"
#include <assert.h>
#include <map>
#include <vector>

#include "diffusion_kernel.h"

std::map<DIFFUSION_KERNEL, std::vector<std::vector<double>>> DIFFUSION_KERNELS =
    {{DIFFUSION_KERNEL::FLOYD_STEINBERG,
      {{0, 0, 0}, {0, 0, 7. / 16}, {3. / 16, 5. / 16, 1. / 16}}},
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

std::vector<BYTE>
get_color(std::vector<std::vector<std::vector<double>>> &ip_crate,
          unsigned int x, unsigned int y, double threshold) {
  std::vector<BYTE> color(ip_crate[0][0].size());
  for (size_t ch = 0; ch < ip_crate[0][0].size(); ++ch) {
    color[ch] = ip_crate[x][y][ch] >= threshold ? 255 : 0;
  }
  return color;
}

std::vector<BYTE>
get_mbvq_color(const std::vector<std::vector<std::vector<double>>> &ip_crate,
               unsigned int x, unsigned int y) {
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

std::vector<std::vector<double>>
fliplr(std::vector<std::vector<double>> kernel) {
  assert(kernel.size() == kernel[0].size() && kernel.size() % 2 == 1);

  const size_t rows = kernel.size(), cols = kernel[0].size();
  for (size_t i = 0; i < cols / 2; ++i) {
    for (size_t j = 0; j < rows; ++j) {
      std::swap(kernel[j][i], kernel[j][cols - 1 - i]);
    }
  }
  return kernel;
}

Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type,
                      bool isMBVQ = false, double threshold = 127.) {

  assert(isMBVQ && image.channels() == 3 || !isMBVQ);

  std::vector<std::vector<double>> kernel = DIFFUSION_KERNELS[kernel_type];
  const int si = kernel.size() / 2;
  Image ret = image.like();
  std::vector<std::vector<std::vector<double>>> ip_crate(
      image.height(),
      std::vector<std::vector<double>>(image.width(),
                                       std::vector<double>(image.channels())));
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
    std::vector<std::vector<double>> curr_kernel;
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
      std::vector<BYTE> color;
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

#endif