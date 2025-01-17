# 🖨️ print_image

## 🎨 Transform Images into Beautiful Print-Like Artwork! 📄

Discover the world of image conversion with JPEG to Printable Image Converter. This C++ command-line tool harnesses the power of image processing algorithms to convert your images into stunning, paper-like artwork seamlessly. 🖼️🖨️

Whether you're seeking precision with **Dithering** and **Error Diffusion** or looking to master kernels like *FLOYD STEINBERG*, *JARVIS JUDICE NINKE*, *STUCKI*, this converter has you covered.

<p align="center">
  <img src="assests/demo_image.jpg" width="75%" height="auto"></img>
  <p align="center">Fig 1. (1) original image (2) Dithered image using 16x16 kernel (3) Error diffused image using STUCKI kernel and threshold as 127
  (4) Error diffused image using STUCKI kernel, threshold as 127 and using MBVQ (5) Dithered b/w image using 16x16 kernel (6) Error diffused image using STUCKI kernel and threshold as 127</p>
</p>

## Installation

You need to install the following dependancies

```bash
sudo apt-get install cmake libjpeg-dev libboost-all-dev
```

```bash
git clone https://github.com/dinesh-GDK/image_print.git
cd image_print
cmake . && make
```

## Usage
Only `JPEG` images are supported
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

## Image credits
https://unsplash.com/photos/OlKkCmToXEs?utm_content=creditShareLink&utm_medium=referral&utm_source=unsplash
