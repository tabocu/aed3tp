#include "database.hpp"
#include <set>

database::database() :
_project_record(2,"project_record.db"),
_project_name_index(2,100,"project_name.db.index"),
_partner_record(2,"partner_record.db"),
_partner_name_index(2,100,"partner_name.db.index"),
_task_record(2,"task_record.db"),
_project_task_index(2,"project_task.db.index"),
_partner_task_index(2,"partner_task.db.index")
{}

void database::insert_project(manager::project &project)
{
    if(!validate_project_partners(project)) return;
    u_int code = _project_name_index.search(project._name);
    if(!code)
    {
        _project_record.insert(project);
        _project_name_index.insert(project._name,project._code);
    }
}

bool database::remove_project(manager::project &project)
{
    if(_project_name_index.remove(project._name))
        return _project_record.remove(project);
    return false;
}

manager::project database::search_project(const u_int &code)
{
    return _project_record.search(code);
}

manager::project database::search_project(const std::string &name)
{
    u_int code = _project_name_index.search(name);
    if(code)
        return _project_record.search(code);
    return manager::project();
}

bool database::update_project(manager::project &project)
{
    manager::project aux = _project_record.search(project._code);
    if(!aux._code) return false;

    if(aux._name.compare(project._name))
    {
        u_int code = _project_name_index.search(project._name);
        if(code) return false;
        _project_name_index.remove(aux._name);
        _project_name_index.insert(project._name,project._code);
        return _project_record.update(project);
    }
    else
    {
        return _project_record.update(project);
    }
}
// project - end

// partner - begin
void database::insert_partner(manager::partner &partner)
{
    u_int code = _partner_name_index.search(partner._name);
    if(!code)
    {
        _partner_record.insert(partner);
        _partner_name_index.insert(partner._name,partner._code);
    }
}

bool database::remove_partner(manager::partner &partner)
{
    if(_partner_name_index.remove(partner._name))
        return _partner_record.remove(partner);
    return false;
}

manager::partner database::search_partner(const u_int &code)
{
    return _partner_record.search(code);
}

manager::partner database::search_partner(const std::string &name)
{
    u_int code = _partner_name_index.search(name);
    if(code)
        return _partner_record.search(code);
    return manager::partner();
}

bool database::update_partner(manager::partner &partner)
{
    manager::partner aux = _partner_record.search(partner._code);
    if(!aux._code) return false;

    if(aux._name.compare(partner._name))
    {
        u_int code = _partner_name_index.search(partner._name);
        if(code) return false;
        _partner_name_index.remove(aux._name);
        _partner_name_index.insert(partner._name,partner._code);
        return _partner_record.update(partner);
    }
    else
    {
        return _partner_record.update(partner);
    }
}

bool database::validate_partners_name(const std::string &name)
{
    return _partner_name_index.search(name);
}

bool database::validate_project_partners(manager::project &project)
{
    for(std::set<u_int>::const_iterator it = project._partners.begin();
        it != project._partners.end();
        ++it)
        if(!_partner_record.contain(*it))
            return false;
    return true;
}

void database::insert_task(manager::task &task)
{
    _task_record.insert(task);
    _project_task_index.insert(task._project,task._code);
    _partner_task_index.insert(task._partner,task._code);
}

void database::remove_task(manager::task &task)
{
    _task_record.remove(task);
    _project_task_index.remove(task._project,task._code);
    _partner_task_index.remove(task._partner,task._code);
}

manager::task database::search_task(const u_int &code)
{
    return _task_record.search(code);
}

std::list<manager::project>* database::list_project()
{
    return _project_record.list();
}

std::list<manager::partner>* database::list_partner()
{
    return _partner_record.list();
}

std::list<manager::task>* database::list_task()
{
    return _task_record.list();
}

std::list<u_int>* database::list_task_project(const u_int &project)
{
    return _project_task_index.list(project);
}

std::list<u_int>* database::list_task_partner(const u_int &partner)
{
    return _partner_task_index.list(partner);
}
