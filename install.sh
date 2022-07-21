CURRENT_DIR=$(pwd)
INCLUDE_PATH=/usr/include/
LIB_PATH=/usr/lib

echo "Installing libsodium library..."
sudo apt install libsodium-dev

echo "Updating submodules..."
git submodule update --init --recursive

echo "Cmake"
cmake .

echo "Building solution..."
cmake --build .

echo "Installing sdk..."
sudo cp -r "$CURRENT_DIR/include/" "$INCLUDE_PATH/erdcpp"
sudo cp "$CURRENT_DIR/src/libsrc.so" "$LIB_PATH"

echo "Finished successfully !"
