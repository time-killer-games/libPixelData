#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp /opt/local/lib/libz.a /opt/local/lib/libpng.a -I../include/ -o ../libPixelData.dylib -std=c++17 -shared -I/opt/local/include -lz -lpng -m64 -fPIC
elif [ $(uname) = "Linux" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lz -lpng -m64 -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  clang++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -lz -lpng -m64 -fPIC
elif [ $(uname) = "DragonFly" ]; then
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -lz -lpng -m64 -fPIC
else
  g++ widenarrow.cpp rgbtorgba.cpp pngformat.cpp bmpformat.cpp libpixeldata.cpp -I../include/ -o ../libPixelData.dll -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lz -lpng -m64
fi
