#include "tree.hpp"
#include "tools.hpp"

#include <cstring>

#ifdef DEBUG
#include "debugtools.hpp"
#endif

// tree::b - begin

tree::b::b(const u_int &order, const std::string &file_name) :
_order(order),
_file_name(file_name)
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
    if(size < 8)
        datafile::write_long(_file,0);
}

tree::b::~b()
{
    _file.close();
}

bool tree::b::is_empty()
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return !root;
}

u_long tree::b::search(const u_int &key)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return search(key,root);
}

u_long tree::b::search(const u_int &key, const u_long &page)
{
    if(!page)
        return 0;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key > pa._key[i])
        ++i;

    if(i < pa._n && key == pa._key[i])
        return pa._address[i];
    else if(i < pa._n && key > pa._key[i])
        return search(key,pa._child[i+1]);
    else
        return search(key,pa._child[i]);
}

void tree::b::insert(const u_int &key, const u_long &address)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);
    
    u_int extra_key = key;
    u_long extra_address = address;
    u_long extra_page = page;

    bool grow = insert(page,extra_page,extra_key,extra_address);

    if(grow)
    {
        tree::b::page np(_order);
        np._n = 1;
        np._key[0] = extra_key;
        np._address[0] = extra_address;
        np._child[0] = page;
        np._child[1] = extra_page;

        char * buffer;

        _file.seekg(0,_file.end);
        u_long root = _file.tellg();
        buffer = new char[np._size];
        np.read(buffer);
        _file.write(buffer,np._size);
        delete[] buffer;

        _file.seekg(0,_file.beg);
        datafile::write_long(_file,root);
    }
}

bool tree::b::insert(const u_long &page, u_long &extra_page, 
    u_int &extra_key, u_long &extra_address)
{
    if(!page)
    {
        extra_page = 0;
        return true;
    }

    char * buffer;
    
    _file.seekg(page,_file.beg);
    tree::b::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && extra_key > pa._key[i])
        ++i;

    bool grow;

    if(i < pa._n && extra_key == pa._key[i])
        return false;
    else if(i < pa._n && extra_key > pa._key[i])
        grow = insert(pa._child[i+1],extra_page,extra_key,extra_address);
    else
        grow = insert(pa._child[i],extra_page,extra_key,extra_address);

    if(!grow)
        return grow;
    
    if(pa._n < _order*2)
    {
        for(u_int j = pa._n; j > i; --j)
        {
            pa._key[j] = pa._key[j-1];
            pa._address[j] = pa._address[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key[i] = extra_key;
        pa._address[i] = extra_address;
        pa._child[i+1] = extra_page;
        ++pa._n;

        buffer = new char[pa._size];
        _file.seekg(page,_file.beg);
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;
        
        return false;
    }

    tree::b::page np(_order);

    for(u_int j = 0; j < _order; ++j)
    {
        np._key[j] = pa._key[j+_order];
        np._address[j] = pa._address[j+_order];
        np._child[j+1] = pa._child[j+_order+1];
    }
    
    np._child[0] = pa._child[_order];
    np._n = _order;
    pa._n = _order;

    if(i < _order)
    {
        u_int aux_key = pa._key[_order-1];
        u_long aux_address = pa._address[_order-1];

        for(u_int j = _order; j > 0 && j > i; --j)
        {
            pa._key[j] = pa._key[j-1];
            pa._address[j] = pa._address[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key[i] = extra_key;
        pa._address[i] = extra_address;
        pa._child[i+1] = extra_page;

        extra_key = aux_key;
        extra_address = aux_address;
    }
    else if(i > _order)
    {
        u_int aux_key = np._key[0];
        u_long aux_address = np._address[0];
        
        u_int j;
        for(j = 0; j < _order-1 && np._key[j+1] < extra_key; ++j)
        {
            np._key[j] = np._key[j+1];
            np._address[j] = np._address[j+1];
            np._child[j] = np._child[j+1];
        }

        np._child[j] = np._child[j+1];

        np._key[j] = extra_key;
        np._address[j] = extra_address;
        np._child[j+1] = extra_page;

        extra_key = aux_key;
        extra_address = aux_address;
    }
    else
    {
        np._child[0] = extra_page;
    }

    _file.seekg(page,_file.beg);
    buffer = new char[pa._size];
    pa.read(buffer);
    _file.write(buffer,pa._size);
    delete[] buffer;
    
    _file.seekg(0,_file.end);
    extra_page = _file.tellg();
   
    buffer = new char[np._size];
    np.read(buffer);
    _file.write(buffer,np._size);
    delete[] buffer;

    return grow;
}

bool tree::b::update(const u_int &key, const u_long &address)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);

    return update(key,address,root);
}

bool tree::b::update(const u_int &key, 
    const u_long &address, const u_long &page)
{
    if(!page)
        return false;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    
    u_int i = 0;
    while(i < pa._n && key > pa._key[i])
        ++i;

    if(i < pa._n && key == pa._key[i])
    {
        pa._address[i] = address;
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;
        return true;
    }
    else if(i < pa._n && key > pa._key[i])
        return update(key,address,pa._child[i+1]);
    else
        return update(key,address,pa._child[i]);
}

bool tree::b::remove(const u_int &key)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);

    bool shrink = false;
    bool removed = remove(key,page,shrink);

    if(removed && shrink)
    {
        char * buffer;
        
        _file.seekg(page,_file.beg);
        tree::b::page pa(_order);
        
        buffer = new char[pa._size];
        _file.read(buffer,pa._size);
        pa.write(buffer);
        delete[] buffer;

        if(!pa._n)
        {
            _file.seekg(0,_file.beg);
            datafile::write_long(_file,pa._child[0]);
        }
    }
    return removed;
}

