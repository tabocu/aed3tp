#ifndef _DEBUGTOOLS_HPP_
#define _DEBUGTOOLS_HPP_

#include <iostream>
#include <vector>
#include <iomanip>

namespace debugtools
{
    void print_buffer(char * buffer, const size_t &size)
    {
        std::clog << "Buffer: ";
        std::clog << std::setbase(16);
        for(size_t i = 0; i < size; ++i)
            std::clog << (u_int)buffer[i];
        std::clog << std::setbase(10);
        std::clog << std::endl;
    }
    
    
    void print_vector_u_int(const std::vector<u_int> &vec)
    {
        std::clog << "Vector:";
        for(std::vector<u_int>::const_iterator it = vec.begin();
            it != vec.end();
            ++it)
            std::clog << " " << *it;
            std::clog << std::endl;
    }

    void break_point(std::string text)
    {
        std::clog << text << std::endl;
        std::string s;
        std::cin >> s;
    }
}

#endif
