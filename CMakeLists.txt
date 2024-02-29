cmake_minimum_required(VERSION 3.27.6)
project(Aptos VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(ThirdParty/bip3x) 

include(CTest)
enable_testing()
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    # include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
    include(FetchContent)
    fetchcontent_declare(
            toolbox
            GIT_REPOSITORY https://github.com/edwardstock/toolbox.git
            GIT_TAG 3.4.0
    )
    set(toolbox_BUILD_TESTS OFF)
    set(toolbox_BUILD_SHARED_LIBS OFF)
    fetchcontent_makeavailable(toolbox)

    fetchcontent_declare(
            bip3x
            GIT_REPOSITORY https://github.com/edwardstock/bip3x.git
            GIT_TAG 3.0.0
    )
    fetchcontent_makeavailable(bip3x)
    if(TARGET toolbox)
        target_compile_options(toolbox PRIVATE
            -include cstdint
        )
    endif()
    if(TARGET bip3x)
        target_compile_options(bip3x PRIVATE
            -include cstdint
        )
    endif()
    find_library(CryptoPP_LIB NAMES libcryptopp.a PATHS /usr/local/lib)
    if (NOT CryptoPP_LIB)
        message(FATAL_ERROR "Crypto++ library not found")
    endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    include(conan_toolchain.cmake)
    find_package(cryptopp REQUIRED)
    find_library(CryptoPP_LIB NAMES libcryptopp.a PATHS ${cryptopp_LIB_DIRS_RELEASE})
    if (NOT CryptoPP_LIB)
        message(FATAL_ERROR "Crypto++ library not found")
    endif()
    find_package(httplib REQUIRED)
endif()

# Check for doxygen presence
option(GEN_DOC "Enable generate document" OFF)
find_package(Doxygen)
if (GEN_DOC AND DOXYGEN_FOUND)
    # Set the path to the Doxyfile
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/html)

    # Add "doc" target for make (or "Documentation" for IDEs)
    add_custom_target(doc_doxygen ALL 
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
    find_package(cryptopp REQUIRED)
    find_package(httplib REQUIRED)
endif(DOXYGEN_FOUND)

find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Code coverage configuration
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_XCODE_ATTRIBUTE_ENABLE_TESTABILITY YES)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif()

include_directories(src)
include_directories(ThirdParty/bip3x/include)
include_directories(${httplib_INCLUDE_DIR})
include_directories(${cryptopp_INCLUDE_DIRS_RELEASE})

