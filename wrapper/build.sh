cmake -S .. -B build
cmake --build build --target ecf-sr
cmake --install build

pip install ./lib/