#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

#include "defines.hpp"
#include <fstream>
#include <iostream>
#include <set>

namespace manager
{
    
    class entry
    {
    public:
        u_int _code = 0;
        virtual void read(char * bufefr);
        virtual void write(char * buffer);
        virtual u_int size();
        virtual void print(std::ostream &os) const;


    private:

        const u_int _base_size = 4;
    };

    std::ostream& operator<<(std::ostream &os, const manager::entry &entry);

    class project : public entry
    {
    public:
        void read(char * buffer);
        void write(char * buffer);
        u_int size();
        void print(std::ostream &os) const;

        std::string _name;
        std::set<u_int> _partners;
    private:

        const u_int _base_size = 4;
    };

    enum priority
    {
        lowest  = 0,
        low     = 1,
        high    = 2,
        highest = 3
    };

    class task : public entry
    {
    public:
        void read(char * buffer);
        void write(char * buffer);
        u_int size();
        void print(std::ostream &os) const;

        std::string _description;
        u_int _project;
        u_int _partner;
        u_long _dead_line;
        manager::priority _priority;
    private:

        const u_int _base_size = 19;
    };

    class partner : public entry 
    {
    public:
        void read(char * buffer);
        void write(char * buffer);
        u_int size();
        void print(std::ostream &os) const;

        std::string _name;
        std::string _email;
    private:

        const u_int _base_size = 4;
    };
}

#endif 
