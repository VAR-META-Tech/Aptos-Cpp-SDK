cmake_minimum_required(VERSION 3.27.6)
project(Aptos VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

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

include_directories(Plugins/include)
link_directories(Plugins/lib)
include_directories(${httplib_INCLUDE_DIR})
include_directories(${cryptopp_INCLUDE_DIRS_RELEASE})

add_library(Aptos SHARED
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
        BCS/rawtransaction.h
        BCS/rawtransaction.cpp
        BCS/MultiAgentRawTransaction.h
        BCS/MultiAgentRawTransaction.cpp
        BCS/TransactionPayload.h
        BCS/TransactionPayload.cpp
        BCS/Script.h
        BCS/Script.cpp
        BCS/ScriptArgument.h
        BCS/ScriptArgument.cpp
        BCS/EntryFunction.h
        BCS/EntryFunction.cpp
        BCS/ModuleId.h
        BCS/ModuleId.cpp
        BCS/TransactionArgument.h
        BCS/TransactionArgument.cpp
        BCS/SignedTransaction.h
        BCS/SignedTransaction.cpp
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
        Accounts/multipublickey.h
        Accounts/multipublickey.cpp
        Accounts/multisignature.h
        Accounts/multisignature.cpp
        HDWallet/wallet.h
        HDWallet/wallet.cpp
        BCS/Authenticator.h
        BCS/Authenticator.cpp
        BCS/StructTag.h
        BCS/StructTag.cpp
        REST/Constant.h
        REST/Constant.cpp
        REST/Model/AccountData.h
        REST/Model/AccountData.cpp
        REST/Model/AccountResourceCoin.h
        REST/Model/AccountResourceCoin.cpp
        REST/Model/AccountResourceCollection.h
        REST/Model/AccountResourceCollection.cpp
        REST/Model/AccountResourceTokenStore.h
        REST/Model/AccountResourceTokenStore.cpp
        REST/Model/AptosError.h
        REST/Model/AptosError.cpp
        REST/Model/GasEstimation.h
        REST/Model/GasEstimation.cpp
        REST/Model/LedgerInfo.h
        REST/Model/LedgerInfo.cpp
        REST/Model/ResourceCollection.h
        REST/Model/ResourceCollection.cpp
        REST/Model/ResponseInfo.h
        REST/Model/ResponseInfo.cpp
        REST/Model/TableItemRequest.h
        REST/Model/TableItemRequest.cpp
        REST/Model/TableItemRequestNFT.h
        REST/Model/TableItemRequestNFT.cpp
        REST/Model/TableItemRequestTokenData.h
        REST/Model/TableItemRequestTokenData.cpp
        REST/Model/TokenDataId.h
        REST/Model/TokenDataId.cpp
        REST/Model/TokenDataIdRequest.h
        REST/Model/TokenDataIdRequest.cpp
        REST/Model/TokenIdRequest.h
        REST/Model/TokenIdRequest.cpp
        REST/Model/Transaction.h
        REST/Model/Transaction.cpp
        REST/Model/TransactionPayload.h
        REST/Model/TransactionPayload.cpp
        REST/Model/TransactionRequest.h
        REST/Model/TransactionRequest.cpp
        REST/Model/ViewRequest.h
        REST/Model/ViewRequest.cpp
        REST/Model/Resources/AccountResource.h
        REST/Model/Resources/AccountResource.cpp
        REST/Model/Resources/Base/CapabilityOffer.h
        REST/Model/Resources/Base/CapabilityOffer.cpp
        REST/Model/Resources/Base/IResourceBase.h
        REST/Model/Resources/Base/IResourceBase.cpp
        REST/Model/Resources/Base/ResourceDataBase.h
        REST/Model/Resources/Base/ResourceDataBase.cpp
        REST/Model/Resources/Base/ResourceEvent.h
        REST/Model/Resources/Base/ResourceEvent.cpp
        REST/Model/Resources/CollectionResource.h
        REST/Model/Resources/CollectionResource.cpp
        REST/Model/Resources/ObjectResource.h
        REST/Model/Resources/ObjectResource.cpp
        REST/Model/Resources/PropertyMapResource.h
        REST/Model/Resources/PropertyMapResource.cpp
        REST/Model/Resources/RoyaltyResource.h
        REST/Model/Resources/RoyaltyResource.cpp
        REST/Model/Resources/TokenResource.h
        REST/Model/Resources/TokenResource.cpp
        REST/Model/TableItemToken/TableItemToken.h
        REST/Model/TableItemToken/TableItemToken.cpp
        REST/Model/TableItemToken/TableItemTokenMetadata.h
        REST/Model/TableItemToken/TableItemTokenMetadata.cpp
        REST/Model/Resources/Base/ResourceBaseListConverter.h
        REST/Model/Resources/Base/ResourceBaseListConverter.cpp
        REST/Model/TransactionRequestConverter.h
        REST/Model/TransactionRequestConverter.cpp
        REST/Model/TransactionConverter.h
        REST/Model/TransactionConverter.cpp
        REST/FaucetClient.h
        REST/FaucetClient.cpp
        REST/RequestClient.h
        REST/RequestClient.cpp
        REST/AptosTokenClient.h
        REST/AptosTokenClient.cpp
        REST/RestClient.h
        REST/RestClient.cpp
        Accounts/Ed25519Bip32.h
        Accounts/Ed25519Bip32.cpp
        Accounts/RotationProofChallenge.h
        Accounts/RotationProofChallenge.cpp
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

add_executable(test_serialization Test/test_serialization.cpp)
add_executable(test_account Test/test_account.cpp)
add_executable(test_utils Test/test_utils.cpp)
add_executable(test_wallet Test/test_wallet.cpp)
add_executable(test_transaction_serialization Test/test_transaction_serialization.cpp)

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
file(COPY Test/two_by_two_transfer.mv DESTINATION ${CMAKE_BINARY_DIR})

# Link test executable against gtest & gtest_main
add_executable(runAllTests 
    Test/test_account.cpp 
    Test/test_utils.cpp 
    Test/test_wallet.cpp 
    Test/test_transaction_serialization.cpp 
    Test/test_serialization.cpp
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
