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
        auto getJson = crow::json::load(req.body);
        if(!getJson)
            return crow::response(400);

        Function test = Function();

        string result = test.requestParser(getJson);

        std::ostringstream os;
        os << result;
        return crow::response{os.str()};
    });

    app.port(3000).multithreaded().run();
}