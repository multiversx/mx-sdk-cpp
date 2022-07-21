source $(dirname "$0")/helper-tests.sh

cd tests || exit 1
runTests test_integration
