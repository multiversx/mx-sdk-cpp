cd ../..
pwd
ls -la
cd tests || exit 1
ls -la
cd test_src || exit 1
ls -la
cd test_provider || exit 1
./test_proxyprovider
