#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Linux" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lz -lpng -lz -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -lz -lpng -lz -fPIC
elif [ $(uname) = "DragonFly" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -lz -lpng -lz -fPIC
fi