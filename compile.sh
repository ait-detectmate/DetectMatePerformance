
# Update python dependencies
uv sync

# Remove old build
rm -rf build
rm src/detectmateperformance/_core/*.so
mkdir build

# Run cmake and build
cmake -B build
cd build
make
cd ..

# Move compiled C++ code
mkdir lib
cp build/bind_class* src/detectmateperformance/lib/

# Install package
uv pip uninstall detectmateperformance
uv pip install --no-cache-dir .

# Run tests
uv run pytest
