#include "Image.h"
#include "dithering.h"
#include "error_diffusion.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

  Image img;
  img.readJpg("./sample/tiger.jpg");

  Image gray = img.rgb_2_gray();

  Image dithered = dithering(gray, 16);
  dithered.writeJpg("./sample/tiger_d.jpg");

  // Image img2 = error_diffusion(img, DIFFUSION_KERNEL::FLOYD_STEINBERG, 127);
  // Image img2 = error_diffusion(img, DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE,
  // 127); Image img2 = error_diffusion(gray, DIFFUSION_KERNEL::STUCKI, 127);

  Image img2 =
      error_diffusion(img, DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE, true);
  img2.writeJpg("./sample/tiger_error.jpg");

  return 0;
}