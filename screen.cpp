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
        
        if(!opt.compare("1")) return;
        else if(!opt.compare("2")) return;
        else if(!opt.compare("3")) return;
        else if(!opt.compare("9")) return;
    }
}

void screen::create_partner(database &db)
{
    std::string name = "", email = "", opt = "";
    bool empty = true;
    
    while(1)
    { 
        opt = "";
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Cadastrar colaborador" << std::endl;
        std::cout << std::endl;

        std::cout << "Nome: " << name;
        if(empty)
            std::getline(std::cin, name);
        else
            std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "Email: " << email;
        if(empty)
            std::getline(std::cin, email);
        else
            std::cout << std::endl;
        std::cout << std::endl;

        bool val_name = db.validate_partners_name(name);

        if(val_name) //INVALIDO
        {
            printf("O colaborador \"%s\""
                "já existe. Deseja incluir outro? (s/n) ", 
                name.c_str());
            name = email = "";
            std::getline(std::cin, opt);
            if(is_letter(opt,'y')) continue;
            if(is_letter(opt,'n')) break;
        }
        else
        {
            std::cout << "Deseja incluir? (s/n) ";
            empty = false;
            std::getline(std::cin, opt);
            if(is_letter(opt,'y'))
            {
                manager::partner p;
                p._name = name;
                p._email = email;
                db.insert_partner(p);
                break;
            }
            if(is_letter(opt,'n')) break;
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
                stage = 3;
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
            std::cout << buffer.str() << std::endl;
            std::cout << "Colaboradores: " << std::endl;
            for(const u_int& i : partner_set)
                std::cout << db.search_partner(i) << std::endl;
            std::cout << "Deseja adicionar um colaborador?(s/n) ";
            std::getline(std::cin, opt);
            if(is_letter(opt,'s'))
            {
                manager::partner p = get_partner(db);
                if(p._code)
                    partner_set.insert(p._code);   
            }
            else if(is_letter(opt,'n'))
                stage = 4;
        }
        else if(stage == 4)
        {
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

