#ifndef RESPONSEINFO_H
#define RESPONSEINFO_H

#include <string>
#include "BCS/BCSTypes.h"

namespace AptosRESTModel {

class APTOS_API ResponseInfo {
public:
    enum class Status {
        Success,
        Pending,
        NotFound,
        Failed,
        Warning
    };
    Status status;
    std::string message;
};
}
#endif // RESPONSEINFO_H
