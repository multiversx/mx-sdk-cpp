function runTests(){
  cd "$1" || exit 1

  for file in */* ; do
      if [[ "$file" == *"test_"* ]] && [[ -x "$file" ]] && [[ "$file" != *"CMake"* ]]; then
        if [[ "$file" != *"test_provider"* ]]; then 
          ./"$file"
          if [[ $? -ne 0 ]]; then
            exit 4
          fi
        fi
      fi
  done

  cd ..
}

cd tests || exit 1
runTests test_cli
runTests test_src
