// #include "Image.h"
#include <iostream>
#include <vector>
// #include "dithering.h"
#include "error_diffusion.h"

int main() {

    Image img;
    img.readJpg("./sample/tiger.jpg");
    // img.writeJpg("./sample/paris1.jpg");

    Image gray = img.rgb_2_gray();
    // gray.writeJpg("./sample/paris_gray.jpg");

    // Image dithered = dithering(gray, 16);
    // dithered.writeJpg("./sample/paris_d.jpg");

    // Image img2 = error_diffusion(img, DIFFUSION_KERNEL::FLOYD_STEINBERG, 127);
    Image img2 = error_diffusion(img, DIFFUSION_KERNEL::JARVIS_JUDICE_NINKE, 127);
    // Image img2 = error_diffusion(gray, DIFFUSION_KERNEL::STUCKI, 127);

    img2.writeJpg("./sample/tiger_error.jpg");


    return 0;
}