add_library(Aptos SHARED
        src/BCS/Deserialization.cpp
        src/BCS/Deserialization.h
        src/BCS/Serialization.cpp
        src/BCS/Serialization.h
        src/BCS/BCSTypes.cpp
        src/BCS/BCSTypes.h
        src/BCS/Bytes.cpp
        src/BCS/Bytes.h
        src/BCS/Bool.cpp
        src/BCS/Bool.h
        src/BCS/U8.cpp
        src/BCS/U8.h
        src/BCS/U16.cpp
        src/BCS/U16.h
        src/BCS/U32.cpp
        src/BCS/U32.h
        src/BCS/U64.cpp
        src/BCS/U64.h
        src/BCS/U128.cpp
        src/BCS/U128.h
        src/BCS/U256.cpp
        src/BCS/U256.h
        src/BCS/TagSequence.cpp
        src/BCS/TagSequence.h
        src/BCS/Sequence.cpp
        src/BCS/Sequence.h
        src/BCS/ByteSequence.cpp
        src/BCS/ByteSequence.h
        src/BCS/BCSMap.cpp
        src/BCS/BCSMap.h
        src/BCS/BString.cpp
        src/BCS/BString.h
        src/Transactions/Rawtransaction.h
        src/Transactions/Rawtransaction.cpp
        src/Transactions/MultiAgentRawTransaction.h
        src/Transactions/MultiAgentRawTransaction.cpp
        src/Transactions/TransactionPayload.h
        src/Transactions/TransactionPayload.cpp
        src/BCS/Script.h
        src/BCS/Script.cpp
        src/BCS/ScriptArgument.h
        src/BCS/ScriptArgument.cpp
        src/BCS/EntryFunction.h
        src/BCS/EntryFunction.cpp
        src/BCS/ModuleId.h
        src/BCS/ModuleId.cpp
        src/Transactions/TransactionArgument.h
        src/Transactions/TransactionArgument.cpp
        src/Transactions/SignedTransaction.h
        src/Transactions/SignedTransaction.cpp
        src/Accounts/PublicKey.cpp
        src/Accounts/PublicKey.h
        src/HDWallet/Utils/Utils.cpp
        src/HDWallet/Utils/Utils.h
        src/Accounts/PrivateKey.cpp
        src/Accounts/PrivateKey.h
        src/Accounts/Types/MultiEd25519PublicKey.cpp
        src/Accounts/Ed25519Signature.cpp
        src/Accounts/Ed25519Signature.h
        src/Accounts/AccountAddress.cpp
        src/Accounts/Account.cpp
        src/Accounts/AuthenticationKey.cpp
        src/Accounts/Multipublickey.h
        src/Accounts/Multipublickey.cpp
        src/Accounts/Multisignature.h
        src/Accounts/Multisignature.cpp
        src/HDWallet/Wallet.h
        src/HDWallet/Wallet.cpp
        src/Transactions/Authenticator.h
        src/Transactions/Authenticator.cpp
        src/BCS/StructTag.h
        src/BCS/StructTag.cpp
        src/REST/Constant.h
        src/REST/Constant.cpp
        src/REST/Model/AccountData.h
        src/REST/Model/AccountData.cpp
        src/REST/Model/AccountResourceCoin.h
        src/REST/Model/AccountResourceCoin.cpp
        src/REST/Model/AccountResourceCollection.h
        src/REST/Model/AccountResourceCollection.cpp
        src/REST/Model/AccountResourceTokenStore.h
        src/REST/Model/AccountResourceTokenStore.cpp
        src/REST/Model/AptosError.h
        src/REST/Model/AptosError.cpp
        src/REST/Model/GasEstimation.h
        src/REST/Model/GasEstimation.cpp
        src/REST/Model/LedgerInfo.h
        src/REST/Model/LedgerInfo.cpp
        src/REST/Model/ResourceCollection.h
        src/REST/Model/ResourceCollection.cpp
        src/REST/Model/ResponseInfo.h
        src/REST/Model/ResponseInfo.cpp
        src/REST/Model/TableItemRequest.h
        src/REST/Model/TableItemRequest.cpp
        src/REST/Model/TableItemRequestNFT.h
        src/REST/Model/TableItemRequestNFT.cpp
        src/REST/Model/TableItemRequestTokenData.h
        src/REST/Model/TableItemRequestTokenData.cpp
        src/REST/Model/TokenDataId.h
        src/REST/Model/TokenDataId.cpp
        src/REST/Model/TokenDataIdRequest.h
        src/REST/Model/TokenDataIdRequest.cpp
        src/REST/Model/TokenIdRequest.h
        src/REST/Model/TokenIdRequest.cpp
        src/REST/Model/Transaction.h
        src/REST/Model/Transaction.cpp
        src/REST/Model/TransactionPayload.h
        src/REST/Model/TransactionPayload.cpp
        src/REST/Model/TransactionRequest.h
        src/REST/Model/TransactionRequest.cpp
        src/REST/Model/ViewRequest.h
        src/REST/Model/ViewRequest.cpp
        src/REST/Model/Resources/AccountResource.h
        src/REST/Model/Resources/AccountResource.cpp
        src/REST/Model/Resources/Base/CapabilityOffer.h
        src/REST/Model/Resources/Base/CapabilityOffer.cpp
        src/REST/Model/Resources/Base/IResourceBase.h
        src/REST/Model/Resources/Base/IResourceBase.cpp
        src/REST/Model/Resources/Base/ResourceDataBase.h
        src/REST/Model/Resources/Base/ResourceDataBase.cpp
        src/REST/Model/Resources/Base/ResourceEvent.h
        src/REST/Model/Resources/Base/ResourceEvent.cpp
        src/REST/Model/Resources/CollectionResource.h
        src/REST/Model/Resources/CollectionResource.cpp
        src/REST/Model/Resources/ObjectResource.h
        src/REST/Model/Resources/ObjectResource.cpp
        src/REST/Model/Resources/PropertyMapResource.h
        src/REST/Model/Resources/PropertyMapResource.cpp
        src/REST/Model/Resources/RoyaltyResource.h
        src/REST/Model/Resources/RoyaltyResource.cpp
        src/REST/Model/Resources/TokenResource.h
        src/REST/Model/Resources/TokenResource.cpp
        src/REST/Model/TableItemToken/TableItemToken.h
        src/REST/Model/TableItemToken/TableItemToken.cpp
        src/REST/Model/TableItemToken/TableItemTokenMetadata.h
        src/REST/Model/TableItemToken/TableItemTokenMetadata.cpp
        src/REST/Model/Resources/Base/ResourceBaseListConverter.h
        src/REST/Model/Resources/Base/ResourceBaseListConverter.cpp
        src/REST/Model/TransactionRequestConverter.h
        src/REST/Model/TransactionRequestConverter.cpp
        src/REST/Model/TransactionConverter.h
        src/REST/Model/TransactionConverter.cpp
        src/REST/FaucetClient.h
        src/REST/FaucetClient.cpp
        src/REST/RequestClient.h
        src/REST/RequestClient.cpp
        src/REST/AptosTokenClient.h
        src/REST/AptosTokenClient.cpp
        src/REST/RestClient.h
        src/REST/RestClient.cpp
        src/Accounts/Ed25519Bip32.h
        src/Accounts/Ed25519Bip32.cpp
        src/Accounts/RotationProofChallenge.h
        src/Accounts/RotationProofChallenge.cpp
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(Aptos PRIVATE 
  nlohmann_json::nlohmann_json bip3x ${CryptoPP_LIB}  ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES}
  toolbox)
