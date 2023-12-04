#include "uicontrollerlogic.h"
#include "../HDWallet/wallet.h"
#include <memory>
namespace AptosUILogic {

class UIController
{
public:
    UIController();
    void setWallet(const std::shared_ptr<Aptos::HDWallet::Wallet> &newWallet);

private:
    std::shared_ptr<Aptos::HDWallet::Wallet> m_wallet;
};


UIController::UIController() {}

void UIController::setWallet(const std::shared_ptr<Aptos::HDWallet::Wallet> &newWallet)
{
    m_wallet = newWallet;
}

UIController *createUiController()
{
    return new UIController();
}

void deleteUiController(UIController *controller)
{
    delete controller;
}

bool createNewWallet(UIController *controller)
{
    bc::data_chunk my_entropy(16);
    bc::pseudo_random_fill(my_entropy);

    bc::wallet::word_list my_word_list = bc::wallet::create_mnemonic(
        my_entropy, bc::wallet::language::en);
    controller->setWallet(std::make_shared<Aptos::HDWallet::Wallet>(bc::join(my_word_list)));
    return true;
}

}
