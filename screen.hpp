#ifndef _SCREEN_HPP_
#define _SCREEN_HPP_

#include "database.hpp"
#include "manager.hpp"

namespace screen
{
    void init(database &db);
    void manage_partner(database &db);
    void manage_project(database &db);
    void manage_task(database &db);
    void create_partner(database &db);
    void create_project(database &db);
    void create_task(database &db);
    manager::partner get_partner(database &db);
    manager::project get_project(database &db);
    manager::task get_task(database &db);
    void list_project(database &db);
    void list_partner(database &db);
    void list_task(database &db);
    void list_task_project(database &db);
    void list_task_partner(database &db);
}


#endif