else()
  target_link_libraries(Aptos PRIVATE 
  nlohmann_json::nlohmann_json bip3x ${cryptopp_LIB_DIRS_RELEASE}/libcryptopp.a  ${openssl_LIB_DIRS_RELEASE}/libssl.a
  ${openssl_LIB_DIRS_RELEASE}/libcrypto.a ${zlib_LIB_DIRS_RELEASE}/libz.a
  toolbox)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_BUILD_TYPE Debug)
include(CPack)

add_executable(test_serialization src/Test/test_serialization.cpp)
add_executable(test_account src/Test/test_account.cpp)
add_executable(test_utils src/Test/test_utils.cpp)
add_executable(test_wallet src/Test/test_wallet.cpp)
add_executable(test_transaction_serialization src/Test/test_transaction_serialization.cpp)

target_link_libraries(test_serialization PRIVATE GTest::gtest GTest::gtest_main pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_account PRIVATE GTest::gtest GTest::gtest_main pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_utils PRIVATE GTest::gtest GTest::gtest_main pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_wallet PRIVATE GTest::gtest GTest::gtest_main pthread Aptos ${CryptoPP_LIB} toolbox)
target_link_libraries(test_transaction_serialization PRIVATE GTest::gtest GTest::gtest_main pthread Aptos ${CryptoPP_LIB})

add_test(NAME SerializationTest COMMAND test_serialization)
add_test(NAME AccountTest COMMAND test_account)
add_test(NAME UtilsTest COMMAND test_utils)
add_test(NAME WalletTest COMMAND test_wallet)
add_test(NAME TransactionSerializationTest COMMAND test_transaction_serialization)
file(COPY src/Test/two_by_two_transfer.mv DESTINATION ${CMAKE_BINARY_DIR})

# Link test executable against gtest & gtest_main
add_executable(runAllTests 
    src/Test/test_account.cpp 
    src/Test/test_utils.cpp 
    src/Test/test_wallet.cpp 
    src/Test/test_transaction_serialization.cpp 
    src/Test/test_serialization.cpp
)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(runAllTests PRIVATE GTest::gtest GTest::gtest_main pthread Aptos toolbox ${CryptoPP_LIB})
else()
  target_link_libraries(runAllTests PRIVATE GTest::gtest GTest::gtest_main pthread Aptos toolbox ${cryptopp_LIB_DIRS_RELEASE}/libcryptopp.a  ${openssl_LIB_DIRS_RELEASE}/libssl.a
  ${openssl_LIB_DIRS_RELEASE}/libcrypto.a ${zlib_LIB_DIRS_RELEASE}/libz.a)
endif()



