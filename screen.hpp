#ifndef _SCREEN_HPP_
#define _SCREEN_HPP_

#include "database.hpp"
#include "manager.hpp"

namespace screen
{
    void init(database &db);
    void manage_partner(database &db);
    void manage_project(database &db);
    void create_partner(database &db);
    void create_project(database &db);
    manager::partner get_partner(database &db);
}


#endif
