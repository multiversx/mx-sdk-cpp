cmake .
cmake --build .

function runTests(){
  cd "$1" || exit

  for file in */* ; do
      if [[ "$file" == *"test_"* ]] && [[ -x "$file" ]] && [[ "$file" != *"CMake"* ]]; then
        ./"$file"
      fi
  done

  cd ..
}

cd tests || exit
runTests test_cli
runTests test_src
