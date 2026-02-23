
# Remove old build
rm -rf build
mkdir build

# Run cmake and build
cmake -B build
cd build
make
cd ..

# Update python dependencies
uv sync

# Generate bindings
cd DetectMatePerformance/src
c++ -O3 -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) _core/_type/messages.cpp _core/_type/templates.cpp _core/message_bind.cpp -o message_class$(python3-config --extension-suffix)
cd ../..

# Run tests
uv run pytest