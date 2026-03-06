
# DetectMate Performance

Library to run high performance operations in DetectMate. It combines C++ functionality with [`Polars`](https://pola.rs/) DataFrames.


## Installation

[`CMake`](https://cmake.org/) allows to build the C++ code:

```bash
sudo apt-get install cmake
```

Intall python dependencies:

```bash
pip install uv
uv sync
```

Compile the code, make sure that all unit tests are green:

```bash
sh compile.sh
```

To run the C++ and Python unit tests:

```bash
pytest
```