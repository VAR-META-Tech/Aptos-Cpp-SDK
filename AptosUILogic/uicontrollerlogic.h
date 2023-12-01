#ifndef UICONTROLLERLOGIC_H
#define UICONTROLLERLOGIC_H

#include <string>

namespace AptosUILogic {
class UIController;
UIController* createUiController();
void deleteUiController(UIController* controller);
bool createNewWallet(UIController* controller);
}
#endif // UICONTROLLERLOGIC_H
