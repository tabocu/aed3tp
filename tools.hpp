#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <string>
#include <fstream>

namespace datastream
{
    void write_char(char * buffer, const char &c);
    void write_short(char * buffer, const short &s);
    void write_int(char * buffer, const int &i);
    void write_long(char * buffer, const long &l);
    void write_float(char * buffer, const float &f);
    void write_double(char * buffer, const double &d);
    void write_UTF(char * buffer, const std::string &str);

    char read_char(char * buffer);
    short read_short(char * buffer);
    int read_int(char * buffer);
    long read_long(char * buffer);
    float read_float(char * buffer);
    double read_double(char * buffer);
    std::string read_UTF(char * buffer);
}

namespace datafile
{
    void write_char(std::ostream &os, const char &c);
    void write_short(std::ostream &os, const short &s);
    void write_int(std::ostream &os, const int &i);
    void write_long(std::ostream &os, const long &l);
    void write_float(std::ostream &os, const float &f);
    void write_double(std::ostream &os, const double &d);
    void write_UTF(std::ostream &os, const std::string &str);
    
    char read_char(std::istream &is);
    short read_short(std::istream &is);
    int read_int(std::istream &is);
    long read_long(std::istream &is);
    float read_float(std::istream &is);
    double read_double(std::istream &is);
    std::string read_UTF(std::istream &is);

    size_t size(std::fstream &is);
}
#endif //_TOOLS_HPP_
