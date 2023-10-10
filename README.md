# 🖨️ print_image

## 🎨 Transform Digital Images into Beautiful Print-Ready Artwork! 📄

Discover the world of image conversion with our JPEG to Printable Image Converter. This C++ command-line tool harnesses the power of advanced algorithms to seamlessly convert your digital images into stunning, paper-ready artwork. Dive into the realm of print perfection and bring your digital creations to life on paper! 🖼️🖨️

Whether you're seeking precision with **Dithering** and **Error Diffusion** or looking to master kernels like *FLOYD STEINBERG*, *JARVIS JUDICE NINKE*, *STUCKI*, our converter has you covered. These algorithms are used to convert images such that we can print them on paper media.

## Installation

You need to install the following dependancies

```bash
sudo apt-get install libjpeg-dev
sudo apt-get install libboost-all-dev
```

```bash
git clone https://github.com/dinesh-GDK/image_print.git
cd image_print
cmake . && make
```

## Usage
```bash
./image_print [options]
```

```bash
Allowed options:
  --help                help message
  --input arg           input image path (only jpg)
  --output arg          output image path (only jpg)
  --op arg              operation to perform DITHERING=1 / ERROR_DIFFUSION=2
  --bw arg              convert image to black and white (default 0)
  --size arg            dimension of dithering matrix for DITHERING (default 8)
  --kernel arg          FLOYD_STEINBERG=1 / JARVIS_JUDICE_NINKE=2 / STUCKI=3 
                        (default 2)
  --threshold arg       threshold for ERROR_DIFFUSION (default 127)
  --mbvq arg            use MBVQ technique for ERROR_DIFFUSION (default 0)

Sample usage
./image_print --input=<input-image-path> --output=<output-image-path> --op=ERROR_DIFFUSION --kernel=FLOYD_STEINBERG --threshold=127 --mbvq=1 --bw=1
./image_print --input=<input-image-path> --output=<output-image-path> --op=DITHERING --size=16 --bw=1
```

## License
This project is licensed under the MIT License - see the [LICENSE](https://github.com/dinesh-GDK/image_print/blob/main/LICENSE) file for details.

## Reference
- [Dithering](https://en.wikipedia.org/wiki/Dither#:~:text=Dithering%20is%20used%20in%20computer,from%20within%20the%20available%20palette)
- [Error Diffusion](https://en.wikipedia.org/wiki/Error_diffusion)
- [MBVQ technique](https://www.hpl.hp.com/techreports/96/HPL-96-128R1.pdf)