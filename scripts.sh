cmake --build .

cd tests || exit

function runTests(){
  cd "$1" || exit

  for file in */* ; do
      #echo "$d"
      if [[ "$file" == *"test_"* ]] && [[ -x "$file" ]] && [[ "$file" != *"CMake"* ]]; then
        ./"$file"
      fi
  done

  cd ..
}

runTests test_cli
runTests test_src