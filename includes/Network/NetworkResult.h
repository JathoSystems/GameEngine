#pragma once

#include "Enums/NetworkError.h"

class NetworkResult {
public:
    NetworkError error;
    std::string message;
    
    bool isSuccess() const { return error == NetworkError::SUCCESS; }
};
