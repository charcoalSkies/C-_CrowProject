#pragma once
#include "crow.h"
#include <string>

class Function
{

public:
    std::string requestParser(crow::json::rvalue request)
    {   
        std::string data1 = request["a"].s();

        return data1;
    }
};
