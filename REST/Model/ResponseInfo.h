#ifndef RESPONSEINFO_H
#define RESPONSEINFO_H

#include <string>
namespace AptosRESTModel {

class ResponseInfo {
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
