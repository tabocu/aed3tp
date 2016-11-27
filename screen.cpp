#include "screen.hpp"
#include <string>
#include <ctype.h>
#include <iostream>
#include <sstream>

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
        else if(!opt.compare("3")) return;
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
        std::cout << std::endl;
        std::cout << "(9) Voltar" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::create_partner(db);
        else if(!opt.compare("2")) screen::get_partner(db);
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
        std::cout << std::endl;
        std::cout << "(9) Voltar" << std::endl;
        std::cout << std::endl;
        std::cout << "Opção: ";
        std::getline(std::cin, opt);
        
        if(!opt.compare("1")) screen::create_project(db);
        if(!opt.compare("2")) screen::get_project(db);
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
                buffer << "colaboradores: " << std::endl << std::endl;
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
                p._partners = partner_set;
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
                buffer << p << std::endl;
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
                buffer << p << std::endl;
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

