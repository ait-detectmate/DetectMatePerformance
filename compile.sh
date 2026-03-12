
# Update python dependencies
uv sync

# Remove old build
rm -rf build
rm detectmateperformance/src/_core/*.so
mkdir build

# Run cmake and build
cmake -B build
cd build
make
cd ..

# Move compiled C++ code
cp build/bind_class* detectmateperformance/src/_core/ 

# Run tests
uv run pytest