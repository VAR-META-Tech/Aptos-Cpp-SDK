#include <iostream>
#include "uicontrollerlogic.h"

int main() {
    AptosUILogic::UIController* controller = AptosUILogic::createUiController();
    AptosUILogic::createNewWallet(controller);
    AptosUILogic::deleteUiController(controller);
}
