cmake --build .

cd tests
cd test_src

#./test_apiresponse
#find -type f -executable -exec file -i '{}' \; | grep 'x-executable; charset=binary'


tst='test_'
cmake='CMake'
for d in */* ; do
    #echo "$d"
    if [[ "$d" == *"$tst"* ]]; then
      if [[ -x "$d" ]]; then
        if [[ "$d" != *"$cmake"* ]]; then
          echo "found $d"
          ./$d
          fi
      fi
    fi
done