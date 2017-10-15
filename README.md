## Solver: fast inverse problem solver for astronomical imaging
[![build status](https://gitlab.com/gnthibault/PDWT/badges/NewArch/build.svg)](https://gitlab.com/gnthibault/PDWT/commits/NewArch)

Solver is fast inverse problem solver for astronomical imaging

It features
  -an openmp implementation of the 2D Discrete Wavelet Transform (DWT).
  
## Installation

# Cloning this repository

The header only pybind11 module is included as a submodule of this repository. This requires some attention when cloning this project. There are two options:

*   The simplest option is:

    ```bash
    git clone --recursive https://github.com/gnthibault/Solver.git
    ```

    This will download the submodule up to the version that is used in this project. To update to the latest commit of the submodule itself:

    ```bash
    git submodule update --remote
    ```

*   One could also directly download the submodule from the source:

    ```bash
    git clone https://github.com/gnthibault/Solver.git
    cd ./binding/python/pybind11/
    git submodule init
    git submodule update
    ```

### Dependencies

If you want to enjoy some developer features of this project, please consider installing the following packages (example for ubuntu):  
sudo apt-get install doxygen graphviz cppcheck

If you want to enjoy cuda related features, you need the [NVIDIA CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit), and of course a NVIDIA GPU.

### Compilation

How to build ?
cd Solver
mkdir build; cd build  
cmake -DINSTALLDIR=/path-to-install-dir -DUSE_CUDA=ON -DUSE_NVTX=OFF -DCMAKE_BUILD_TYPE=Release -DWITH_PYTHON=ON -DPYBIND11_PYTHON_VERSION=3.6 -DTHRUST_BACKEND:STRING=CUDA ..  
make -j8 install

## How to test
In the build directory, do:  
make test

## How to generate doxygen documentation
In the build directory, do:  
make doc  
The documentation should be build in the "doc" directory

## How to perform static code analysis with cppcheck
In the build directory, do:  
make cppcheck  
The cppcheck report can be found in the directory cppcheckdir-report

## Getting started