bool tree::b::remove(const u_int &key, const u_long &page, 
    bool &shrink)
{
    bool removed = false;
    u_int shrinked;

    if(!page)
    {
        shrink = false;
        return false;
    }

    char * buffer;

    _file.seekg(page,_file.beg);

    tree::b::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key > pa._key[i]) 
        ++i;

    if(i < pa._n && key == pa._key[i])
    {
        if(!pa._child[i])
        {
            for(u_int j = i; j < pa._n-1; ++j)
            {
                pa._key[j] = pa._key[j+1];
                pa._address[j] = pa._address[j+1];
            }

            --pa._n;
            _file.seekg(page,_file.beg);
            buffer = new char[pa._size];
            pa.read(buffer);
            _file.write(buffer,pa._size);
            delete[] buffer;

            shrink = pa._n < _order;
            return true;
        }
        else
        {
            u_long aux_page = pa._child[i];
            tree::b::page pa2(_order);
            
            while(aux_page)
            {
                _file.seekg(aux_page,_file.beg);
                buffer = new char[pa2._size];
                _file.read(buffer,pa2._size);
                pa2.write(buffer);
                aux_page = pa2._child[pa2._n];
                delete[] buffer;
            }

            u_int last_key = pa2._key[pa2._n-1];
            u_long last_address = pa2._address[pa2._n-1];

            pa._key[i] = last_key;
            pa._address[i] = last_address;

            buffer = new char[pa._size];
            _file.seekg(page,_file.beg);
            pa.read(buffer);
            _file.write(buffer,pa._size);
            delete[] buffer;

            removed = remove(last_key, pa._child[i], shrink);
            shrinked = i;
        }
    }
    else if(i < pa._n && key > pa._key[i])
    {
        removed = remove(key, pa._child[i+1], shrink);
        shrinked = i+1;
    }
    else
    {
        removed = remove(key, pa._child[i], shrink);
        shrinked = i;
    }
    
    if(shrink)
    {
        u_long child_page = pa._child[shrinked];
        tree::b::page pc(_order);
        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        _file.read(buffer,pc._size);
        pc.write(buffer);
        delete[] buffer;

        u_long brother_page;
        tree::b::page pb(_order);

        if(shrinked)
        {
            brother_page = pa._child[shrinked-1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            if(pb._n > _order)
            {
                for(u_int j = pc._n; j > 0; --j)
                {
                    pc._key[j] = pc._key[j-1];
                    pc._address[j] = pc._address[j-1];
                    pc._child[j+1] = pc._child[j];
                }
                pc._child[1] = pc._child[0];
                ++pc._n;

                pc._key[0] = pa._key[shrinked-1];
                pc._address[0] = pa._address[shrinked-1];

                pa._key[shrinked-1] = pb._key[pb._n-1];
                pa._address[shrinked-1] = pb._address[pb._n-1];

                pc._child[0] = pb._child[pb._n];
                --pb._n;
                shrink = false;
            }
            else
            {
                pb._key[pb._n] = pa._key[shrinked-1];
                pb._address[pb._n] = pa._address[shrinked-1];
                pb._child[pb._n+1] = pc._child[0];
                ++pb._n;

                for(u_int j = 0; j < pc._n; ++j)
                {
                    pb._key[pb._n] = pc._key[j];
                    pb._address[pb._n] = pc._address[j];
                    pb._child[pb._n+1] = pc._child[j+1];
                    ++pb._n;
                }

                pc._n = 0;

                for(u_int j = shrinked-1; j< pa._n; ++j)
                {
                    pa._key[j] = pa._key[j+1];
                    pa._address[j] = pa._address[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        else
        {
            brother_page = pa._child[shrinked+1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            pc._key[pc._n] = pa._key[shrinked];
            pc._address[pc._n] = pa._address[shrinked];
            pc._child[pc._n+1] = pb._child[0];
            ++pc._n;
                
            if(pb._n > _order)
            {
                pa._key[shrinked] = pb._key[0];
                pa._address[shrinked] = pb._address[0];

                for(u_int j = 0; j < pb._n-1; ++j)
                {
                    pb._key[j] = pb._key[j+1];
                    pb._address[j] = pb._address[j+1];
                    pb._child[j] = pb._child[j+1];
                }
                pb._child[pb._n-1] = pb._child[pb._n];
                --pb._n;
                shrink = false;
            }
            else
            {
                for(u_int j = 0; j < pb._n; ++j)
                {
                    pc._key[pc._n] = pb._key[j];
                    pc._address[pc._n] = pb._address[j];
                    pc._child[pc._n+1] = pb._child[j+1];
                    ++pc._n;
                }

                pb._n = 0;

                for(u_int j = shrinked; j < pa._n-1; ++j)
                {
                    pa._key[j] = pa._key[j+1];
                    pa._address[j] = pa._address[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;

        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        pc.read(buffer);
        _file.write(buffer,pc._size);
        delete[] buffer;

        _file.seekg(brother_page,_file.beg);
        buffer = new char[pb._size];
        pb.read(buffer);
        _file.write(buffer,pb._size);
        delete[] buffer;
    }
    return removed;
}

void tree::b::print(std::ostream &os)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    print(os,root);
}

void tree::b::print(std::ostream &os, const u_long &page, 
    u_int level, u_int data)
{
    if(!page)
        return;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b::page pa(_order);
    
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    os << level << "." << data << ": (";

    data = level*data*_order*2;
    ++level;
    
    for(u_int i = 0; i < pa._n; ++i)
    {
        if(pa._child[i])
            os << level << "." << data + i;
        os << ") " << pa._key[i] << " (";
    }

    if(pa._child[pa._n])
        os << level << "." << data + pa._n;

    os << ")" << std::endl;

    for(u_int i = 0; i <= pa._n; ++i)
        print(os,pa._child[i],level,data+i);
}

// tree::b::page - begin

tree::b::page::page(const u_int &order) : 
_n(0),
_key(order*2,0),
_address(order*2,0),
_child(order*2 + 1,0),
_order(order), 
_size(4 + (order*2)*4 + (order*2)*8 + (order*2+1)*8)
{}

void tree::b::page::write(char * buffer)
{
    _n = datastream::read_int(buffer);
    buffer += 4;
    for(u_int i = 0; i < _n; ++i)
    {
        _child[i] = datastream::read_long(buffer);
        _key[i] = datastream::read_int(buffer+8);
        _address[i] = datastream::read_long(buffer+12);
        buffer += 20;
    }
    _child[_n] = datastream::read_long(buffer);
}

void tree::b::page::read(char * buffer)
{
    datastream::write_int(buffer,_n);
    buffer += 4;
    for(u_int i = 0; i < _order*2; ++i)
    {
        datastream::write_long(buffer,_child[i]);
        datastream::write_int(buffer+8,_key[i]);
        datastream::write_long(buffer+12,_address[i]);
        buffer += 20;
    }
    datastream::write_long(buffer,_child[_order*2]);
}

// tree::b::page - end
// tree::b - end

// tree::b_string - begin

tree::b_string::b_string(const u_int &order, 
    const u_int &key_size, const std::string &file_name) :
_order(order),
_key_size(key_size),
_file_name(file_name)
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
    if(size < 8)
        datafile::write_long(_file,0);
}

tree::b_string::~b_string()
{
    _file.close();
}

bool tree::b_string::is_empty()
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return !root;
}

u_int tree::b_string::search(const std::string &key)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return search(key,root);
}

u_int tree::b_string::search(const std::string &key, const u_long &page)
{
    if(!page)
        return 0;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b_string::page pa(_order,_key_size);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key.compare(pa._key[i]) > 0)
        ++i;

    if(i < pa._n && key.compare(pa._key[i]) == 0)
        return pa._data[i];
    else if(i < pa._n && key.compare(pa._key[i]) > 0)
        return search(key,pa._child[i+1]);
    else
        return search(key,pa._child[i]);
}

void tree::b_string::insert(const std::string &key, const u_int &data)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);
    
    std::string extra_key = key;
    u_int extra_data = data;
    u_long extra_page = page;

    bool grow = insert(page,extra_page,extra_key,extra_data);

    if(grow)
    {
        tree::b_string::page np(_order,_key_size);
        np._n = 1;
        np._key[0] = extra_key;
        np._data[0] = extra_data;
        np._child[0] = page;
        np._child[1] = extra_page;

        char * buffer;

        _file.seekg(0,_file.end);
        u_long root = _file.tellg();
        buffer = new char[np._size];
        np.read(buffer);
        _file.write(buffer,np._size);
        delete[] buffer;

        _file.seekg(0,_file.beg);
        datafile::write_long(_file,root);
    }
}

bool tree::b_string::insert(const u_long &page, u_long &extra_page, 
    std::string &extra_key, u_int &extra_data)
{
    if(!page)
    {
        extra_page = 0;
        return true;
    }

    char * buffer;
    
    _file.seekg(page,_file.beg);
    tree::b_string::page pa(_order,_key_size);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && extra_key.compare(pa._key[i]) > 0)
        ++i;

    bool grow;

    if(i < pa._n && extra_key.compare(pa._key[i]) == 0)
        return false;
    else if(i < pa._n && extra_key.compare(pa._key[i]) > 0)
        grow = insert(pa._child[i+1],extra_page,extra_key,extra_data);
    else
        grow = insert(pa._child[i],extra_page,extra_key,extra_data);

    if(!grow)
        return grow;
    
    if(pa._n < _order*2)
    {
        for(u_int j = pa._n; j > i; --j)
        {
            pa._key[j] = pa._key[j-1];
            pa._data[j] = pa._data[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key[i] = extra_key;
        pa._data[i] = extra_data;
        pa._child[i+1] = extra_page;
        ++pa._n;

        buffer = new char[pa._size];
        _file.seekg(page,_file.beg);
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;
        
        return false;
    }

    tree::b_string::page np(_order,_key_size);

    for(u_int j = 0; j < _order; ++j)
    {
        np._key[j] = pa._key[j+_order];
        np._data[j] = pa._data[j+_order];
        np._child[j+1] = pa._child[j+_order+1];
    }
    
    np._child[0] = pa._child[_order];
    np._n = _order;
    pa._n = _order;

    if(i < _order)
    {
        std::string aux_key = pa._key[_order-1];
        u_int aux_data = pa._data[_order-1];

        for(u_int j = _order; j > 0 && j > i; --j)
        {
            pa._key[j] = pa._key[j-1];
            pa._data[j] = pa._data[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key[i] = extra_key;
        pa._data[i] = extra_data;
        pa._child[i+1] = extra_page;

        extra_key = aux_key;
        extra_data = aux_data;
    }
    else if(i > _order)
    {
        std::string aux_key = np._key[0];
        u_int aux_data = np._data[0];
        
        u_int j;
        for(j = 0; j < _order-1 && np._key[j+1].compare(extra_key) < 0; ++j)
        {
            np._key[j] = np._key[j+1];
            np._data[j] = np._data[j+1];
            np._child[j] = np._child[j+1];
        }

        np._child[j] = np._child[j+1];

        np._key[j] = extra_key;
        np._data[j] = extra_data;
        np._child[j+1] = extra_page;

        extra_key = aux_key;
        extra_data = aux_data;
    }
    else
    {
        np._child[0] = extra_page;
    }

    _file.seekg(page,_file.beg);
    buffer = new char[pa._size];
    pa.read(buffer);
    _file.write(buffer,pa._size);
    delete[] buffer;
    
    _file.seekg(0,_file.end);
    extra_page = _file.tellg();
   
    buffer = new char[np._size];
    np.read(buffer);
    _file.write(buffer,np._size);
    delete[] buffer;

    return grow;
}

bool tree::b_string::update(const std::string &key, const u_int &data)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return update(key,data,root);
}

bool tree::b_string::update(const std::string &key, 
    const u_int &data, const u_long &page)
{
    if(!page)
        return false;
    
    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b_string::page pa(_order,_key_size);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key.compare(pa._key[i]) > 0)
        ++i;

    if(i < pa._n && key.compare(pa._key[i]) == 0)
    {
        pa._data[i] = data;
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;
        return true;
    }
    else if(i < pa._n && key.compare(pa._key[i]) > 0)
        return update(key,data,pa._child[i+1]);
    else
        return update(key,data,pa._child[i]);
}

bool tree::b_string::remove(const std::string &key)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);

    bool shrink = false;
    bool removed = remove(key,page,shrink);

    if(removed && shrink)
    {
        char * buffer;
        
        _file.seekg(page,_file.beg);
        tree::b_string::page pa(_order,_key_size);
        
        buffer = new char[pa._size];
        _file.read(buffer,pa._size);
        pa.write(buffer);
        delete[] buffer;

        if(!pa._n)
        {
            _file.seekg(0,_file.beg);
            datafile::write_long(_file,pa._child[0]);
        }
    }
    return removed;
}

bool tree::b_string::remove(const std::string &key, const u_long &page, 
    bool &shrink)
{
    bool removed = false;
    u_int shrinked;

    if(!page)
    {
        shrink = false;
        return false;
    }

    char * buffer;

    _file.seekg(page,_file.beg);

    tree::b_string::page pa(_order,_key_size);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key.compare(pa._key[i]) > 0) 
        ++i;

    if(i < pa._n && key.compare(pa._key[i]) == 0)
    {
        if(!pa._child[i])
        {
            for(u_int j = i; j < pa._n-1; ++j)
            {
                pa._key[j] = pa._key[j+1];
                pa._data[j] = pa._data[j+1];
            }

            --pa._n;
            _file.seekg(page,_file.beg);
            buffer = new char[pa._size];
            pa.read(buffer);
            _file.write(buffer,pa._size);
            delete[] buffer;

            shrink = pa._n < _order;
            return true;
        }
        else
        {
            u_long aux_page = pa._child[i];
            tree::b_string::page pa2(_order,_key_size);
            
            while(aux_page)
            {
                _file.seekg(aux_page,_file.beg);
                buffer = new char[pa2._size];
                _file.read(buffer,pa2._size);
                pa2.write(buffer);
                aux_page = pa2._child[pa2._n];
                delete[] buffer;
            }

            std::string last_key = pa2._key[pa2._n-1];
            u_long last_data = pa2._data[pa2._n-1];

            pa._key[i] = last_key;
            pa._data[i] = last_data;

            buffer = new char[pa._size];
            _file.seekg(page,_file.beg);
            pa.read(buffer);
            _file.write(buffer,pa._size);
            delete[] buffer;

            removed = remove(last_key, pa._child[i], shrink);
            shrinked = i;
        }
    }
    else if(i < pa._n && key.compare(pa._key[i]) > 0)
    {
        removed = remove(key, pa._child[i+1], shrink);
        shrinked = i+1;
    }
    else
    {
        removed = remove(key, pa._child[i], shrink);
        shrinked = i;
    }
    
    if(shrink)
    {
        u_long child_page = pa._child[shrinked];
        tree::b_string::page pc(_order,_key_size);
        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        _file.read(buffer,pc._size);
        pc.write(buffer);
        delete[] buffer;

        u_long brother_page;
        tree::b_string::page pb(_order,_key_size);

        if(shrinked)
        {
            brother_page = pa._child[shrinked-1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            if(pb._n > _order)
            {
                for(u_int j = pc._n; j > 0; --j)
                {
                    pc._key[j] = pc._key[j-1];
                    pc._data[j] = pc._data[j-1];
                    pc._child[j+1] = pc._child[j];
                }
                pc._child[1] = pc._child[0];
                ++pc._n;

                pc._key[0] = pa._key[shrinked-1];
                pc._data[0] = pa._data[shrinked-1];

                pa._key[shrinked-1] = pb._key[pb._n-1];
                pa._data[shrinked-1] = pb._data[pb._n-1];

                pc._child[0] = pb._child[pb._n];
                --pb._n;
                shrink = false;
            }
            else
            {
                pb._key[pb._n] = pa._key[shrinked-1];
                pb._data[pb._n] = pa._data[shrinked-1];
                pb._child[pb._n+1] = pc._child[0];
                ++pb._n;

                for(u_int j = 0; j < pc._n; ++j)
                {
                    pb._key[pb._n] = pc._key[j];
                    pb._data[pb._n] = pc._data[j];
                    pb._child[pb._n+1] = pc._child[j+1];
                    ++pb._n;
                }

                pc._n = 0;

                for(u_int j = shrinked-1; j< pa._n; ++j)
                {
                    pa._key[j] = pa._key[j+1];
                    pa._data[j] = pa._data[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        else
        {
            brother_page = pa._child[shrinked+1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            pc._key[pc._n] = pa._key[shrinked];
            pc._data[pc._n] = pa._data[shrinked];
            pc._child[pc._n+1] = pb._child[0];
            ++pc._n;
                
            if(pb._n > _order)
            {
                pa._key[shrinked] = pb._key[0];
                pa._data[shrinked] = pb._data[0];

                for(u_int j = 0; j < pb._n-1; ++j)
                {
                    pb._key[j] = pb._key[j+1];
                    pb._data[j] = pb._data[j+1];
                    pb._child[j] = pb._child[j+1];
                }
                pb._child[pb._n-1] = pb._child[pb._n];
                --pb._n;
                shrink = false;
            }
            else
            {
                for(u_int j = 0; j < pb._n; ++j)
                {
                    pc._key[pc._n] = pb._key[j];
                    pc._data[pc._n] = pb._data[j];
                    pc._child[pc._n+1] = pb._child[j+1];
                    ++pc._n;
                }

                pb._n = 0;

                for(u_int j = shrinked; j < pa._n-1; ++j)
                {
                    pa._key[j] = pa._key[j+1];
                    pa._data[j] = pa._data[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;

        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        pc.read(buffer);
        _file.write(buffer,pc._size);
        delete[] buffer;

        _file.seekg(brother_page,_file.beg);
        buffer = new char[pb._size];
        pb.read(buffer);
        _file.write(buffer,pb._size);
        delete[] buffer;
    }
    return removed;
}


void tree::b_string::print(std::ostream &os)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    print(os,root);
}

void tree::b_string::print(std::ostream &os, const u_long &page, 
    u_int level, u_int data)
{
    if(!page)
        return;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b_string::page pa(_order,_key_size);
    
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    os << level << "." << data << ": (";

    data = level*data*_order*2;
    ++level;
    
    for(u_int i = 0; i < pa._n; ++i)
    {
        if(pa._child[i])
            os << level << "." << data + i;
        os << ") " << pa._key[i] << " (";
    }

    if(pa._child[pa._n])
        os << level << "." << data + pa._n;

    os << ")" << std::endl;

    for(u_int i = 0; i <= pa._n; ++i)
        print(os,pa._child[i],level,data+i);
}


// tree::b_string::page - begin

tree::b_string::page::page(const u_int &order, const u_int &key_size) : 
_n(0),
_key(order*2,""),
_data(order*2,0),
_child(order*2 + 1,0),
_order(order), 
_key_size(key_size),
_size(4 + (order*2)*key_size + (order*2)*4 + (order*2+1)*8)
{}

void tree::b_string::page::read(char * buffer)
{
    datastream::write_int(buffer,_n);
    buffer += 4;
    for(u_int i = 0; i < _order*2; ++i)
    {
        datastream::write_long(buffer,_child[i]);
        strncpy(buffer+8, _key[i].c_str(),_key_size);
        datastream::write_int(buffer+_key_size+8,_data[i]);
        buffer += (12 + _key_size);
    }
    datastream::write_long(buffer,_child[_order*2]);
}

void tree::b_string::page::write(char * buffer)
{
    _n = datastream::read_int(buffer);
    buffer += 4;
    for(u_int i = 0; i < _n; ++i)
    {
        _child[i] = datastream::read_long(buffer);
        _key[i] = std::string(buffer+8);
        _data[i] = datastream::read_int(buffer+8+_key_size);
        buffer += (12 + _key_size);
    }
    _child[_n] = datastream::read_long(buffer);
}

// tree::b_string::page - end
// tree::b_string - end

// tree::b_plus - begin

tree::b_plus::b_plus(const u_int &order, const std::string &file_name) :
_order(order),
_file_name(file_name)
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
    if(size < 8)
        datafile::write_long(_file,0);
}

tree::b_plus::~b_plus()
{
    _file.close();
}

bool tree::b_plus::is_empty()
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    return !root;
}

std::list<u_int>* tree::b_plus::list(const u_int &key_a)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
   if(root) return list(key_a,root);
   else return new std::list<u_int>();
}

std::list<u_int>* tree::b_plus::list(const u_int &key_a, const u_long &page)
{
    if(!page) return new std::list<u_int>();
    
    char * buffer;

    tree::b_plus::page pa(_order);

    buffer = new char[pa._size];
    _file.seekg(page, _file.beg);
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && key_a > pa._key_a[i]) ++i;

    if(i < pa._n && !pa._child[0] && key_a == pa._key_a[i])
    {
        std::list<u_int> * r_list = new std::list<u_int>();
        while(key_a == pa._key_a[i])
        {
            r_list->push_back(pa._key_b[i]);
            ++i;
            if(i == pa._n)
            {
                if(!pa._child[_order*2]) 
                    break;

                buffer = new char[pa._size];
                _file.seekg(pa._child[_order*2]);
                _file.read(buffer,pa._size);
                pa.write(buffer);
                delete[] buffer;

                i = 0;
            }
        }
        return r_list;
    }
    else if(i == pa._n && !pa._child[0])
    {
        if(!pa._child[_order*2]) 
            return new std::list<u_int>();

        buffer = new char[pa._size];
        _file.seekg(pa._child[_order*2],_file.beg);
        _file.read(buffer,pa._size);
        pa.write(buffer);
        delete[] buffer;

        i = 0;
        if(key_a == pa._key_a[0])
        {
            std::list<u_int> * r_list = new std::list<u_int>();
            while(key_a == pa._key_a[i])
            {
                r_list->push_back(pa._key_b[i]);
                ++i;
                if(i == pa._n)
                {
                    if(!pa._child[_order*2]) 
                        break;

                    buffer = new char[pa._size];
                    _file.seekg(pa._child[_order*2]);
                    _file.read(buffer,pa._size);
                    pa.write(buffer);
                    delete[] buffer;

                    i = 0;
                }
            }
            return r_list;
        }
        else
        {
            return new std::list<u_int>();
        }
    }

    if(i == pa._n || key_a <= pa._key_a[i])
        return list(key_a, pa._child[i]);
    else
        return list(key_a, pa._child[i+1]);
}

void tree::b_plus::insert(const u_int &key_a, const u_int &key_b)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);

    u_long extra_page = 0;
    u_int extra_key_a = key_a;
    u_int extra_key_b = key_b;
    
    bool grow = insert(page, extra_page, extra_key_a, extra_key_b);
    
    if(grow)
    {
        tree::b_plus::page np(_order);
        np._n = 1;
        np._key_a[0] = extra_key_a;
        np._key_b[0] = extra_key_b;
        np._child[0] = page;
        np._child[1] = extra_page;

        char * buffer;

        _file.seekg(0,_file.end);
        u_long root = _file.tellg();
        buffer = new char[np._size];
        np.read(buffer);
        _file.write(buffer,np._size);
        delete[] buffer;

        _file.seekg(0,_file.beg);
        datafile::write_long(_file,root);
    }
}

bool tree::b_plus::insert(const u_long &page, 
    u_long &extra_page, u_int &extra_key_a, u_int &extra_key_b)
{
    if(!page)
    {
        extra_page = 0;
        return true;
    }

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b_plus::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && (extra_key_a > pa._key_a[i] || 
        (extra_key_a == pa._key_a[i] && extra_key_b > pa._key_b[i])))
        ++i;
    
    if(i < pa._n && !pa._child[0] && 
        extra_key_a == pa._key_a[i] && extra_key_b == pa._key_b[i])
        return false;
    
    bool grow;
    if(i == pa._n || extra_key_a < pa._key_a[i] || 
        (extra_key_a == pa._key_a[i] && extra_key_b <= pa._key_b[i]))
        grow = insert(pa._child[i],extra_page,extra_key_a,extra_key_b);
    else
        grow = insert(pa._child[i+1],extra_page,extra_key_a,extra_key_b);
    
    if(!grow) 
        return grow;

    if(pa._n < _order*2)
    {
        u_long next_page = 0;

        if(!pa._child[0] && pa._child[_order*2])
            next_page = pa._child[_order*2];

        for(u_int j = pa._n; j > i; --j)
        {
            pa._key_a[j] = pa._key_a[j-1];
            pa._key_b[j] = pa._key_b[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key_a[i] = extra_key_a;
        pa._key_b[i] = extra_key_b;
        pa._child[i+1] = extra_page;

        ++pa._n;

        if(next_page)
            pa._child[_order*2] = next_page;
        
        buffer = new char[pa._size];
        _file.seekg(page,_file.beg);
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;
        
        return false;
    }
    
    tree::b_plus::page np(_order);

    for(u_int j = 0; j < _order; ++j)
    {
        np._key_a[j] = pa._key_a[j+_order];
        np._key_b[j] = pa._key_b[j+_order];
        np._child[j+1] = pa._child[j+_order+1];
    }

    np._child[0] = pa._child[_order];
    np._n = _order;
    pa._n = _order;

    if(i <= _order)
    {
        for(u_int j = _order; j > 0 && j > i; --j)
        {
            pa._key_a[j] = pa._key_a[j-1];
            pa._key_b[j] = pa._key_b[j-1];
            pa._child[j+1] = pa._child[j];
        }

        pa._key_a[i] = extra_key_a;
        pa._key_b[i] = extra_key_b;
        pa._child[i+1] = extra_page;

        ++pa._n;
    }
    else
    {
        u_int j;
        for(j = _order; j > 0 && (extra_key_a < np._key_a[j-1] ||
            (extra_key_a == np._key_a[j-1] && extra_key_b < np._key_b[j-1]))
            ; --j)
        {
            np._key_a[j] = np._key_a[j-1];
            np._key_b[j] = np._key_b[j-1];
            np._child[j+1] = np._child[j];
        }
        np._key_a[j] = extra_key_a;
        np._key_b[j] = extra_key_b;
        np._child[j+1] = extra_page;
        ++np._n;
    }
    
    extra_key_a = pa._key_a[pa._n-1];
    extra_key_b = pa._key_b[pa._n-1];
    
    _file.seekg(0,_file.end);
    extra_page = _file.tellg();

    buffer = new char[np._size];
    np.read(buffer);
    _file.write(buffer,np._size);
    delete[] buffer;

    if(!pa._child[0])
        pa._child[_order*2] = extra_page;

    _file.seekg(page,_file.beg);
    
    buffer = new char[pa._size];
    pa.read(buffer);
    _file.write(buffer,pa._size);
    delete[] buffer;

    return grow;
}

bool tree::b_plus::remove(const u_int &key_a, const u_int &key_b)
{
    _file.seekg(0,_file.beg);
    u_long page = datafile::read_long(_file);

    bool shrink = false;
    bool removed = remove(key_a,key_b,page,shrink);

    if(removed && shrink)
    {
        char * buffer;
        
        _file.seekg(page,_file.beg);
        tree::b_plus::page pa(_order);
        
        buffer = new char[pa._size];
        _file.read(buffer,pa._size);
        pa.write(buffer);
        delete[] buffer;

        if(!pa._n)
        {
            _file.seekg(0,_file.beg);
            datafile::write_long(_file,pa._child[0]);
        }
    }
    return removed;
}

bool tree::b_plus::remove(const u_int &key_a, const u_int &key_b, 
    const u_long &page, bool &shrink)
{
    bool removed = false;
    u_int shrinked;

    if(!page)
    {
        shrink = false;
        return false;
    }

    char * buffer;

    _file.seekg(page,_file.beg);

    tree::b_plus::page pa(_order);
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    u_int i = 0;
    while(i < pa._n && (key_a > pa._key_a[i] 
        || (key_a == pa._key_a[i] && key_b > pa._key_b[i])))
        ++i;
    
    if(i < pa._n && !pa._child[0] 
        && key_a == pa._key_a[i] && key_b == pa._key_b[i])
    {
        for(u_int j = i; j < pa._n-1; ++j)
        {
            pa._key_a[j] = pa._key_a[j+1];
            pa._key_b[j] = pa._key_b[j+1];
        }

        --pa._n;
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;

        shrink = pa._n < _order;
        return true;
    }

    if(i == pa._n || key_a < pa._key_a[i] 
        || (key_a == pa._key_a[i] && key_b <= pa._key_b[i]))
    {
        removed = remove(key_a,key_b,pa._child[i], shrink);
        shrinked = i;
    }
    else
    {
        removed = remove(key_a,key_b,pa._child[i+1], shrink);
        shrinked = i+1;
    }

    if(shrink)
    {
        u_long child_page = pa._child[shrinked];
        tree::b_plus::page pc(_order);

        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        _file.read(buffer,pc._size);
        pc.write(buffer);
        delete[] buffer;

        u_long brother_page;
        tree::b_plus::page pb(_order);

        if(shrinked)
        {
            brother_page = pa._child[shrinked-1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            if(pb._n > _order)
            {
                for(u_int j = pc._n; j > 0; --j)
                {
                    pc._key_a[j] = pc._key_a[j-1];
                    pc._key_b[j] = pc._key_b[j-1];
                    pc._child[j+1] = pc._child[j];
                }
                pc._child[1] = pc._child[0];
                ++pc._n;

                pc._key_a[0] = pb._key_a[pb._n-1];
                pc._key_b[0] = pb._key_b[pb._n-1];

                pc._child[0] = pb._child[pb._n];
                --pb._n;

                pa._key_a[shrinked-1] = pb._key_a[pb._n-1];
                pa._key_b[shrinked-1] = pb._key_b[pb._n-1];

                shrink = false;
            }
            else
            {
                for(u_int j = 0; j < pc._n; ++j)
                {
                    pb._key_a[pb._n] = pc._key_a[j];
                    pb._key_b[pb._n] = pc._key_b[j];
                    pb._child[pb._n+1] = pc._child[j+1];
                    ++pb._n;
                }

                pc._n = 0;

                for(u_int j = shrinked-1; j< pa._n; ++j)
                {
                    pa._key_a[j] = pa._key_a[j+1];
                    pa._key_b[j] = pa._key_b[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        else
        {
            brother_page = pa._child[shrinked+1];
            _file.seekg(brother_page,_file.beg);

            buffer = new char[pb._size];
            _file.read(buffer,pb._size);
            pb.write(buffer);
            delete[] buffer;

            if(pb._n > _order)
            {
                pc._key_a[pc._n] = pb._key_a[0];
                pc._key_b[pc._n] = pb._key_b[0];
                pc._child[pc._n+1] = pb._child[0];
                ++pc._n;

                pa._key_a[shrinked] = pb._key_a[0];
                pa._key_b[shrinked] = pb._key_b[0];

                for(u_int j = 0; j < pb._n-1; ++j)
                {
                    pb._key_a[j] = pb._key_a[j+1];
                    pb._key_b[j] = pb._key_b[j+1];
                    pb._child[j] = pb._child[j+1];
                }
                pb._child[pb._n-1] = pb._child[pb._n];
                --pb._n;
                shrink = false;
            }
            else
            {
                for(u_int j = 0; j < pb._n; ++j)
                {
                    pc._key_a[pc._n] = pb._key_a[j];
                    pc._key_b[pc._n] = pb._key_b[j];
                    pc._child[pc._n+1] = pb._child[j+1];
                    ++pc._n;
                }

                pb._n = 0;

                for(u_int j = shrinked; j < pa._n-1; ++j)
                {
                    pa._key_a[j] = pa._key_a[j+1];
                    pa._key_b[j] = pa._key_b[j+1];
                    pa._child[j+1] = pa._child[j+2];
                }
                --pa._n;
                shrink = pa._n < _order;
            }
        }
        _file.seekg(page,_file.beg);
        buffer = new char[pa._size];
        pa.read(buffer);
        _file.write(buffer,pa._size);
        delete[] buffer;

        _file.seekg(child_page,_file.beg);
        buffer = new char[pc._size];
        pc.read(buffer);
        _file.write(buffer,pc._size);
        delete[] buffer;

        _file.seekg(brother_page,_file.beg);
        buffer = new char[pb._size];
        pb.read(buffer);
        _file.write(buffer,pb._size);
        delete[] buffer;
    }
    return removed;
}

void tree::b_plus::print(std::ostream &os)
{
    _file.seekg(0,_file.beg);
    u_long root = datafile::read_long(_file);
    print(os,root);
}

void tree::b_plus::print(std::ostream &os, const u_long &page, 
    u_int level, u_int data)
{
    if(!page)
        return;

    char * buffer;

    _file.seekg(page,_file.beg);
    tree::b_plus::page pa(_order);
    
    buffer = new char[pa._size];
    _file.read(buffer,pa._size);
    pa.write(buffer);
    delete[] buffer;

    os << level << "." << data << ": (";

    data = level*data*_order*2;
    ++level;

    u_int i;
    for(i = 0; i < pa._n; ++i)
    {
        if(pa._child[i])
            os << level << "." << data+i;
        os << ") " << pa._key_a[i] << "," << pa._key_b[i] << " (";
    }
    for(; i < _order*2; ++i)
    {
        if(pa._child[i])
            os << level << "." << data+i;
        os << ") - (";
    }
    if(!pa._child[i])
        os << ")" << std::endl;
    else
        os << level << "." << data+i << ")" << std::endl;

    for(i = 0; i <= _order*2; ++i)
        print(os,pa._child[i],level,data+i);

}

// tree::b_plus::page - begin

tree::b_plus::page::page(const u_int &order) : 
_n(0),
_key_a(order*2,0),
_key_b(order*2,0),
_child(order*2 + 1,0),
_order(order), 
_size(4 + (order*2)*4 + (order*2)*4 + (order*2+1)*8)
{}

void tree::b_plus::page::write(char * buffer)
{
    _n = datastream::read_int(buffer);
    buffer += 4;
    for(u_int i = 0; i < _order*2; ++i)
    {
        _child[i] = datastream::read_long(buffer);
        _key_a[i] = datastream::read_int(buffer+8);
        _key_b[i] = datastream::read_int(buffer+12);
        buffer += 16;
    }
    _child[_order*2] = datastream::read_long(buffer);
}

void tree::b_plus::page::read(char * buffer)
{
    datastream::write_int(buffer,_n);
    buffer += 4;
    for(u_int i = 0; i < _n; ++i)
    {
        datastream::write_long(buffer,_child[i]);
        datastream::write_int(buffer+8,_key_a[i]);
        datastream::write_int(buffer+12,_key_b[i]);
        buffer += 16;
    }
    for(u_int i = _n; i < _order*2; ++i)
    {
        datastream::write_long(buffer,_child[i]);
        datastream::write_int(buffer+8,0);
        datastream::write_int(buffer+12,0);
        buffer += 16;
    }
    datastream::write_long(buffer,_child[_order*2]);
}

// tree::b_plus::page - end
// tree::b_plus - end

std::ostream& tree::operator<<(std::ostream &os, tree::b &tree)
{
    tree.print(os);
    return os;
}

std::ostream& tree::operator<<(std::ostream &os, tree::b_string &tree)
{
    tree.print(os);
    return os;
}

std::ostream& tree::operator<<(std::ostream &os, tree::b_plus &tree)
{
    tree.print(os);
    return os;
}
