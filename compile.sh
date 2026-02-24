
# Update python dependencies
uv sync

# Remove old build
rm -rf build
mkdir build

# Run cmake and build
cmake -B build
cd build
make
cd ..

# Run tests
uv run pytest