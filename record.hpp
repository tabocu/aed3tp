#ifndef _RECORD_HPP_
#define _RECORD_HPP_

#include "tools.hpp"
#include "defines.hpp"
#include "tree.hpp"
#include <iostream>


template <class T>
class record
{
public:
    record(const u_int order, const std::string file_name) :
    _file_name(file_name),
    _index(order, file_name + ".index")
    {
        #ifndef PERSISTENT
        _file.open(file_name,
            std::fstream::binary |
            std::fstream::in |
            std::fstream::out);

        if(!_file.is_open())
        #endif
            _file.open(file_name,
                std::fstream::binary |
                std::fstream::in |
                std::fstream::out |
                std::fstream::trunc);

        u_int size = datafile::size(_file);
        if(size < 4)
            datafile::write_int(_file,0);
                
    }
    ~record()
    {
        _file.close();
    }

    u_int insert(T &t)
    {
        _file.seekg(0,_file.beg);
        u_int code = datafile::read_int(_file);
        _file.seekg(0,_file.end);

        ++code;

        t._code = code;

        _file.put(tombstone::alive);
        
        _index.insert(code,_file.tellg());

        datafile::write_short(_file,t.size());

        char * buffer = new char[t.size()];
        t.read(buffer);
        _file.write(buffer,t.size());
        delete[] buffer;

        _file.seekg(0,_file.beg);
        datafile::write_int(_file,code);
        
        return code;
    }

    T search(const u_int &code)
    {
        u_long pos = _index.search(code);

        T t;

        if(!pos) return t;

        _file.seekg(pos,_file.beg);

        u_short size = datafile::read_short(_file);

        char * buffer = new char[size];
        _file.read(buffer,size);
        t.write(buffer);
        delete[] buffer;

        return t;
    }

    bool contain(const u_int &code)
    {
        return _index.search(code);
    }

    bool update(T t)
    {
        u_long pos = _index.search(t._code);

        if(!pos) 
            return false;

        _file.seekg(pos-1,_file.beg);

        _file.put(tombstone::dead);

        _file.seekg(0,_file.end);

        _file.put(tombstone::alive);

        _index.update(t._code,_file.tellg());

        datafile::write_short(_file,t.size());

        char * buffer = new char[t.size()];
        t.read(buffer);
        _file.write(buffer,t.size());
        delete[] buffer;

        return true;
    }

    bool remove(T t)
    {
        u_long pos = _index.search(t._code);

        if(!pos) 
            return false;

        _file.seekg(pos-1,_file.beg);

        _file.put(tombstone::dead);

        return _index.remove(t._code);
    }

    std::list<T>* list()
    {
        std::list<T>* t_vec = new std::list<T>();
        
        u_int size = datafile::size(_file);

        _file.seekg(4, _file.beg);

        char tbt;
        while(_file.tellg() < size)
        {
            tbt = _file.get();

            T t;

            u_short size = datafile::read_short(_file);

            char * buffer = new char[size];
            _file.read(buffer,size);
            t.write(buffer);
            delete[] buffer;

            if(tbt == tombstone::alive)
            {
                t_vec->push_back(t);
            }
        }
        return t_vec;
    }

#ifndef DEBUG
private:
#endif
    const std::string _file_name;
    tree::b _index;
    std::fstream _file;
    
    enum tombstone
    {
        dead = '*',
        alive = ' '
    };
};

#endif
