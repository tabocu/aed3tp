#ifndef _TREE_HPP_
#define _TREE_HPP_

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

#include "defines.hpp"

namespace tree
{
    class b
    {
    public:
        b(const u_int &order, const std::string &file_name);        
        ~b();

        bool is_empty();
        u_long search(const u_int &key);
        void insert(const u_int &key, const u_long &address);
        bool update(const u_int &key, const u_long &address);
        bool remove(const u_int &key);
        void print(std::ostream &os = std::cout);
    private:
        u_long search(const u_int &key, const u_long &page);
        bool insert(const u_long &page, 
            u_long &extra_page, u_int &extra_key, u_long &extra_address);
        bool update(const u_int &key, 
            const u_long &address, const u_long &page);
        bool remove(const u_int &key, const u_long &page, 
            bool &shrink);
        void print(std::ostream &os, const u_long &page, 
            u_int level = 0, u_int data = 0);
        
        const u_int _order;
        const std::string _file_name;
        std::fstream _file;

        class page
        {
        public:
            page(const u_int &order);
            
            void read(char*);
            void write(char*);

            
            u_int _n;
            
            std::vector<u_int> _key;
            std::vector<u_long> _address;
            std::vector<u_long> _child;

            const u_int _order;
            const u_int _size;
        };
    };

    std::ostream& operator<<(std::ostream &os, tree::b &tree);

    class b_string
    {
    public:
        b_string(const u_int &order, const u_int &key_size, 
            const std::string &file_name);
        ~b_string();

        bool is_empty();
        u_int search(const std::string &key);
        void insert(const std::string &key, const u_int &data);
        bool update(const std::string &key, const u_int &data);
        bool remove(const std::string &key);
        void print(std::ostream &os = std::cout);
    private:
        u_int search(const std::string &key, const u_long &page);
        bool insert(const u_long &page, 
            u_long &extra_page, std::string &extra_key, u_int &extra_data);
        bool update(const std::string &key, 
            const u_int &data, const u_long &page);
        bool remove(const std::string &key, const u_long &page, 
            bool &shrink);
        void print(std::ostream &os, const u_long &page, 
            u_int level = 0, u_int data = 0);

        const u_int _order;
        const u_int _key_size;
        const std::string _file_name;
        std::fstream _file;

        class page
        {
        public:
            page(const u_int &order, const u_int &key_size);

            void read(char * buffer);
            void write(char * buffer);

            u_int _n;

            std::vector<std::string> _key;
            std::vector<u_int> _data;
            std::vector<u_long> _child;
            const u_int _order;
            const u_int _key_size;
            const u_int _size;
        };
    };

    std::ostream& operator<<(std::ostream &os, tree::b_string &tree);

    class b_plus
    {
    public:
        b_plus(const u_int &order, const std::string &file_name);
        ~b_plus();

        bool is_empty();
        std::list<u_int>* list(const u_int &key_a);
        void insert(const u_int &key_a, const u_int &key_b);
        bool remove(const u_int &key_a, const u_int &key_b);
        void print(std::ostream &os = std::cout);
    private:
        
        std::list<u_int>* list(const u_int &key_a, const u_long &page);
        bool insert(const u_long &page, 
            u_long &extra_page, u_int &extra_key_a, u_int &extra_key_b);
        bool remove(const u_int &key_a, const u_int &key_b, 
            const u_long &page, bool &shrink);
        void print(std::ostream &os, const u_long &page, 
            u_int level = 0, u_int data = 0);

        const u_int _order;
        const std::string _file_name;
        std::fstream _file;
        
        class page
        {
        public:
            page(const u_int &order);
            
            void read(char* buffer);
            void write(char* buffer);

            u_int _n;

            std::vector<u_int> _key_a;
            std::vector<u_int> _key_b;
            std::vector<u_long> _child;
            const u_int _order;
            const u_int _size;
        };
    };

    std::ostream& operator<<(std::ostream &os, tree::b_plus &tree);
}

#endif //_TREE_HPP_
