#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Linux" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lpng -lz -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -lpng -lz -fPIC
elif [ $(uname) = "DragonFly" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -lpng -lz -fPIC
fi