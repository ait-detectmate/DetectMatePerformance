
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
cp build/bind_class* src/detectmateperformance/_core/
cp build/bind_class* lib/

# Run tests
uv run pytest
