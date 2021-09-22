/*

BSD 2-Clause License

Copyright © 2018-2019 Robert B. Colton
Copyright © 2021, Samuel Venable
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#if defined(_WIN32)
#include "widenarrow.h"
#endif

#if defined(PREFER_LIBPNG)
#include <png.h>
#else
#include "lodepng.h"
#endif

int loadPNG(const char *filename, unsigned char **pixels, int *width, int *height, bool *alpha) {
  #if defined(PREFER_LIBPNG)
  (*width) = 0; (*height) = 0;
  #if !defined(_WIN32)
  FILE *fp = fopen(filename, "rb");
  #else
  std::wstring wfilename = widen(filename);
  FILE *fp = _wfopen(wfilename.c_str(), L"rb");
  #endif
  if (fp == nullptr) return -1;

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) return -2;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, nullptr, nullptr);
    return -3;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  png_byte color_type, bit_depth;
  (*width)       = png_get_image_width(png, info);
  (*height)       = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);
    
  //if (bgra)
  //  png_set_bgr(png);

  png_read_update_info(png, info);

  png_bytep image;
  size_t pitch = sizeof(png_byte) * 4 * (*width); // number of bytes in a row
  image = new png_byte[pitch * (*height)];

  for (size_t y = 0; y < (*height); y++) {
    png_read_row(png, (png_bytep)&image[pitch * y], nullptr);
  }

  if (color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_GA)
    *alpha = true;
  else {
    png_bytep trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16p trans_color = nullptr;

    png_get_tRNS(png, info, &trans_alpha, &num_trans, &trans_color);
    if (trans_alpha != nullptr)
      *alpha = true;
    else
      *alpha = false;
  }

  png_destroy_read_struct(&png, &info, nullptr);
  fclose(fp);

  (*pixels) = image;

  return 0;
  #else
  *alpha = true;
  return lodepng_decode32_file(pixels, (unsigned *)width, (unsigned *)height, filename);
  #endif
}

int writePNG(const char *filename, unsigned char *data, int width, int height) {
  #if defined(PREFER_LIBPNG)
  #if !defined(_WIN32)
  FILE *fp = fopen(filename, "wb");
  #else
  std::wstring wfilename = widen(filename);
  FILE *fp = _wfopen(wfilename.c_str(), L"wb");
  #endif

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, nullptr);
    return -2;
  }

  png_init_io(png, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png, info, (unsigned)width, (unsigned)height,
               8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
               
  //if (bgra)
  //  png_set_bgr(png);

  png_write_info(png, info);

  for (int i = 0; i < height; i++) {
    png_write_row(png, (png_bytep)&data[sizeof(png_byte) * 4 * width * i]);
  }
  png_write_end(png, nullptr);

  fclose(fp);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, &info);

  return 0;
  #else
  return lodepng_encode32_file(filename, data, width, height);
  #endif
}
