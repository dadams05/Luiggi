# Luiggi

- to build use `cmake -S . -B build`
- use `-DSDL_SHARED=OFF -DSDL_STATIC=ON` to build a static library instead of a dynamic one
- 4 build types for SDL3 (https://wiki.libsdl.org/SDL3/README-cmake)
  - `Debug`
  - `Release`
  - `RelWithDebInfo`
  - `MinSizeRel`
- to build for release `cmake --build build --config Release` also have config `MinSizeRel`

## how to turn font file (.ttf or .otf)
- get ur font file
- go to linux
- run this command
`xxd -i my_font.ttf > my_font.cpp` or `my_font.h` whatever u need