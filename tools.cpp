#include "tools.hpp"

#include <string>
#include <cstring>
#include <iostream>

void datastream::write_char(char * buffer, const char &c)
{
    *(buffer + 0) = c;
}

void datastream::write_short(char * buffer, const short &s)
{
    *(buffer + 0) = s >> 8;
    *(buffer + 1) = s >> 0;
}

void datastream::write_int(char * buffer, const int &i)
{
    *(buffer + 0) = (char)(i >> 24);
    *(buffer + 1) = (char)(i >> 16);
    *(buffer + 2) = (char)(i >> 8);
    *(buffer + 3) = (char)(i >> 0);
}

void datastream::write_long(char * buffer, const long &l)
{
    *(buffer + 0) = l >> 56;
    *(buffer + 1) = l >> 48;
    *(buffer + 2) = l >> 40;
    *(buffer + 3) = l >> 32;
    *(buffer + 4) = l >> 24;
    *(buffer + 5) = l >> 16;
    *(buffer + 6) = l >> 8;
    *(buffer + 7) = l >> 0;
}

void datastream::write_float(char * buffer, const float &f)
{
    int fi = *(int*)&f;
    datastream::write_int(buffer,fi);
}

void datastream::write_double(char * buffer, const double &d)
{
    long dl = *(long*)&d;
    datastream::write_long(buffer,dl);
}

void datastream::write_UTF(char * buffer, const std::string &str)
{
    datastream::write_short(buffer,str.size());
    std::memcpy(buffer+2,str.data(),str.size());
}

char datastream::read_char(char * buffer)
{
    return *buffer;
}

short datastream::read_short(char * buffer)
{
    short s0 = *(buffer + 0)&0xff;
    short s1 = *(buffer + 1)&0xff;
    return (s0 << 8) + (s1 << 0);
}

int datastream::read_int(char * buffer)
{
    int i0 = *(buffer + 0)&0xff;
    int i1 = *(buffer + 1)&0xff;
    int i2 = *(buffer + 2)&0xff;
    int i3 = *(buffer + 3)&0xff;
    return (i0 << 24) + (i1 << 16) + (i2 << 8) + (i3 << 0);
}

long datastream::read_long(char * buffer)
{
    long l0 = *(buffer + 0)&0xff;
    long l1 = *(buffer + 1)&0xff;
    long l2 = *(buffer + 2)&0xff;
    long l3 = *(buffer + 3)&0xff;
    long l4 = *(buffer + 4)&0xff;
    long l5 = *(buffer + 5)&0xff;
    long l6 = *(buffer + 6)&0xff;
    long l7 = *(buffer + 7)&0xff;
    return (l0 << 56) + (l1 << 48) + (l2 << 40) + (l3 << 32) + 
    (l4 << 24) + (l5 << 16) + (l6 << 8) + (l7 << 0) ;
}

float datastream::read_float(char * buffer)
{
    int fi = datastream::read_int(buffer);
    return *(float*)&fi;
}

double datastream::read_double(char * buffer)
{
    double ld = datastream::read_long(buffer);
    return *(long*)&ld;
}

std::string datastream::read_UTF(char * buffer)
{
    short size = datastream::read_short(buffer);
    return std::string(buffer+2,size);
}

void datafile::write_char(std::ostream &os, const char &c)
{
    os.put(c);
}

void datafile::write_short(std::ostream &os, const short &s)
{
    os.put(s >> 8);
    os.put(s >> 0);
}

void datafile::write_int(std::ostream &os, const int &i)
{
    os.put(i >> 24);
    os.put(i >> 16);
    os.put(i >> 8);
    os.put(i >> 0);
}

void datafile::write_long(std::ostream &os, const long &l)
{
    os.put(l >> 56);
    os.put(l >> 48);
    os.put(l >> 40);
    os.put(l >> 32);
    os.put(l >> 24);
    os.put(l >> 16);
    os.put(l >> 8);
    os.put(l >> 0);
}

void datafile::write_float(std::ostream &os, const float &f)
{
    int fi = *(int*)&f;
    datafile::write_int(os,fi);
}

void datafile::write_double(std::ostream &os, const double &d)
{
    long dl = *(long*)&d;
    datafile::write_long(os,dl);
}

void datafile::write_UTF(std::ostream &os, const std::string &str)
{
    datafile::write_short(os,str.size());
    os.write(str.c_str(),str.size());
}

char datafile::read_char(std::istream &is)
{
    return is.get();
}

short datafile::read_short(std::istream &is)
{
    short s0 = is.get()&0xff;
    short s1 = is.get()&0xff;
    return (s0 << 8) + (s1 << 0);
}

int datafile::read_int(std::istream &is)
{
    int i0 = is.get()&0xff;
    int i1 = is.get()&0xff;
    int i2 = is.get()&0xff;
    int i3 = is.get()&0xff;
    return (i0 << 24) + (i1 << 16) + (i2 << 8) + (i3 << 0);
}

long datafile::read_long(std::istream &is)
{
    long l0 = is.get()&0xff;
    long l1 = is.get()&0xff;
    long l2 = is.get()&0xff;
    long l3 = is.get()&0xff;
    long l4 = is.get()&0xff;
    long l5 = is.get()&0xff;
    long l6 = is.get()&0xff;
    long l7 = is.get()&0xff;
    return (l0 << 56) + (l1 << 48) + (l2 << 40) + (l3 << 32) + 
    (l4 << 24) + (l5 << 16) + (l6 << 8) + (l7 << 0) ;
}

float datafile::read_float(std::istream &is)
{
    int fi = datafile::read_int(is);
    return *(float*)&fi;
}

double datafile::read_double(std::istream &is)
{
    double ld = datafile::read_long(is);
    return *(long*)&ld;
}

std::string datafile::read_UTF(std::istream &is)
{
    short size = datafile::read_short(is);
    char * buffer = new char[size];
    is.read(buffer,size);
    std::string utf(buffer,size);
    delete[] buffer;
    return utf;
}

size_t datafile::size(std::fstream &fs)
{
    size_t pos = fs.tellg();
    fs.seekg(0, fs.end);
    size_t size = fs.tellg();
    fs.seekg(pos, fs.beg);
    return size;
}
