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
    include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
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

include_directories(Src)
include_directories(ThirdParty/bip3x/include)
include_directories(${httplib_INCLUDE_DIR})
include_directories(${cryptopp_INCLUDE_DIRS_RELEASE})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${OpenSSL_INCLUDE_DIRS})

add_library(Aptos SHARED
        Src/BCS/Deserialization.cpp
        Src/BCS/Deserialization.h
        Src/BCS/Serialization.cpp
        Src/BCS/Serialization.h
        Src/BCS/BCSTypes.cpp
        Src/BCS/BCSTypes.h
        Src/BCS/Bytes.cpp
        Src/BCS/Bytes.h
        Src/BCS/Bool.cpp
        Src/BCS/Bool.h
        Src/BCS/U8.cpp
        Src/BCS/U8.h
        Src/BCS/U16.cpp
        Src/BCS/U16.h
        Src/BCS/U32.cpp
        Src/BCS/U32.h
        Src/BCS/U64.cpp
        Src/BCS/U64.h
        Src/BCS/U128.cpp
        Src/BCS/U128.h
        Src/BCS/U256.cpp
        Src/BCS/U256.h
        Src/BCS/TagSequence.cpp
        Src/BCS/TagSequence.h
        Src/BCS/Sequence.cpp
        Src/BCS/Sequence.h
        Src/BCS/ByteSequence.cpp
        Src/BCS/ByteSequence.h
        Src/BCS/BCSMap.cpp
        Src/BCS/BCSMap.h
        Src/BCS/BString.cpp
        Src/BCS/BString.h
        Src/Transactions/Rawtransaction.h
        Src/Transactions/Rawtransaction.cpp
        Src/Transactions/MultiAgentRawTransaction.h
        Src/Transactions/MultiAgentRawTransaction.cpp
        Src/Transactions/TransactionPayload.h
        Src/Transactions/TransactionPayload.cpp
        Src/BCS/Script.h
        Src/BCS/Script.cpp
        Src/BCS/ScriptArgument.h
        Src/BCS/ScriptArgument.cpp
        Src/BCS/EntryFunction.h
        Src/BCS/EntryFunction.cpp
        Src/BCS/ModuleId.h
        Src/BCS/ModuleId.cpp
        Src/Transactions/TransactionArgument.h
        Src/Transactions/TransactionArgument.cpp
        Src/Transactions/SignedTransaction.h
        Src/Transactions/SignedTransaction.cpp
        Src/Accounts/PublicKey.cpp
        Src/Accounts/PublicKey.h
        Src/HDWallet/Utils/Utils.cpp
        Src/HDWallet/Utils/Utils.h
        Src/Accounts/PrivateKey.cpp
        Src/Accounts/PrivateKey.h
        Src/Accounts/Types/MultiEd25519PublicKey.cpp
        Src/Accounts/Ed25519Signature.cpp
        Src/Accounts/Ed25519Signature.h
        Src/Accounts/AccountAddress.cpp
        Src/Accounts/Account.cpp
        Src/Accounts/AuthenticationKey.cpp
        Src/Accounts/Multipublickey.h
        Src/Accounts/Multipublickey.cpp
        Src/Accounts/Multisignature.h
        Src/Accounts/Multisignature.cpp
        Src/HDWallet/Wallet.h
        Src/HDWallet/Wallet.cpp
        Src/Transactions/Authenticator.h
        Src/Transactions/Authenticator.cpp
        Src/BCS/StructTag.h
        Src/BCS/StructTag.cpp
        Src/REST/Constant.h
        Src/REST/Constant.cpp
        Src/REST/Model/AccountData.h
        Src/REST/Model/AccountData.cpp
        Src/REST/Model/AccountResourceCoin.h
        Src/REST/Model/AccountResourceCoin.cpp
        Src/REST/Model/AccountResourceCollection.h
        Src/REST/Model/AccountResourceCollection.cpp
        Src/REST/Model/AccountResourceTokenStore.h
        Src/REST/Model/AccountResourceTokenStore.cpp
        Src/REST/Model/AptosError.h
        Src/REST/Model/AptosError.cpp
        Src/REST/Model/GasEstimation.h
        Src/REST/Model/GasEstimation.cpp
        Src/REST/Model/LedgerInfo.h
        Src/REST/Model/LedgerInfo.cpp
        Src/REST/Model/ResourceCollection.h
        Src/REST/Model/ResourceCollection.cpp
        Src/REST/Model/ResponseInfo.h
        Src/REST/Model/ResponseInfo.cpp
        Src/REST/Model/TableItemRequest.h
        Src/REST/Model/TableItemRequest.cpp
        Src/REST/Model/TableItemRequestNFT.h
        Src/REST/Model/TableItemRequestNFT.cpp
        Src/REST/Model/TableItemRequestTokenData.h
        Src/REST/Model/TableItemRequestTokenData.cpp
        Src/REST/Model/TokenDataId.h
        Src/REST/Model/TokenDataId.cpp
        Src/REST/Model/TokenDataIdRequest.h
        Src/REST/Model/TokenDataIdRequest.cpp
        Src/REST/Model/TokenIdRequest.h
        Src/REST/Model/TokenIdRequest.cpp
        Src/REST/Model/Transaction.h
        Src/REST/Model/Transaction.cpp
        Src/REST/Model/TransactionPayload.h
        Src/REST/Model/TransactionPayload.cpp
        Src/REST/Model/TransactionRequest.h
        Src/REST/Model/TransactionRequest.cpp
        Src/REST/Model/ViewRequest.h
        Src/REST/Model/ViewRequest.cpp
        Src/REST/Model/Resources/AccountResource.h
        Src/REST/Model/Resources/AccountResource.cpp
        Src/REST/Model/Resources/Base/CapabilityOffer.h
        Src/REST/Model/Resources/Base/CapabilityOffer.cpp
        Src/REST/Model/Resources/Base/IResourceBase.h
        Src/REST/Model/Resources/Base/IResourceBase.cpp
        Src/REST/Model/Resources/Base/ResourceDataBase.h
        Src/REST/Model/Resources/Base/ResourceDataBase.cpp
        Src/REST/Model/Resources/Base/ResourceEvent.h
        Src/REST/Model/Resources/Base/ResourceEvent.cpp
        Src/REST/Model/Resources/CollectionResource.h
        Src/REST/Model/Resources/CollectionResource.cpp
        Src/REST/Model/Resources/ObjectResource.h
        Src/REST/Model/Resources/ObjectResource.cpp
        Src/REST/Model/Resources/PropertyMapResource.h
        Src/REST/Model/Resources/PropertyMapResource.cpp
        Src/REST/Model/Resources/RoyaltyResource.h
        Src/REST/Model/Resources/RoyaltyResource.cpp
        Src/REST/Model/Resources/TokenResource.h
        Src/REST/Model/Resources/TokenResource.cpp
        Src/REST/Model/TableItemToken/TableItemToken.h
        Src/REST/Model/TableItemToken/TableItemToken.cpp
        Src/REST/Model/TableItemToken/TableItemTokenMetadata.h
        Src/REST/Model/TableItemToken/TableItemTokenMetadata.cpp
        Src/REST/Model/Resources/Base/ResourceBaseListConverter.h
        Src/REST/Model/Resources/Base/ResourceBaseListConverter.cpp
        Src/REST/Model/TransactionRequestConverter.h
        Src/REST/Model/TransactionRequestConverter.cpp
        Src/REST/Model/TransactionConverter.h
        Src/REST/Model/TransactionConverter.cpp
        Src/REST/FaucetClient.h
        Src/REST/FaucetClient.cpp
        Src/REST/RequestClient.h
        Src/REST/RequestClient.cpp
        Src/REST/AptosTokenClient.h
        Src/REST/AptosTokenClient.cpp
        Src/REST/RestClient.h
        Src/REST/RestClient.cpp
        Src/Accounts/Ed25519Bip32.h
        Src/Accounts/Ed25519Bip32.cpp
        Src/Accounts/RotationProofChallenge.h
        Src/Accounts/RotationProofChallenge.cpp
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
include_directories(${GTest_INCLUDE_DIRS})

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(GTest_LIB ${GTest_INCLUDE_DIRS}/../lib/libgtest.a )
  set(GTest_main_LIB ${GTest_INCLUDE_DIRS}/../lib/libgtest_main.a )
else()
  set(GTest_LIB GTest::gtest )
  set(GTest_main_LIB GTest::gtest_main )
endif()

add_executable(test_serialization Src/Test/test_serialization.cpp)
add_executable(test_account Src/Test/test_account.cpp)
add_executable(test_utils Src/Test/test_utils.cpp)
add_executable(test_wallet Src/Test/test_wallet.cpp)
add_executable(test_transaction_serialization Src/Test/test_transaction_serialization.cpp)

target_link_libraries(test_serialization PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_account PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_utils PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos ${CryptoPP_LIB})
target_link_libraries(test_wallet PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos ${CryptoPP_LIB} toolbox)
target_link_libraries(test_transaction_serialization PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos ${CryptoPP_LIB})

add_test(NAME SerializationTest COMMAND test_serialization)
add_test(NAME AccountTest COMMAND test_account)
add_test(NAME UtilsTest COMMAND test_utils)
add_test(NAME WalletTest COMMAND test_wallet)
add_test(NAME TransactionSerializationTest COMMAND test_transaction_serialization)
file(COPY Src/Test/two_by_two_transfer.mv DESTINATION ${CMAKE_BINARY_DIR})

# Link test executable against gtest & gtest_main
add_executable(runAllTests 
    Src/Test/test_account.cpp 
    Src/Test/test_utils.cpp 
    Src/Test/test_wallet.cpp 
    Src/Test/test_transaction_serialization.cpp 
    Src/Test/test_serialization.cpp
)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(runAllTests PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos toolbox ${CryptoPP_LIB})
else()
  target_link_libraries(runAllTests PRIVATE ${GTest_LIB} ${GTest_main_LIB} pthread Aptos toolbox ${cryptopp_LIB_DIRS_RELEASE}/libcryptopp.a  ${openssl_LIB_DIRS_RELEASE}/libssl.a
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
