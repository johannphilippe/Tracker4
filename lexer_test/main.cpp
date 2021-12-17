#include <iostream>
#include<fstream>
#include<streambuf>

#include"lexertk/lexertk.hpp"
#include"chrono.h"

using namespace std;

void compare_checkups()
{
    std::vector<std::string> vec;
    std::unordered_set<std::string> set;
    for(int i = 0; i < 1000000; i++)
    {
        std::string s;
        for(int j = 0; j < 10; j++)
        {
            s += char(rand() % 255);
        }
       vec.push_back(s);
       set.insert(s);
    }


    int found = 0;
    JChrono<int, chrono::nanoseconds> ch;
    ch.trigger();
    while(found < 100)
    {
        int idx = rand() % 1000000;
        std::string s = vec[idx];
        for(size_t i = 0; i < vec.size(); i++)
        {
            if(vec[i] == s){
                found ++;
                break;
            }
        }
    }
    std::cout << "vector : " << ch.timeSinceLastTrigger() << std::endl;


    found = 0;
    ch.trigger();
    while(found < 100)
    {
       int idx = rand() % 1000000;
       std::string s = vec[idx];
       if(set.find(s) != set.end())
       {
           found++;
           continue;
       }
    }

    std::cout << "set : " << ch.timeSinceLastTrigger() << std::endl;

}



int main()
{


    ifstream lua_file("/home/johann/Documents/QtProjects/lexer_test/test.lua");
    std::string lua_str((std::istreambuf_iterator<char>(lua_file)),
                     std::istreambuf_iterator<char>());
    ifstream cpp_file("/home/johann/Documents/QtProjects/lexer_test/test.cpp");
    std::string cpp_str((std::istreambuf_iterator<char>(cpp_file)),
                     std::istreambuf_iterator<char>());


    std::vector<const char *> arr = {"int", "double", "char", "bool", "if", "for", "while", "do", "class", "struct",
                                     "const" , "static", "constexpr", "using", "namespace", "return", "sizeof",
                                    "inline", "enum", "template", "typename", "else", "goto", "virtual", "volatile", "void",
                                     "friend", "break", "switch", "case", "default", "true", "false", "extern", "delete", "new","throw", "try" ,"catch",
                                     "signed", "auto", "continue", "public", "protected", "private", "typedef" ,"decltype", "unsigned", "this", "short","float", "explicit"
                                    };


    lexertk::syntax_descriptor syntax(
                lexertk::comment_style::CPP(),
                lexertk::macro_style::CPP(),
                arr
                );
    lexertk::generator lexer(syntax);
    JChrono<int, chrono::nanoseconds> timer;
    timer.trigger();

    std::cout << "CPP : \n\n" << std::endl;

    timer.trigger();
    if(!lexer.process(cpp_str))
    {
        std::cout << "lexer could not lex cpp" << std::endl;
    }

    cout << "cpp lex in " << timer.timeSinceLastTrigger() << "  nanoseconds " << std::endl;
    lexertk::helper::dump(lexer);





    return 0;
}
