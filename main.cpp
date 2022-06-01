#include <iostream>
#include <sstream>
#include "crow.h"
#include "function.h"
using namespace std;

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/test").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if(!x)
            return crow::response(400);

        int sum = x["a"].i()+x["b"].i();
        std::ostringstream os;
        os << sum;

        return crow::response{os.str()};
    });

    CROW_ROUTE(app, "/").methods("POST"_method)([](const crow::request& req)
    {
        std::string rsaEncrypted, aesEncrypted, pinId, readData;

        auto getJson = crow::json::load(req.body);
        if(!getJson)
            return crow::response(400);

        Function* function = new Function();

        std::tie(rsaEncrypted, aesEncrypted, pinId) = function->requestParser(getJson);

        readData = function->decryptData(rsaEncrypted, aesEncrypted);

        std::ostringstream os;
        os << readData;
        
        delete function;
        return crow::response{os.str()};
    });

    app.port(3000).multithreaded().run();
}