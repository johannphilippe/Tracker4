#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include<string>
#include<fstream>
#include<streambuf>
#include<sstream>

namespace jtracker { namespace string {

// File to string utilities
inline std::string read_file_as_string(std::string path)
{
        std::ifstream ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());
        return str;
}

inline void write_file_as_string(std::string path, std::string &to_write)
{
    std::ofstream ofs(path);
    ofs << to_write;
}

// number conversion utilities

//get a Number from a std::string (set precision)
template<typename T> static T get_number(std::string &val,int precision=5) {
    std::stringstream ss(val);
    ss.precision(precision);
    T number;
    ss >> number;
    return number;
}

//gets a string from a number with custom precision
template<typename T>
static std::string get_string(T val, int precision = 5) {
        std::ostringstream ss;
        ss.precision(precision);
        ss << val;
        return ss.str();
}

}}

#endif // STRING_UTILITIES_H
