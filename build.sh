#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.dylib -std=c++17 -shared -arch arm64 -arch x86_64 -fPIC
elif [ $(uname) = "Linux" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -static-libgcc -static-libstdc++ -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -fPIC
elif [ $(uname) = "DragonFly" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -fPIC
else
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.dll -std=c++17 -shared -static-libgcc -static-libstdc++ -static
fi
