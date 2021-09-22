#include <vector>

#include <cstring>

#include "rgbtorgba.h"

void rgb_to_rgba(unsigned char *src, unsigned char **dst, int w, int h, bool flip) {
  int offset = 0;
  std::vector<unsigned char> pixels(w * h * 4);
  std::vector<unsigned char> data  (w * h * 4);
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      pixels[offset + 0] = src[offset + 0];
      pixels[offset + 1] = src[offset + 1];
      pixels[offset + 2] = src[offset + 2];
	  pixels[offset + 3] = 255;
      offset += 4;
    }
  }
  for (unsigned int i = 0; i < h; i++) {
    memcpy(&data[i * w * 4 * sizeof(unsigned char)],
    &pixels[(flip ? (h - i - 1) : i) * w * 4 * sizeof(unsigned char)],
    w * 4 * sizeof(unsigned char));  
  }
  offset = 0;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      (*dst)[offset + 0] = data[offset + 0];
      (*dst)[offset + 1] = data[offset + 1];
      (*dst)[offset + 2] = data[offset + 2];
      (*dst)[offset + 3] = data[offset + 3];
      offset += 4;
    }
  }
}

void copy_rgba(unsigned char *src, unsigned char **dst, int w, int h) {
  int offset = 0;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      (*dst)[offset + 0] = src[offset + 0];
      (*dst)[offset + 1] = src[offset + 1];
      (*dst)[offset + 2] = src[offset + 2];
      (*dst)[offset + 3] = src[offset + 3];
      offset += 4;
    }
  }
}
