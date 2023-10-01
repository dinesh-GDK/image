#include <assert.h>
#include <map>
#include <vector>
#include "Image.h"

enum DIFFUSION_KERNEL {
    FLOYD_STEINBERG,
    JARVIS_JUDICE_NINKE,
    STUCKI
};

std::map<DIFFUSION_KERNEL, std::vector<std::vector<double>>> DIFFUSION_KERNELS = {
    {
        DIFFUSION_KERNEL::FLOYD_STEINBERG, {
            {0, 0, 0},
            {0, 0, 7./16},
            {3./16, 5./16, 1./16}
        }
    },
    {
        DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE, {
            {0./48, 0./48, 0./48, 0./48, 0./48},
            {0./48, 0./48, 0./48, 0./48, 0./48},
            {0./48, 0./48, 0./48, 7./48, 5./48},
            {3./48, 5./48, 7./48, 5./48, 3./48},
            {1./48, 3./48, 5./48, 3./48, 1./48},
        }
    },
    {
        DIFFUSION_KERNEL::STUCKI, {
            {0./42, 0./42, 0./42, 0./42, 0./42,},
            {0./42, 0./42, 0./42, 0./42, 0./42,},
            {0./42, 0./42, 0./42, 8./42, 4./42,},
            {2./42, 4./42, 8./42, 4./42, 2./42,},
            {1./42, 2./42, 4./42, 2./42, 1./42,}
        }
    }
};


std::vector<std::vector<double>> fliplr(std::vector<std::vector<double>> kernel) {
    assert(kernel.size() == kernel[0].size() && kernel.size() % 2 == 1);
    
    const int rows = kernel.size(), cols = kernel[0].size();
    for (size_t i = 0; i < cols / 2; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            std::swap(kernel[j][i], kernel[j][cols-1-i]);
        }
    }
    return kernel;
}

Image error_diffusion(Image image, DIFFUSION_KERNEL kernel_type, double threshold) {    
    std::vector<std::vector<double>> kernel = DIFFUSION_KERNELS[kernel_type];
    const int si = kernel.size() / 2;
    Image ret = image.like();
    std::vector<std::vector<std::vector<double>>> ip_crate(image.height(), std::vector<std::vector<double>>(image.width(), std::vector<double>(image.channels())));
    for (size_t i = 0; i < image.height(); ++i) {
        for (size_t j = 0; j < image.width(); ++j) {
            for (size_t ch = 0; ch < image.channels(); ++ch) {
                ip_crate[i][j][ch] = (double)image.get(i, j, ch);
            }
        }
    }
    for (size_t ch = 0; ch < image.channels(); ++ch) {
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
                ret.set(x, y, ch, ip_crate[x][y][ch] >= threshold ? 255 : 0);
                double error = ip_crate[x][y][ch] - ret.get(x, y, ch);
                for (int i = -1 * si; i <= si; ++i) {
                    for (int j = -1 * si; j <= si; ++j) {
                        int newX = x + i, newY = y + j;
                        bool inside = newX >= 0 && newY >= 0 && newX < image.height() && newY < image.width();
                        if (inside) {
                            ip_crate[newX][newY][ch] = ip_crate[newX][newY][ch] + error * curr_kernel[i+si][j+si]; 
                        }
                    }
                }
            }
        }
    }
    return ret;
}
