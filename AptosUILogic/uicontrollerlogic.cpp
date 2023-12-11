#include "uicontrollerlogic.h"
#include "../HDWallet/wallet.h"
#include "../REST/Model/AccountResourceCoin.h"
#include "../REST/Model/ResponseInfo.h"
#include "../REST/RestClient.cpp"
#include "../REST/Constant.h"
#include "../HDWallet/Utils/Utils.h"

#include <memory>
namespace AptosUILogic {
int accountNumLimit = 10;
char* stringToChar(const std::string& str) {
    char* charArray = new char[str.length() + 1];
    strcpy(charArray, str.c_str());
    return charArray;
}

std::string charToString(const char* str) {
    return std::string(str);;
}

class UIController
{
public:
    UIController();
    void setWallet(const std::shared_ptr<Aptos::HDWallet::Wallet> &newWallet);
    std::string getMnemonicsKey() const;
    int currentAddressIndexKey() const;
    void setCurrentAddressIndexKey(int newCurrentAddressIndexKey);
    std::vector<std::string> getWalletAddress();
    std::string getCurrentWalletAddress() const;
    std::string getPrivateKey();
    void loadCurrentWalletBalance();
    void setNetwork(std::string _target);
    std::string getBalanceText() const;
    static float AptosTokenToFloat(float _token);
    static int AptosFloatToToken(float _amount);

private:
    std::shared_ptr<Aptos::HDWallet::Wallet> m_wallet;
    int m_currentAddressIndexKey;
    std::vector<std::string> m_addressList;
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
    if (m_wallet) {
        return m_wallet->getMnemonicsKey();
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
    m_addressList.clear();
    for (int i = 0; i < accountNumLimit; i++)
    {
        auto account = m_wallet->GetDerivedAccount(i);
        auto addr = account.getAccountAddress()->ToString();
        m_addressList.push_back(addr);
    }

    return m_addressList;
}

std::string UIController::getCurrentWalletAddress() const
{
    return m_addressList.at(m_currentAddressIndexKey);
}

std::string UIController::getPrivateKey()
{
    return m_wallet->account().getPrivateKey()->ToString();
}

void UIController::loadCurrentWalletBalance()
{
    AptosRESTModel::AccountResourceCoin::Coin coin;
    AptosRESTModel::ResponseInfo responseInfo;
    m_restClient.GetAccountBalance([this, &coin, &responseInfo](AptosRESTModel::AccountResourceCoin::Coin _coin, AptosRESTModel::ResponseInfo _responseInfo) {
        coin = _coin;
        responseInfo = _responseInfo;
        if(responseInfo.status != AptosRESTModel::ResponseInfo::Status::Success)
        {
            m_balance = 0.0f;
        }
        else
        {
            m_balance = std::stof(coin.value());
        }
    }, m_wallet->GetDerivedAccount(m_currentAddressIndexKey).getAccountAddress()->ToString());
}

void UIController::setNetwork(std::string _target)
{
    if (_target == "Mainnet") {
        m_restClient.SetEndpoint(Constants::MAINNET_BASE_URL);
    } else if (_target == "Devnet") {
        m_restClient.SetEndpoint(Constants::DEVNET_BASE_URL);
    } else if (_target == "Testnet") {
        m_restClient.SetEndpoint(Constants::TESTNET_BASE_URL);
    } else {
        m_restClient.SetEndpoint(Constants::DEVNET_BASE_URL);
    }
}

std::string UIController::getBalanceText() const
{
    return Aptos::Utils::floatToStringWithFormat(UIController::AptosTokenToFloat(m_balance), 4) + " APT";
}

float UIController::AptosTokenToFloat(float _token) {
    return _token / 100000000.0f;
}

int UIController::AptosFloatToToken(float _amount) {
    return std::round(_amount * 100000000.0);
}

}

void *AptosUILogic_createUiController()
{
    return new AptosUILogic::UIController();
}

void AptosUILogic_deleteUiController(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    delete ptr;
}

bool AptosUILogic_createNewWallet(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    bc::data_chunk my_entropy(16);
    bc::pseudo_random_fill(my_entropy);

    bc::wallet::word_list my_word_list = bc::wallet::create_mnemonic(
        my_entropy, bc::wallet::language::en);
    ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(bc::join(my_word_list)));
    ptr->setCurrentAddressIndexKey(0);
    ptr->getWalletAddress();
    //ptr->loadCurrentWalletBalance();

    return true;
}

char* AptosUILogic_getMnemonicsKey(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    return AptosUILogic::stringToChar(ptr->getMnemonicsKey());
}

char* AptosUILogic_getCurrentWalletBalanceText(void *controller)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    return AptosUILogic::stringToChar(ptr->getBalanceText());
}

void AptosUILogic_setNetwork(void *controller, const char* target)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    ptr->setNetwork(AptosUILogic::charToString(target));
}

bool AptosUILogic_initWalletFromCache(void *controller, char* mnemonicsKey, int currentAddressIndexKey)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(AptosUILogic::charToString(mnemonicsKey)));
    ptr->setCurrentAddressIndexKey(currentAddressIndexKey);
    ptr->getWalletAddress();
    ptr->loadCurrentWalletBalance();
    return true;
}

bool AptosUILogic_restoreWallet(void *controller, const char* mnemonicsKey)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    ptr->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(AptosUILogic::charToString(mnemonicsKey)));
    ptr->setCurrentAddressIndexKey(0);
    ptr->getWalletAddress();
    ptr->loadCurrentWalletBalance();
    return true;
}

void AptosUILogic_deleteString(char *str)
{
    delete[] str;
}

void AptosUILogic_deleteStringArray(char **strArr, int size)
{
    for (std::size_t i = 0; i < size; ++i) {
        AptosUILogic_deleteString(strArr[i]);
    }
    delete[] strArr;
}

char** AptosUILogic_getWalletAddress(void *controller, size_t *size)
{
    auto ptr = static_cast<AptosUILogic::UIController*>(controller);
    auto addressList = ptr->getWalletAddress();
    *size = addressList.size();
    if (size == 0) {
        return nullptr;
    }
    char** ret = new char*[*size];
    for (std::size_t i = 0; i < *size; ++i) {
        ret[i] = AptosUILogic::stringToChar(addressList[i]);
    }
    return ret;
}
