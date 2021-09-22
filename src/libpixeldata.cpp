/*

BSD 2-Clause License

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

#include <string>
#include <unordered_map> 

#include <cstdlib>

#include "rgbtorgba.h"
#include "pngformat.h"
#include "bmpformat.h"

#ifdef _WIN32
#define EXPORTED_FUNCTION extern "C" __declspec(dllexport)
#else /* macOS, Linux, and BSD */
#define EXPORTED_FUNCTION extern "C" __attribute__((visibility("default")))
#endif

static std::unordered_map<std::string, int> width;
static std::unordered_map<std::string, int> height;

EXPORTED_FUNCTION double image_load(const char *filename) {
  unsigned char *src = nullptr;
  bool alpha = false;
  if (!loadBMP(filename, &src, &width[filename], &height[filename])) {
    free(src);
    return true;
  }
  if (!loadPNG(filename, &src, &width[filename], &height[filename], &alpha)) {
    free(src);
    return true;
  }
  return false;
}

EXPORTED_FUNCTION double image_get_width(const char *filename) {
  if (width.find(filename) != width.end()) {
    int res = width[filename];
    width.erase(filename);
    return res;
  }
  return 0;
}

EXPORTED_FUNCTION double image_get_height(const char *filename) {
  if (height.find(filename) != height.end()) {
    int res = height[filename];
    height.erase(filename);
    return res;
  }
  return 0;
}

EXPORTED_FUNCTION double image_set_buffer(const char *filename, char *buffer) {
  unsigned char *src = nullptr;
  unsigned char *dst = (unsigned char *)buffer;
  bool alpha = false;
  if (dst) {
    if (!loadBMP(filename, &src, &width[filename], &height[filename])) {
      rgb_to_rgba(src, &dst, width[filename], height[filename], true);
      return true;
    }
    if (!loadPNG(filename, &src, &width[filename], &height[filename], &alpha)) {
      if (!alpha) {
        rgb_to_rgba(src, &dst, width[filename], height[filename], false);
      } else {
        copy_rgba(src, &dst, width[filename], height[filename]);
      }
      return true;
    }
  }
  return false;
}
