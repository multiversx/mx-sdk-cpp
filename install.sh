CURRENT_DIR=$(pwd)
INCLUDE_PATH=/usr/include
LIB_PATH=/usr/lib

echo "Installing libsodium library..."
sudo dpkg --configure -a
sudo apt install libsodium-dev || exit 1

echo "Updating submodules..."
git submodule update --init --recursive || exit 1

if ! dpkg -s libssl-dev > /dev/null; then
  echo "OpenSSL libssl-dev not installed, installing..."
  sudo apt-get install libssl-dev || exit 1
fi

echo "CMake"
if ! dpkg -s cmake > /dev/null; then
  echo "CMake not installed, installing..."
  sudo apt install cmake || sudo snap install cmake --classic || exit 1
fi
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
