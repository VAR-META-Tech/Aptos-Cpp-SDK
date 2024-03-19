#include "uicontrollerlogic.h"
#include "HDWallet/Wallet.h"
#include "REST/Model/AccountResourceCoin.h"
#include "REST/Model/ResponseInfo.h"
#include "REST/RestClient.cpp"
#include "REST/Constant.h"
#include "HDWallet/Utils/Utils.h"
#include "REST/FaucetClient.h"
#include <memory>

namespace AptosUILogic
{
int accountNumLimit = 10;
char *stringToChar(const std::string &str)
{
    char *charArray = new char[str.length() + 1];
    strcpy(charArray, str.c_str());
    return charArray;
}

std::string charToString(const char *str)
{
    return std::string(str);
    ;
}

class UIController
{
public:
    UIController();
    void setWallet(const std::shared_ptr<Aptos::HDWallet::Wallet> &newWallet);
    std::string getMnemonicsKey() const;
    std::string getPrivateKey() const;
    int currentAddressIndexKey() const;
    void setCurrentAddressIndexKey(int newCurrentAddressIndexKey);
    std::vector<std::string> getWalletAddress();
    std::string getCurrentWalletAddress() const;
    void loadCurrentWalletBalance();
    void setNetwork(std::string _target);
    std::string getBalanceText();
    static float AptosTokenToFloat(float _token);
    static int AptosFloatToToken(float _amount);
    void airdrop(int amount);
    bool sendToken(std::string _targetAddress, long _amount);
    bool createCollection(std::string _collectionName, std::string _collectionDescription, std::string _collectionUri);
    bool createNFT(std::string _collectionName, std::string _tokenName, std::string _tokenDescription, int _supply, int _max, std::string _uri, int _royaltyPointsPerMillion);

private:
    std::shared_ptr<Aptos::HDWallet::Wallet> m_wallet;
    int m_currentAddressIndexKey;
    std::vector<Account> m_accountList;
    Aptos::Rest::RestClient m_restClient;
    float m_balance;
};

UIController::UIController() {}

void UIController::setWallet(const std::shared_ptr<Aptos::HDWallet::Wallet> &newWallet)
{
    m_wallet = newWallet;
}

std::string UIController::getMnemonicsKey() const
{
    if (m_wallet)
    {
        return m_wallet->getMnemonicsKey();
        m_wallet->Account();
    }
    return {};
}

std::string UIController::getPrivateKey() const
{
    if (m_wallet)
    {
        return m_wallet->Account().getPrivateKey()->ToString();
    }
    return {};
}

int UIController::currentAddressIndexKey() const
{
    return m_currentAddressIndexKey;
}

void UIController::setCurrentAddressIndexKey(int newCurrentAddressIndexKey)
{
    m_currentAddressIndexKey = newCurrentAddressIndexKey;
}

std::vector<std::string> UIController::getWalletAddress()
{
    m_accountList.clear();
    std::vector<std::string> addressList;
    for (int i = 0; i < accountNumLimit; i++)
    {
        auto account = m_wallet->GetDerivedAccount(i);
        auto addr = account.getAccountAddress()->ToString();
        m_accountList.push_back(account);
        addressList.push_back(addr);
    }

    return addressList;
}

std::string UIController::getCurrentWalletAddress() const
{
    return m_accountList.at(m_currentAddressIndexKey).getAccountAddress()->ToString();
}

void UIController::loadCurrentWalletBalance()
{
    AptosRESTModel::AccountResourceCoin::Coin coin;
    AptosRESTModel::ResponseInfo responseInfo;
    m_restClient.GetAccountBalance([this, &coin, &responseInfo](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo)
                                   {
                                       coin = _coin;
                                       responseInfo = _responseInfo;
                                       if(responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
                                       {
                                           m_balance = 0.0f;
                                       }
                                       else
                                       {
                                           m_balance = std::stof(coin.value());
                                       } },
                                   *m_accountList.at(m_currentAddressIndexKey).getAccountAddress());
}

void UIController::setNetwork(std::string _target)
{
    if (_target == "Mainnet")
    {
        m_restClient.SetEndpoint(Constants::MAINNET_BASE_URL);
    }
    else if (_target == "Devnet")
    {
        m_restClient.SetEndpoint(Constants::DEVNET_BASE_URL);
    }
    else if (_target == "Testnet")
    {
        m_restClient.SetEndpoint(Constants::TESTNET_BASE_URL);
    }
    else
    {
        m_restClient.SetEndpoint(Constants::DEVNET_BASE_URL);
    }
}

std::string UIController::getBalanceText()
{
    loadCurrentWalletBalance();
    return Aptos::Utils::floatToStringWithFormat(UIController::AptosTokenToFloat(m_balance), 4) + " APT";
}

float UIController::AptosTokenToFloat(float _token)
{
    return _token / 100000000.0f;
}

int UIController::AptosFloatToToken(float _amount)
{
    return std::round(_amount * 100000000.0);
}

void UIController::airdrop(int amount)
{
    std::string faucetEndpoint = "https://faucet.devnet.aptoslabs.com";
    Aptos::Rest::FaucetClient::FundAccount([amount](bool success, AptosRESTModel::ResponseInfo)
                                           {
                                               if (success) {
                                                   std::cout << "Successfully Get Airdrop of " << (float)amount << " APT" << std::endl;
                                               } else {
                                                   std::cout << "airdrop failed" << std::endl;
                                               } },
                                           m_accountList.at(m_currentAddressIndexKey).getAccountAddress()->ToString(),
                                           amount,
                                           faucetEndpoint);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loadCurrentWalletBalance();
}

bool UIController::sendToken(std::string _targetAddress, long _amount)
{
    std::shared_ptr<AptosRESTModel::Transaction> transferTxn;
    AptosRESTModel::ResponseInfo responseInfo;
    bool waitForTxnSuccess = false;

    m_restClient.Transfer([&](const auto &_transferTxn, const auto &_responseInfo)
                          {
                              transferTxn = _transferTxn;
                              responseInfo = _responseInfo; },
                          m_accountList.at(m_currentAddressIndexKey), _targetAddress, _amount);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Success)
    {
        std::string transactionHash = transferTxn->getHash();
        m_restClient.WaitForTransaction([&](bool _pending, const auto &_responseInfo)
                                        {
                                            waitForTxnSuccess = _pending;
                                            responseInfo = _responseInfo; },
                                        transactionHash);
    }
    else
    {
        waitForTxnSuccess = false;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loadCurrentWalletBalance();
    return waitForTxnSuccess;
}

bool UIController::createCollection(std::string _collectionName, std::string _collectionDescription, std::string _collectionUri)
{
    AptosRESTModel::Transaction createCollectionTxn;
    AptosRESTModel::ResponseInfo responseInfo;
    m_restClient.CreateCollection([&](AptosRESTModel::Transaction _createCollectionTxn, AptosRESTModel::ResponseInfo _responseInfo)
                                  {
                                      createCollectionTxn = _createCollectionTxn;
                                      responseInfo = _responseInfo; },
                                  m_accountList.at(m_currentAddressIndexKey),
                                  _collectionName, _collectionDescription, _collectionUri);
    bool success = false;
    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Success)
    {
        success = true;
    }
    else
    {
        success = false;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loadCurrentWalletBalance();
    return success;
}

bool UIController::createNFT(std::string _collectionName, std::string _tokenName, std::string _tokenDescription, int _supply, int _max, std::string _uri, int _royaltyPointsPerMillion)
{
    bool success = false;
    AptosRESTModel::Transaction createTokenTxn;
    AptosRESTModel::ResponseInfo responseInfo;
    m_restClient.CreateToken([&](const auto &_createTokenTxn, const auto &_responseInfo)
                             {
                                 createTokenTxn = _createTokenTxn;
                                 responseInfo = _responseInfo; },
                             m_accountList.at(m_currentAddressIndexKey),
                             _collectionName,
                             _tokenName,
                             _tokenDescription,
                             _supply,
                             _max,
                             _uri,
                             _royaltyPointsPerMillion);

    if (responseInfo.status == AptosRESTModel::ResponseInfo::Status::Success)
    {
        success = true;
    }
    else
    {
        success = false;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loadCurrentWalletBalance();
    return success;
}

}

void *AptosUILogic_createUiController()
{
    return new AptosUILogic::UIController();
}

void AptosUILogic_deleteUiController(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    delete ptr;
}

bool AptosUILogic_createNewWallet(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    try {
        bip3x::bip3x_mnemonic::mnemonic_result mnemonic = bip3x::bip3x_mnemonic::generate();
        ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(mnemonic.raw));
        ptr->setCurrentAddressIndexKey(0);
        ptr->getWalletAddress();
        ptr->loadCurrentWalletBalance();
        return true;
    } catch (std::exception&) {
        return false;
    }

}

char *AptosUILogic_getMnemonicsKey(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return AptosUILogic::stringToChar(ptr->getMnemonicsKey());
}

char *AptosUILogic_getCurrentWalletBalanceText(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return AptosUILogic::stringToChar(ptr->getBalanceText());
}

void AptosUILogic_setNetwork(void *controller, const char *target)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    ptr->setNetwork(AptosUILogic::charToString(target));
}

