@echo off
git clone https://github.com/Andrija44/PythonECFSR.git
cd PythonECFSR

conda env create -f environment.yml
call conda activate ecf

git clone https://github.com/djakobovic/ECF.git

cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release

pip install ./wrapper/lib