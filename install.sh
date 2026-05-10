git clone https://github.com/Andrija44/PythonECFSR.git
cd PythonECFSR

conda env create -f environment.yml
conda activate ecf

git clone https://github.com/djakobovic/ECF.git

cmake -S . -B build
cmake --build build
cmake --install build

pip install ./wrapper/lib