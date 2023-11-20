# Aptos-Cpp-SDK

#For run code coverage on MacOS

First of all, make sure you have llvm and lcov installed. You can install them using brew:
brew install llvm lcov

To generate coverage data, you would then do:
cmake -DCODE_COVERAGE=ON ..
make
make test
make coverage