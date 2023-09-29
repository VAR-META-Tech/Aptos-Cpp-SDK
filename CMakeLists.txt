cmake_minimum_required(VERSION 3.0.0)
project(Aptos VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(/usr/local/include)
include_directories(/opt/homebrew/include)
include_directories(Libs)

find_library(CryptoPP_LIB NAMES libcryptopp.a PATHS /usr/local/lib)
if (NOT CryptoPP_LIB)
    message(FATAL_ERROR "Crypto++ library not found")
endif()

add_library(Aptos
        BCS/Deserialization.cpp
        BCS/Deserialization.h
        BCS/Serialization.cpp
        BCS/Serialization.h
        BCS/BCSTypes.cpp
        BCS/BCSTypes.h
        BCS/Bytes.cpp
        BCS/Bytes.h
        BCS/Bool.cpp
        BCS/Bool.h
        BCS/U8.cpp
        BCS/U8.h
        BCS/U16.cpp
        BCS/U16.h
        BCS/U32.cpp
        BCS/U32.h
        BCS/U64.cpp
        BCS/U64.h
        BCS/U128.cpp
        BCS/U128.h
        BCS/U256.cpp
        BCS/U256.h
        BCS/TagSequence.cpp
        BCS/TagSequence.h
        BCS/Sequence.cpp
        BCS/Sequence.h
        BCS/ByteSequence.cpp
        BCS/ByteSequence.h
        BCS/BCSMap.cpp
        BCS/BCSMap.h
        BCS/BString.cpp
        BCS/BString.h
        Accounts/PublicKey.cpp
        Accounts/PublicKey.h
        HDWallet/Utils/Utils.cpp
        HDWallet/Utils/Utils.h
        Accounts/PrivateKey.cpp
        Accounts/PrivateKey.h
        Accounts/Types/MultiEd25519PublicKey.cpp
        Accounts/Signature.cpp
        Accounts/Signature.h
        Accounts/AccountAddress.cpp
        Accounts/Account.cpp
        Accounts/AuthenticationKey.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_serialization Test/test_serialization.cpp)
add_executable(test_account Test/test_account.cpp)

target_link_libraries(test_serialization ${GTEST_BOTH_LIBRARIES} pthread Aptos) # Link lib
target_link_libraries(test_account ${GTEST_BOTH_LIBRARIES} pthread Aptos) # Link

add_test(NAME SerializationTest COMMAND test_serialization)
add_test(NAME AccountTest COMMAND test_account)

target_link_libraries(Aptos PRIVATE ${CryptoPP_LIB})
