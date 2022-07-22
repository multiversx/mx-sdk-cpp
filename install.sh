CURRENT_DIR=$(pwd)
INCLUDE_PATH=/usr/include/
LIB_PATH=/usr/lib

echo "Installing libsodium library..."
sudo apt install libsodium-dev || exit 1

echo "Updating submodules..."
git submodule update --init --recursive || exit 1

echo "Cmake"
cmake . || exit 1

echo "Building solution..."
cmake --build . || exit 1

echo "Installing sdk..."

sudo cp -r "$CURRENT_DIR/include/" "$INCLUDE_PATH/erdcpp"
if [[ $? -ne 0 ]]; then
  echo "Could not copy include headers from $CURRENT_DIR/include/ to $INCLUDE_PATH/erdcpp"
  exit 1
fi

sudo cp "$CURRENT_DIR/src/libsrc.so" "$LIB_PATH"
if [[ $? -ne 0 ]]; then
  echo "Could not copy generated shared library from $CURRENT_DIR/src/libsrc.so to $LIB_PATH"
  exit 1
fi

echo "Finished successfully !"
