#include <iostream>
#include<fstream>
#include<streambuf>
#include"json/include/nlohmann/json.hpp"
#include"rapidjson/include/rapidjson/rapidjson.h"
#include"rapidjson/include/rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include"chrono.h"

using namespace std;

int main()
{

    ifstream ifs("/home/johann/Documents/QtProjects/json_test/score_data.json");
    std::string str((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());

    std::cout << str << std::endl;;

    JChrono<int, chrono::nanoseconds> c;
    c.start();

    nlohmann::json nl_json = nlohmann::json::parse(str);

    std::cout << "Nl : " << c.timeSinceStart() << std::endl;

    c.trigger();

    rapidjson::Document d;
    d.Parse(str.c_str());

    std::cout << "Rj : " << c.timeSinceLastTrigger() << std::endl;

    int track_nbr  = nl_json["tracknbr"].get<int>();


    std::cout << "retrieve nl : " << c.timeSinceLastTrigger() << std::endl;

    int v = d["tracknbr"].GetInt();

    std::cout << "retrieve rj : " << c.timeSinceLastTrigger() << std::endl;

    for(auto & it : nl_json["lignbr"].items())
    {
        std::cout << "nl : " << it.value().get<int>() << std::endl;
    }

    std::cout << "iter nl : " << c.timeSinceLastTrigger() << std::endl;

    rapidjson::Value& obj = d["lignbr"].GetObj();

    for(rapidjson::Value::MemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr)
    {
        std::cout << "rj : " << itr->value.GetInt() << std::endl;

    }

    std::cout << "iter rj : " << c.timeSinceLastTrigger() << std::endl;

    std::string nl_pretty = nl_json.dump(4);

    std::cout << "pretty nl : " << c.timeSinceLastTrigger() << std::endl;

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    d.Accept(writer);
    std::string rj_pretty = buf.GetString();

    std::cout << "pretty nl : " << c.timeSinceLastTrigger() << std::endl;


    return 0;
}