if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_custom_target(coverage
    COMMAND xcrun ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
    COMMAND xcrun llvm-profdata merge -sparse default.profraw -o default.profdata
    COMMAND xcrun llvm-cov show ${CMAKE_CURRENT_BINARY_DIR}/runAllTests -instr-profile=default.profdata -format=html > coverage_report.html
    COMMAND xcrun llvm-cov report ${CMAKE_CURRENT_BINARY_DIR}/runAllTests -instr-profile=default.profdata > coverage_summary.txt
  )
endif()

add_library(AptosUILogic SHARED
    AptosUILogic/uicontrollerlogic.h
    AptosUILogic/uicontrollerlogic.cpp
)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(AptosUILogic PRIVATE nlohmann_json::nlohmann_json pthread Aptos bip3x ${CryptoPP_LIB})
else()
  target_link_libraries(AptosUILogic PRIVATE nlohmann_json::nlohmann_json pthread Aptos bip3x ${cryptopp_LIB_DIRS_RELEASE}/libcryptopp.a  ${openssl_LIB_DIRS_RELEASE}/libssl.a
  ${openssl_LIB_DIRS_RELEASE}/libcrypto.a ${zlib_LIB_DIRS_RELEASE}/libz.a)
endif()


add_executable(AptosUILogicTest
    AptosUILogic/main.cpp
)

target_link_libraries(AptosUILogicTest PRIVATE Aptos AptosUILogic )

add_executable(AptosSDKDemo
    AptosSDKDemo/AptosToken/Model/AddTokenPropertyResponse.h
    AptosSDKDemo/AptosToken/Model/AddTokenPropertyResponse.cpp
    AptosSDKDemo/AptosToken/Model/CreateCollectionResponse.h
    AptosSDKDemo/AptosToken/Model/CreateCollectionResponse.cpp
    AptosSDKDemo/AptosToken/Model/CreateTokenResponse.h
    AptosSDKDemo/AptosToken/Model/CreateTokenResponse.cpp
    AptosSDKDemo/AptosToken/Model/MintTokenResponse.h
    AptosSDKDemo/AptosToken/Model/MintTokenResponse.cpp
    AptosSDKDemo/AptosToken/Model/TransferObjectResponse.h
    AptosSDKDemo/AptosToken/Model/TransferObjectResponse.cpp
    AptosSDKDemo/AptosToken/Model/UpdateTokenResponse.h
    AptosSDKDemo/AptosToken/Model/UpdateTokenResponse.cpp
    AptosSDKDemo/Multisig/Model/MultisigTransferBcsTransactionResponse.h
    AptosSDKDemo/Multisig/Model/MultisigTransferBcsTransactionResponse.cpp
    AptosSDKDemo/Multisig/Model/RotateKeyBcsTransactionResponse.h
    AptosSDKDemo/Multisig/Model/RotateKeyBcsTransactionResponse.cpp
    AptosSDKDemo/TransferCoin/Model/TransferCoinBCSResponse.h
    AptosSDKDemo/TransferCoin/Model/TransferCoinBCSResponse.cpp
    AptosSDKDemo/AptosToken/AptosToken.h
    AptosSDKDemo/AptosToken/AptosToken.cpp
    AptosSDKDemo/main.cpp
    AptosSDKDemo/TransferCoin/TransferCoinExample.h
    AptosSDKDemo/TransferCoin/TransferCoinExample.cpp
    AptosSDKDemo/SimulateTransferCoin/SimulateTransferCoin.h
    AptosSDKDemo/SimulateTransferCoin/SimulateTransferCoin.cpp
    AptosSDKDemo/SimpleNftExample.h
    AptosSDKDemo/SimpleNftExample.cpp
    AptosSDKDemo/Multisig/Multisig.h
    AptosSDKDemo/Multisig/Multisig.cpp
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(AptosSDKDemo PRIVATE Aptos )
else()
  target_link_libraries(AptosSDKDemo PRIVATE Aptos nlohmann_json::nlohmann_json Aptos bip3x ${cryptopp_LIB_DIRS_RELEASE}/libcryptopp.a  ${openssl_LIB_DIRS_RELEASE}/libssl.a
${openssl_LIB_DIRS_RELEASE}/libcrypto.a ${zlib_LIB_DIRS_RELEASE}/libz.a)
endif()
