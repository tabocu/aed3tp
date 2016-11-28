#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "manager.hpp"
#include "record.hpp"
#include "tree.hpp"
#include "defines.hpp"
#include <string>
#include <list>

class database
{
public:
    database();

    void insert_project(manager::project &project);
    bool remove_project(manager::project &project);
    manager::project search_project(const u_int &code);
    manager::project search_project(const std::string &name);
    bool update_project(manager::project &project);
    std::list<manager::project>* list_project();
    std::list<u_int>* list_task_project(const u_int &project);

    void insert_partner(manager::partner &partner);
    bool remove_partner(manager::partner &partner);
    manager::partner search_partner(const u_int &code);
    manager::partner search_partner(const std::string &name);
    bool update_partner(manager::partner &partner);
    std::list<manager::partner>* list_partner();
    std::list<u_int>* list_task_partner(const u_int &partner);
    
    bool validate_partners_name(const std::string &name);
    bool validate_project_partners(manager::project &project);
    
    void insert_task(manager::task &task);
    manager::task search_task(const u_int &code);
    std::list<manager::task>* list_task();
    void remove_task(manager::task &task);


#ifndef DEBUG   
private:
#endif
    record<manager::project> _project_record;
    tree::b_string _project_name_index;

    record<manager::partner> _partner_record;
    tree::b_string _partner_name_index;

    record<manager::task> _task_record;
    tree::b_plus _project_task_index;
    tree::b_plus _partner_task_index;
};

#endif
