#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp /opt/local/lib/libpng.a /opt/local/lib/libz.a -I../include/ -o ../libPixelData.dylib -std=c++17 -shared -I/opt/local/include -m64 -fPIC
elif [ $(uname) = "Linux" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lpng -lz -m64 -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -lpng -lz -m64 -fPIC
elif [ $(uname) = "DragonFly" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -lpng -lz -m64 -fPIC
else
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp lodepng.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.dll -DPREFER_LIBPNG -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lpng -lz -m64
fi
