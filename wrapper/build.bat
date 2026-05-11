cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release

pip install ./lib/