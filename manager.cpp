#include "manager.hpp"
#include "tools.hpp"

#include <ctime>
#include <iostream>
#include <iomanip>

//internal non-member function - begin

void print_u_int(std::ostream &os, const u_int &i)
{
    os << "| ";
    os.width(6);
    os << std::right << i;
    os << " |";
}

void print_string(std::ostream &os, const std::string &s)
{
    os << "| ";
    os.width(30);
    os << std::left << s;
    os << " |";
}

void print_priority(std::ostream &os, const manager::priority &p)
{
    os << "| ";
    os.width(10);
    switch(p)
    {
        case manager::priority::lowest:
            os << std::left << "1 - Baixa";
            break;
        case manager::priority::low:
            os << std::left << "2 - Média baixa";
            break;
        case manager::priority::high:
            os << std::left << "3 - Média alta";
            break;
        case manager::priority::highest:
            os << std::left << "4 - Alta";
            break;
        default:
            os << std::left << "N/A";
            break;
    }
    os << " |";
}

//internal non-member function - end

//manager::entry - begin

void manager::entry::read(char * buffer)
{
    datastream::write_int(buffer,_code);
}

void manager::entry::write(char * buffer)
{
    _code = datastream::read_int(buffer);
}

u_int manager::entry::size()
{
    return _base_size;
}

void manager::entry::print(std::ostream &os) const
{
    print_u_int(os,_code);
}

//manager::entry - end

//manager::project - begin

void manager::project::read(char * buffer)
{
    entry::read(buffer);
    buffer += entry::size();
    datastream::write_UTF(buffer,_name);
    buffer += 2 + _name.size();
    datastream::write_short(buffer,_partners.size());
    buffer += 2;
    for(std::set<u_int>::const_iterator it = _partners.begin();
        it != _partners.end();
        ++it)
    {
        datastream::write_int(buffer,*it);
        buffer += 4;
    }
}

void manager::project::write(char * buffer)
{
    entry::write(buffer);
    buffer += entry::size();
    _name = datastream::read_UTF(buffer);

    buffer += 2 + _name.size();
    u_short partners = datastream::read_short(buffer);

    buffer += 2;
    _partners.clear();
    for(u_int i = 0; i < partners; ++i)
    {
        _partners.insert(datastream::read_int(buffer));
        buffer += 4;
    }
}

u_int manager::project::size()
{
    return entry::size() + _base_size + _name.size() + _partners.size()*4;
}

void manager::project::print(std::ostream &os) const
{
    entry::print(os);
    print_string(os,_name);
}

//manager::project - end

//manager::task - begin

void manager::task::read(char * buffer)
{
    entry::read(buffer);
    buffer += entry::size();
    datastream::write_UTF(buffer,_description);
    buffer += 2 + _description.size();
    datastream::write_int(buffer,_project);
    buffer += 4;
    datastream::write_int(buffer,_partner);
    buffer += 4;
    datastream::write_int(buffer,_dead_line._d);
    buffer += 4;
    datastream::write_int(buffer,_dead_line._m);
    buffer += 4;
    datastream::write_int(buffer,_dead_line._Y);
    buffer += 4;
    datastream::write_char(buffer,_priority);
}

void manager::task::write(char * buffer)
{
    entry::write(buffer);
    buffer += entry::size();
    _description = datastream::read_UTF(buffer);
    buffer += 2 + _description.size();
    _project = datastream::read_int(buffer);
    buffer += 4;
    _partner = datastream::read_int(buffer);
    buffer += 4;
    _dead_line._d = datastream::read_int(buffer);
    buffer += 4;
    _dead_line._m = datastream::read_int(buffer);
    buffer += 4;
    _dead_line._Y = datastream::read_int(buffer);
    buffer += 4;
    _priority = static_cast<priority>(datastream::read_char(buffer));

}

u_int manager::task::size()
{
    return entry::size() + _base_size + _description.size();
}

void manager::task::print(std::ostream &os) const
{
    entry::print(os);
    print_string(os,_description);
    os << "| " << _dead_line << " |";
    print_priority(os,_priority);
}

//manager::task - end

//manager::partner - begin

void manager::partner::read(char * buffer)
{
    entry::read(buffer);
    buffer += entry::size();
    datastream::write_UTF(buffer,_name);
    buffer += 2 + _name.size();
    datastream::write_UTF(buffer,_email);
    buffer += 2 + _email.size();

}

void manager::partner::write(char * buffer)
{
    entry::write(buffer);
    buffer += entry::size();
    _name = datastream::read_UTF(buffer);
    buffer += 2 + _name.size();
    _email = datastream::read_UTF(buffer);
    buffer += 2 + _email.size();

}

u_int manager::partner::size()
{
    return entry::size() + _base_size + _name.size() + _email.size();
}

void manager::partner::print(std::ostream &os) const
{
    entry::print(os);
    print_string(os,_name);
    print_string(os,_email);
}

//manager::partner - end

//manager::date - begin
std::ostream& manager::operator<<(std::ostream &os, const manager::date &date)
{
    os << std::setfill('0') << std::setw(2) << date._d;
    os << "/";
    os << std::setfill('0') << std::setw(2) << date._m;
    os << "/";
    os << std::setfill('0') << std::setw(4) << date._Y;
    os << std::setfill(' ');
    return os;
}
//manager::date - end

//manager non-member functions - begin

std::ostream& manager::operator<<(std::ostream &os, const manager::entry &entry)
{
    entry.print(os);
    return os;
}

//manager non-member functions - end
