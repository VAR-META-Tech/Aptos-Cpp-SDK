# Aptos-Cpp-SDK
#For setup project 
conan install . -s compiler.cppstd=20 --build=missing

#For build source code on MacOS
To install dependency library:
brew install boost cpprestsdk sec256k1 libbitcoin-system doxygen llvm lcov nlohmann-json crytopp cmake googletest 

#For run code coverage on MacOS

First of all, make sure you have llvm and lcov installed. You can install them using brew:
brew install llvm lcov

To generate coverage data, you would then do:
cmake -DCODE_COVERAGE=ON ..
make
make test
make coverage