bool AptosUILogic_initWalletFromCache(void *controller, char *mnemonicsKey, int currentAddressIndexKey)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    try {
        ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(AptosUILogic::charToString(mnemonicsKey)));
        ptr->setCurrentAddressIndexKey(currentAddressIndexKey);
        ptr->getWalletAddress();
        ptr->loadCurrentWalletBalance();
        return true;
    } catch (std::exception&) {
        return false;
    }
}

bool AptosUILogic_restoreWallet(void *controller, const char *mnemonicsKey)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    try {
        ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(AptosUILogic::charToString(mnemonicsKey)));
        ptr->setCurrentAddressIndexKey(0);
        ptr->getWalletAddress();
        ptr->loadCurrentWalletBalance();
        return true;
    } catch (std::exception&) {
        return false;
    }
}

void AptosUILogic_deleteString(char *str)
{
    delete[] str;
}

void AptosUILogic_deleteStringArray(char **strArr, int size)
{
    for (std::size_t i = 0; i < size; ++i)
    {
        AptosUILogic_deleteString(strArr[i]);
    }
    delete[] strArr;
}

char **AptosUILogic_getWalletAddress(void *controller, size_t *size)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    auto addressList = ptr->getWalletAddress();
    *size = addressList.size();
    if (size == 0)
    {
        return nullptr;
    }
    char **ret = new char *[*size];
    for (std::size_t i = 0; i < *size; ++i)
    {
        ret[i] = AptosUILogic::stringToChar(addressList[i]);
    }
    return ret;
}

