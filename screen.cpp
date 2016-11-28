#include "screen.hpp"
#include <string>
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <ctime>

bool is_yes(std::string opt)
{
    return !opt.empty() && (opt[0] == 's' || opt[0] == 'S');
}


bool is_no(std::string opt)
{
    return !opt.empty() && (opt[0] == 'n' || opt[0] == 'N');
}

bool is_letter(const std::string &opt, const char &copt)
{
    return !opt.empty() && (toupper(opt[0]) == toupper(copt));
}

bool validate_date(std::string s)
{
    if(s.size() != 10) return false;

    for(u_int i = 0; i < s.size(); ++i)
    {
        if(i == 2 || i == 5)
        {
            if(s[i] != '/') return false;
        }
        else
        {
            if(s[i] < '0' || s[i] > '9') return false;
        }
    }

    int d = (s[0]-'0')*10 + s[1]-'0';
    int m = (s[3]-'0')*10 + s[4]-'0';
    int Y = (s[6]-'0')*1000 + (s[7]-'0')*100 + (s[8]-'0')*10 + s[9]-'0';

    if(d == 0 || m == 0 || Y < 1970 || m > 12) return false;
    if((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) &&
        d > 31) return false;
    if((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return false;
    if(Y%4 == 0 && m == 2 && d > 29) return false;
    if(Y%4 != 0 && m == 2 && d > 28) return false;

    return true;
}

void screen::init(database &db)
{
    std::string opt = "";

    while(1)
    {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Project Manager" << std::endl;
        std::cout << std::endl;
        std::cout << "(1) Gerenciar colaboradores" << std::endl;
        std::cout << "(2) Gerenciar projetos"      << std::endl;
        std::cout << "(3) Gerenciar tarefas"       << std::endl;
        std::cout << std::endl;
        std::cout << "(9) Sair" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::manage_partner(db);
        else if(!opt.compare("2")) screen::manage_project(db);
        else if(!opt.compare("3")) screen::manage_task(db);
        else if(!opt.compare("9")) 
        {
            std::cout << "\033[2J\033[1;1H";
            return;
        }
    }
}

void screen::manage_partner(database &db)
{
     std::string opt = "";

    while(1)
    {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Project Manager" << std::endl;
        std::cout << std::endl;
        std::cout << "Gerenciar Colaborador" << std::endl;
        std::cout << std::endl;
        std::cout << "(1) Criar colaborador" << std::endl;
        std::cout << "(2) Pesquisar colaborador"      << std::endl;
        std::cout << "(3) Listar colaboradores"      << std::endl;
        std::cout << std::endl;
        std::cout << "(9) Voltar" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::create_partner(db);
        else if(!opt.compare("2")) screen::get_partner(db);
        else if(!opt.compare("3")) screen::list_partner(db);
        else if(!opt.compare("9")) return;
    }
}

void screen::manage_project(database &db)
{
     std::string opt = "";

    while(1)
    {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Project Manager" << std::endl;
        std::cout << std::endl;
        std::cout << "Gerenciar Projeto" << std::endl;
        std::cout << std::endl;
        std::cout << "(1) Criar projeto" << std::endl;
        std::cout << "(2) Pesquisar projeto" << std::endl;
        std::cout << "(3) Listar projetos" << std::endl;
        std::cout << std::endl;
        std::cout << "(9) Voltar" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::create_project(db);
        if(!opt.compare("2")) screen::get_project(db);
        if(!opt.compare("3")) screen::list_project(db);
        else if(!opt.compare("9")) return;
    }
}

void screen::manage_task(database &db)
{
     std::string opt = "";

    while(1)
    {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Project Manager" << std::endl;
        std::cout << std::endl;
        std::cout << "Gerenciar Tarefa" << std::endl;
        std::cout << std::endl;
        std::cout << "(1) Criar tarefa" << std::endl;
        std::cout << "(2) Pesquisar tarefa" << std::endl;
        std::cout << "(3) Listar tarefas" << std::endl;
        std::cout << std::endl;
        std::cout << "(9) Voltar" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::create_task(db);
        if(!opt.compare("2")) screen::get_task(db);
        if(!opt.compare("3")) screen::list_task(db);
        else if(!opt.compare("9")) return;
    }
}

void screen::create_partner(database &db)
{
    
    std::stringstream buffer;
    std::string name = "", email = "", opt = "";

    u_char stage = 1;
    while(1)
    { 
        if(stage == 1)
        {
            buffer.clear();
            buffer << "\033[2J\033[1;1H";
            buffer << "Cadastrar colaborador:" << std::endl;
            buffer << std::endl;
            buffer << "Nome: ";
            std::cout << buffer.str();
            std::getline(std::cin, name);
            buffer << name << std::endl;
            manager::partner p = db.search_partner(name);
            if(p._code)
                stage = 2;
            else
                stage = 3;
        }
        else if(stage == 2)
        {
            std::cout << buffer.str();
            std::cout << "Já existe um colaborador com este nome!" << std::endl;
            std::cout << "Deseja cadastrar outro?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
                stage = 1;
            else if(is_letter(opt,'n'))
                return;
        }
        else if(stage == 3)
        {
            buffer << "Email: ";
            std::cout << buffer.str();
            std::getline(std::cin, email);
            buffer << email << std::endl;
            std::cout << "Deseja adicionar este colaborador?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s')) 
            {
                manager::partner p;
                p._name = name;
                p._email = email;
                db.insert_partner(p);
                buffer << "Colaborador adicionado!" << std::endl;
                stage = 4;
            }
            else if(is_letter(opt,'n'))
                stage = 4;
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();
            std::cout << "Deseja adicionar mais contatos?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
                stage = 1;
            else if(is_letter(opt,'n'))
                return;
        }
    }
}

void screen::create_project(database &db)
{
    
    std::stringstream buffer;
    std::string name = "", opt = "";
    std::set<u_int> partner_set;

    u_char stage = 1;
    while(1)
    { 
        if(stage == 1)
        {
            partner_set.clear();
            buffer.clear();
            buffer << "\033[2J\033[1;1H";
            buffer << "Cadastrar projeto:" << std::endl;
            buffer << std::endl;
            buffer << "Nome: ";
            std::cout << buffer.str();
            std::getline(std::cin, name);
            buffer << name << std::endl;
            manager::project p = db.search_project(name);
            if(p._code)
                stage = 2;
            else
            {
                buffer << std::endl;
                buffer << "Colaboradores associados:" << std::endl;
                stage = 3;
            }
        }
        else if(stage == 2)
        {
            std::cout << buffer.str();
            std::cout << "Já existe um projeto com este nome!" << std::endl;
            std::cout << "Deseja cadastrar outro?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
                stage = 1;
            else if(is_letter(opt,'n'))
                return;
        }
        else if(stage == 3)
        {
            std::cout << buffer.str();
            
            std::cout << "Deseja adicionar um colaborador?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
            {
                manager::partner p = get_partner(db);
                if(p._code && partner_set.insert(p._code).second)
                    buffer << p << std::endl;
            }
            else if(is_letter(opt,'n'))
                stage = 4;
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();
            std::cout << "Deseja adicionar este projeto?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s')) 
            {
                manager::project p;
                p._name = name;
                p._partners = std::set<u_int>(partner_set);
                db.insert_project(p);

                buffer << "Projeto adicionado!" << std::endl;
                stage = 5;
            }
            else if(is_letter(opt,'n'))
                stage = 5;
        }
        else if(stage == 5)
        {
            std::cout << buffer.str();
            std::cout << "Deseja adicionar mais projetos?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
                stage = 1;
            else if(is_letter(opt,'n'))
                return;
        }
    }
}

void screen::create_task(database &db)
{
    
    std::stringstream buffer;
    std::string description = "", date = "", priority, opt = "";
    u_int project = 0, partner = 0;
    manager::priority pri;
    
    bool no_partners;

    u_char stage = 1;
    while(1)
    { 
        if(stage == 1)
        {
            no_partners = false;
            buffer.clear();
            buffer << "\033[2J\033[1;1H";
            buffer << "Cadastrar tarefa:" << std::endl;
            buffer << std::endl;
            buffer << "Descrição: ";
            std::cout << buffer.str();
            std::getline(std::cin, description);
            buffer << description << std::endl;

            stage = 2;
        }
        else if(stage == 2)
        {
            std::cout << buffer.str();
            if(no_partners)
                std::cout << "Este projeto não tem colaboradores." << std::endl;
            std::cout << "Adicione um projeto que tenha colaboradores. ";
            std::cout << "Adicionar ou cancelar?(a/c) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'a'))
            {
                manager::project p = get_project(db);
                project = p._code;
                if(p._code)
                {
                    if(p._partners.size())
                    {
                        buffer << "Project: " << p._code << " - ";
                        buffer << p._name << std::endl;
                        stage = 3;
                    }
                    else
                        no_partners = true;
                }
            }
            else if(is_letter(opt,'c'))
                stage = 7; 
        }
        else if(stage == 3)
        {
            std::cout << buffer.str() << std::endl;
            manager::project p = db.search_project(project);
            
            for(std::set<u_int>::iterator it = p._partners.begin();
                it != p._partners.end(); ++it)
                std::cout << db.search_partner(*it) << std::endl;
            
            std::cout << std::endl << "Escolha um colaborador: ";
            std::getline(std::cin, opt);
            char * v;
            partner = strtoul(opt.c_str(),&v,10);
            if(!(*v || 
                opt.find('-') != std::string::npos ||
                opt.find('+') != std::string::npos))
                {
                    manager::partner pt = db.search_partner(partner);
                    if(pt._code)
                    {
                        buffer << "Colaborador: " << pt._code << " - ";
                        buffer << pt._name << std::endl;
                        buffer << "Data limite (dd/mm/aaaa): ";
                        stage = 4;
                    }
                }
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();
            std::getline(std::cin, date);
            if(validate_date(date))
            {
                buffer << date << std::endl;
                buffer << "Prioridade: ";
                stage = 5;
            }

        }
        else if(stage == 5)
        {
            std::cout << buffer.str() << std::endl;
            std::cout << "1 - Baixa" << std::endl;
            std::cout << "2 - Média baixa" << std::endl;
            std::cout << "3 - Média alta" << std::endl;
            std::cout << "4 - Alta" << std::endl;
            std::cout << "Opção: ";
            std::getline(std::cin, priority);
            if(!priority.compare("1"))
            {
                pri = manager::priority::lowest;
                buffer << "1 - Baixa" << std::endl;
                stage = 6;
            }
            else if(!priority.compare("2"))
            {
                pri = manager::priority::low;
                buffer << "2 - Média baixa" << std::endl;
                stage = 6;
            }
            else if(!priority.compare("3"))
            {
                pri = manager::priority::high;
                buffer << "3 - Média alta" << std::endl;
                stage = 6;
            }
            else if(!priority.compare("4"))
            {
                pri = manager::priority::highest;
                buffer << "4 - Alta" << std::endl;
                stage = 6;
            }
        }
        else if(stage == 6)
        {
            std::cout << buffer.str();
            std::cout << "Deseja salvar a tarefa?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
            {
                manager::task t;
                t._description = description;
                t._project = project;
                t._partner = partner;
                t._priority = pri;
                t._dead_line._d = (date[0]-'0')*10 + date[1]-'0';
                t._dead_line._m = (date[3]-'0')*10 + date[4]-'0';
                t._dead_line._Y = (date[6]-'0')*1000 
                    + (date[7]-'0')*100 + (date[8]-'0')*10 + date[9]-'0';
                
                db.insert_task(t);
                stage = 7;
            }
            else if(is_letter(opt,'n'))
                stage = 7;
            
        }
        else if(stage == 7)
        {
            std::cout << buffer.str();
            std::cout << "Deseja adicionar mais tarefas?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
                stage = 1;
            else if(is_letter(opt,'n'))
                return;
        }
    }
}

manager::partner screen::get_partner(database &db)
{
    std::stringstream buffer;
    std::string key, opt, opt2;
    key = opt = opt2 = "";
    u_int code = 0;

    u_char stage = 1;
    while(1)
    {
        if(stage == 1)
        {
            std::cout << "\033[2J\033[1;1H";
            std::cout << "Buscar colaborador" << std::endl;
            std::cout << std::endl;
            std::cout << "Pesquisar por nome(n), código(c) ou cancelar(x)? ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'n')) stage = 2;
            else if(is_letter(opt,'c')) stage = 3;
            else if(is_letter(opt,'x')) return manager::partner();
        }
        else if(stage == 2)
        {
            buffer << "\033[2J\033[1;1H";
            buffer << "Buscar colaborador" << std::endl;
            buffer << std::endl;
            buffer << "Digite o nome: ";
            std::cout << buffer.str();
            std::getline(std::cin, key);
            buffer << key << std::endl;

            manager::partner p = db.search_partner(key);
            code = p._code;
            if(code)
                buffer << p << std::endl;
            else
                buffer << "Colaborador não encontrado!" << std::endl;

            stage = 4;
        }
        else if(stage == 3)
        {
            buffer << "\033[2J\033[1;1H";
            buffer << "Buscar colaborador" << std::endl;
            buffer << std::endl;
            buffer << "Digite o código: ";
            std::cout << buffer.str();
            std::getline(std::cin, key);
            char * v;
            code = strtoul(key.c_str(),&v,10);
            if(*v || 
                key.find('-') != std::string::npos ||
                key.find('+') != std::string::npos)
            {
                buffer.clear();
                continue;
            }
            buffer << key << std::endl;
            
            manager::partner p = db.search_partner(code);
            code = p._code;
            if(code)
                buffer << p << std::endl;
            else
                buffer << "Colaborador não encontrado!" << std::endl;

            stage = 4;
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();

            std::cout << "Deseja pesquisar outro colaborador?(s/n) ";
            std::getline(std::cin, opt);

            if(is_letter(opt,'s'))
            {
                buffer.clear();
                stage = 1;
            }
            else if(is_letter(opt,'n')) return db.search_partner(code);
        }
    }
}

manager::project screen::get_project(database &db)
{
    std::stringstream buffer;
    std::string key, opt, opt2;
    key = opt = opt2 = "";
    u_int code = 0;

    u_char stage = 1;
    while(1)
    {
        if(stage == 1)
        {
            buffer.clear();
            buffer << "\033[2J\033[1;1H";
            buffer << "Buscar Projeto" << std::endl;
            buffer << std::endl;
            std::cout << buffer.str();
            std::cout << "Pesquisar por nome(n), código(c) ou cancelar(x)? ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'n')) stage = 2;
            else if(is_letter(opt,'c')) stage = 3;
            else if(is_letter(opt,'x')) return manager::project();
        }
        else if(stage == 2)
        {
            buffer << "Digite o nome: ";
            std::cout << buffer.str();
            std::getline(std::cin, key);
            buffer << key << std::endl;

            manager::project p = db.search_project(key);
            code = p._code;
            if(code)
            {
                buffer << p << std::endl;
                buffer << "Colaboradores associados:" << std::endl;
                for(std::set<u_int>::iterator it = p._partners.begin();
                    it != p._partners.end(); ++it)
                    buffer << db.search_partner(*it) << std::endl;
            }
            else
                buffer << "Projeto não encontrado!" << std::endl;

            stage = 4;
        }
        else if(stage == 3)
        {
            buffer << "Digite o código: ";
            std::cout << buffer.str();
            std::getline(std::cin, key);
            char * v;
            code = strtoul(key.c_str(),&v,10);
            if(*v || 
                key.find('-') != std::string::npos ||
                key.find('+') != std::string::npos)
            {
                buffer.clear();
                continue;
            }
            buffer << key << std::endl;
            
            manager::project p = db.search_project(code);
            code = p._code;
            if(code)
            {
                buffer << p << std::endl;
                buffer << "Colaboradores associados:" << std::endl;
                for(std::set<u_int>::const_iterator it = p._partners.begin();
                    it != p._partners.end(); ++it)
                    buffer << db.search_partner(*it) << std::endl;
            }
            else
                buffer << "Projeto não encontrado!" << std::endl;

            stage = 4;
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();

            std::cout << "Deseja pesquisar outro projet?(s/n) ";
            std::getline(std::cin, opt);

            if(is_letter(opt,'s'))
            {
                buffer.clear();
                stage = 1;
            }
            else if(is_letter(opt,'n')) return db.search_project(code);
        }
    }
}

manager::task screen::get_task(database &db)
{
    std::stringstream buffer;
    std::string key, opt;
    key = opt = "";
    u_int code = 0;

    u_char stage = 1;
    while(1)
    {
        if(stage == 1)
        {
            buffer.clear();
            buffer << "\033[2J\033[1;1H";
            buffer << "Buscar Tarefa" << std::endl;
            buffer << std::endl;
            std::cout << buffer.str();
            std::cout << "Pesquisar por código(c) ou cancelar(x)? ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'c')) stage = 3;
            else if(is_letter(opt,'x')) return manager::task();
        }
        else if(stage == 3)
        {
            buffer << "Digite o código: ";
            std::cout << buffer.str();
            std::getline(std::cin, key);
            char * v;
            code = strtoul(key.c_str(),&v,10);
            if(*v || 
                key.find('-') != std::string::npos ||
                key.find('+') != std::string::npos)
            {
                buffer.clear();
                continue;
            }
            buffer << key << std::endl;
            
            manager::task t = db.search_task(code);
            code = t._code;
            if(code)
            {
                buffer << t << std::endl;
                buffer << "Projeto: " << std::endl;
                buffer << db.search_project(t._project) << std::endl;
                buffer << "Colaborador: " << std::endl;
                buffer << db.search_partner(t._partner) << std::endl;
            }
            else
                buffer << "Tarefa não encontrada!" << std::endl;

            stage = 4;
        }
        else if(stage == 4)
        {
            std::cout << buffer.str();

            std::cout << "Deseja pesquisar outra tarefa?(s/n) ";
            std::getline(std::cin, opt);

            if(is_letter(opt,'s'))
            {
                buffer.clear();
                stage = 1;
            }
            else if(is_letter(opt,'n')) return db.search_task(code);
        }
    }
}

void screen::list_project(database &db)
{
    std::stringstream buffer;
    std::string opt;
    
    buffer << "\033[2J\033[1;1H";
    buffer << "Projetos: " << std::endl << std::endl;
    std::list<manager::project> list = db.list_project();
    for(auto p : list)
        buffer << p << std::endl;

    while(!is_letter(opt,'x'))
    {
        std::cout << buffer.str() << std::endl << std::endl;
        std::cout << "Digite (x) para sair: ";
        std::getline(std::cin, opt);
    }
}

void screen::list_partner(database &db)
{
    std::stringstream buffer;
    std::string opt;
    
    buffer << "\033[2J\033[1;1H";
    buffer << "Colaboradores: " << std::endl << std::endl;
    std::list<manager::partner> list = db.list_partner();
    for(auto p : list)
        buffer << p << std::endl;

    while(!is_letter(opt,'x'))
    {
        std::cout << buffer.str() << std::endl << std::endl;
        std::cout << "Digite (x) para sair: ";
        std::getline(std::cin, opt);
    }
}

void screen::list_task(database &db)
{
    std::stringstream buffer;
    std::string opt;
    
    buffer << "\033[2J\033[1;1H";
    buffer << "Tarefas: " << std::endl << std::endl;
    std::list<manager::task> list = db.list_task();
    for(auto p : list)
        buffer << p << std::endl;

    while(!is_letter(opt,'x'))
    {
        std::cout << buffer.str() << std::endl << std::endl;
        std::cout << "Digite (x) para sair: ";
        std::getline(std::cin, opt);
    }
}
