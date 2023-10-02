# Compiled on Debian 11 with gcc and g++

CP=g++
IPDIR=./src
OPDIR=./

All : Image

Image: $(IPDIR)/main.cpp
	cd src && \
	$(CP) -o ../main.o *.cpp *.h -ljpeg -std=gnu++17
	@echo "[USAGE] ./main <input_image> <warp_image> <unwarp_image> <width> <height>"
	./main.o
	@echo "----------------------------------------------------------------------------------------"

clean:
	rm -f ./main.o