char *AptosUILogic_onWalletListDropdownValueChanged(void *controller, int listIndex)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    ptr->setCurrentAddressIndexKey(listIndex);
    ptr->loadCurrentWalletBalance();
    return AptosUILogic::stringToChar(ptr->getCurrentWalletAddress());
}

char *AptosUILogic_getCurrentWalletAddress(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return AptosUILogic::stringToChar(ptr->getCurrentWalletAddress());
}

char *AptosUILogic_getPrivateKey(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return AptosUILogic::stringToChar(ptr->getPrivateKey());
}

void AptosUILogic_airdrop(void *controller, int amount)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    ptr->airdrop(amount);
}

bool AptosUILogic_sendToken(void *controller, const char *_targetAddress, long _amount)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return ptr->sendToken(AptosUILogic::charToString(_targetAddress), _amount);
}

bool AptosUILogic_createCollection(void *controller, const char *_collectionName, const char *_collectionDescription, const char *_collectionUri)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return ptr->createCollection(AptosUILogic::charToString(_collectionName), AptosUILogic::charToString(_collectionDescription), AptosUILogic::charToString(_collectionUri));
}

bool AptosUILogic_createNFT(void *controller, const char *_collectionName, const char *_tokenName, const char *_tokenDescription, int _supply, int _max, const char *_uri, int _royaltyPointsPerMillion)
{
    auto ptr = static_cast<AptosUILogic::UIController *>(controller);
    return ptr->createNFT(AptosUILogic::charToString(_collectionName), AptosUILogic::charToString(_tokenName), AptosUILogic::charToString(_tokenDescription), _supply, _max, AptosUILogic::charToString(_uri), _royaltyPointsPerMillion);
}
