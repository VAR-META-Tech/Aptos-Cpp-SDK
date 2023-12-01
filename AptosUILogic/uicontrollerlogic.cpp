#include "uicontrollerlogic.h"
#include "../HDWallet/wallet.h"
#include <memory>
namespace AptosUILogic {

class UIController
{
public:
    UIController();
private:
    std::shared_ptr<Aptos::HDWallet::Wallet> m_wallet;
};


UIController::UIController() {}

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
    std::cout << bc::join(my_word_list) << std::endl;
    return true;
}

}
