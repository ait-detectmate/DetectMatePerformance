# DetectMate Performance

`DetectMate Performance` is a library that performs high-performance operations in DetectMate. While the `DetectMate Library` runs everything in a streaming manner, `DetectMate Performance` allows you to process data in batches as well.

It combines C++ functionality with [`Polars`](https://pola.rs/) DataFrames. Currently the library supports:

| | Python 3.12 | Python 3.13|
|--|----|---|
**x86_64** | Yes | Yes |
**arch64** | No | Yes |
**amd64** | No | No |


## Installation

Intall python dependencies:

```bash
pip install uv
uv sync
prek install

```
## Compilation

Only need it if the library does not have binaries to your setup. The code can be compiled directly in your machine or with a Docker simulator.

The binaries can be found in `src/detectmateperformance/lib` and they follow the next format:

```
bind_class.cpython-<PYTHON_VERSION>-<ARCHITECTURE>-<OS>.so
```

### Witout Docker

Install [`CMake`](https://cmake.org/) allows to build the C++ code:

```bash
sudo apt-get install cmake
```

Compile the code to run in your machine, make sure that all unit tests pass:

```bash
sh compile.sh
```

### With Docker

Using `Docker` as a simulator simplifies the pipeline an allows compiling the code for multiple architecutres. For more information about Docker cross-compilation check [here](https://docs.docker.com/build/building/multi-platform/).

Install:

```
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
```

To compile all the binaries:

```
docker compose up
```

To compile a single type of binary select one from the docker compose file:

```
docker compose up arm64_builder
```

## Run unit tests
To run the C++ and Python unit tests:

```bash
uv run pytest
```

### To do a local instalation in a different venv

```bash
uv pip install --no-cache-dir <directory_detectmateperformance>
```
