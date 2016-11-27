#include <iostream>
#include <list>
#include <cstring>
#include "tree.hpp"
#include "tools.hpp"
#include "manager.hpp"
#include "record.hpp"
#include "database.hpp"
#include "screen.hpp"

using namespace std;


void b_test()
{
    tree::b tree(2,"b.db");

    std::cout << "Árvore B de ordem 2:" << std::endl;
    std::cout << "Inserção de 10,1 - 25,2 - 30,3 - 17,4" << std::endl;
    tree.insert(10,1);
    tree.insert(25,2);
    tree.insert(30,3);
    tree.insert(17,4);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 15,5" << std::endl;
    tree.insert(15,5);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 5,6" << std::endl;
    tree.insert(5,6);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 46,7" << std::endl;
    tree.insert(46,7);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 23,8 - 8,9 - 42,10" << std::endl;
    tree.insert(23,8);
    tree.insert(8,9);
    tree.insert(42,10);
    std::cout << tree << std::endl;
    std::cout << "Search: " << 10 << " -> " << tree.search(10) << std::endl;
    std::cout << "Search: " << 25 << " -> " << tree.search(25) << std::endl;
    std::cout << "Search: " << 30 << " -> " << tree.search(30) << std::endl;
    std::cout << "Search: " << 17 << " -> " << tree.search(17) << std::endl;
    std::cout << "Search: " << 15 << " -> " << tree.search(15) << std::endl;
    std::cout << "Search: " <<  5 << " -> " << tree.search( 5) << std::endl;
    std::cout << "Search: " << 46 << " -> " << tree.search(46) << std::endl;
    std::cout << "Search: " << 23 << " -> " << tree.search(23) << std::endl;
    std::cout << "Search: " <<  8 << " -> " << tree.search( 8) << std::endl;
    std::cout << "Search: " << 42 << " -> " << tree.search(42) << std::endl;
    
    
    std::cout << "Troca de 5,6 por 5,66" << std::endl;
    std::cout << "Troca de 46,7 por 46,70" << std::endl;

    tree.update(5,66);
    tree.update(46,70);
   
    std::cout << "Search: " << 10 << " -> " << tree.search(10) << std::endl;
    std::cout << "Search: " << 25 << " -> " << tree.search(25) << std::endl;
    std::cout << "Search: " << 30 << " -> " << tree.search(30) << std::endl;
    std::cout << "Search: " << 17 << " -> " << tree.search(17) << std::endl;
    std::cout << "Search: " << 15 << " -> " << tree.search(15) << std::endl;
    std::cout << "Search: " <<  5 << " -> " << tree.search( 5) << std::endl;
    std::cout << "Search: " << 46 << " -> " << tree.search(46) << std::endl;
    std::cout << "Search: " << 23 << " -> " << tree.search(23) << std::endl;
    std::cout << "Search: " <<  8 << " -> " << tree.search( 8) << std::endl;
    std::cout << "Search: " << 42 << " -> " << tree.search(42) << std::endl;

    
    std::cout << "Remove 30:" << std::endl;
    tree.remove(30);
    std::cout << tree << std::endl;

    std::cout << "Remove 46:" << std::endl;
    tree.remove(46);
    std::cout << tree << std::endl;
}

void b_string_test()
{
    tree::b_string tree(2,20,"b_sring.db");

    std::cout << "Árvore B Indireta de ordem 2:" << std::endl;
    std::cout << "Inserção de Tales,1 - Casa,2 - Ju,3 - Fome,4" << std::endl;
    tree.insert("Tales",1);
    tree.insert("Casa",2);
    tree.insert("Ju",3);
    tree.insert("Fome",4);
    std::cout << tree << std::endl;
    std::cout << "Inserção de Coca,5" << std::endl;
    tree.insert("Coca",5);
    std::cout << tree << std::endl;
    std::cout << "Inserção de Quiabo,6" << std::endl;
    tree.insert("Quiabo",6);
    std::cout << tree << std::endl;
    std::cout << "Inserção de Feijão,7" << std::endl;
    tree.insert("Feijão",7);
    std::cout << tree << std::endl;
    std::cout << "Inserção de Tv,8 - Chocolate,9 - Cigarro,10" << std::endl;
    tree.insert("Tv",8);
    tree.insert("Chocolate",9);
    tree.insert("Cigarro",10);
    std::cout << tree << std::endl;
    std::cout << "Search: " << "Tales   " << " -> " << tree.search("Tales")     << std::endl;
    std::cout << "Search: " << "Casa    " << " -> " << tree.search("Casa")      << std::endl;
    std::cout << "Search: " << "Ju      " << " -> " << tree.search("Ju")        << std::endl;
    std::cout << "Search: " << "Fome    " << " -> " << tree.search("Fome")      << std::endl;
    std::cout << "Search: " << "Coca    " << " -> " << tree.search("Coca")      << std::endl;
    std::cout << "Search: " << "Quiabo  " << " -> " << tree.search("Quiabo")    << std::endl;
    std::cout << "Search: " << "Feijão  " << " -> " << tree.search("Feijão")    << std::endl;
    std::cout << "Search: " << "Tv      " << " -> " << tree.search("Tv")        << std::endl;
    std::cout << "Search: " << "Chcolate" << " -> " << tree.search("Chocolate") << std::endl;
    std::cout << "Search: " << "Cigarro " << " -> " << tree.search("Cigarro")   << std::endl;

    std::cout << "Troca de Tv,8 por Tv 80" << std::endl;
    std::cout << tree.update("Tv",80) << std::endl;
    std::cout << "Troca de Quiabo,7 por Quiabo,70" << std::endl;
    std::cout << tree.update("Quiabo",70) << std::endl;

    std::cout << "Search: " << "Tales   " << " -> " << tree.search("Tales")     << std::endl;
    std::cout << "Search: " << "Casa    " << " -> " << tree.search("Casa")      << std::endl;
    std::cout << "Search: " << "Ju      " << " -> " << tree.search("Ju")        << std::endl;
    std::cout << "Search: " << "Fome    " << " -> " << tree.search("Fome")      << std::endl;
    std::cout << "Search: " << "Coca    " << " -> " << tree.search("Coca")      << std::endl;
    std::cout << "Search: " << "Quiabo  " << " -> " << tree.search("Quiabo")    << std::endl;
    std::cout << "Search: " << "Feijão  " << " -> " << tree.search("Feijão")    << std::endl;
    std::cout << "Search: " << "Tv      " << " -> " << tree.search("Tv")        << std::endl;
    std::cout << "Search: " << "Chcolate" << " -> " << tree.search("Chocolate") << std::endl;
    std::cout << "Search: " << "Cigarro " << " -> " << tree.search("Cigarro")   << std::endl;
    
    std::cout << "Remoção de Quiabo" << std::endl;
    tree.remove("Quiabo");
    std::cout << tree << std::endl;
    std::cout << "Remoção de Feijão" << std::endl;
    tree.remove("Feijão");
    std::cout << tree << std::endl;
}

void b_plus_test()
{
    tree::b_plus tree(2,"b_plus.db");

    std::cout << "Árvore B+ de ordem 2:" << std::endl;
    std::cout << "Inserção de 20,20 - 20,21 - 30,30 - 20,13" << std::endl;
    tree.insert(20,20);
    tree.insert(20,21);
    tree.insert(30,30);
    tree.insert(20,13);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 20,28" << std::endl;
    tree.insert(20,28);
    std::cout << tree << std::endl;

    std::cout << "Lista de key_b de 20:"<< std::endl;
    std::list<u_int> * list20 = tree.list(20);
    for(std::list<u_int>::const_iterator it = list20->begin();
        it != list20->end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl << std::endl;
    
    delete list20;

    std::cout << "Inserção de 10,10" << std::endl;
    tree.insert(10,10);
    std::cout << tree << std::endl;
    std::cout << "Inserção de 10,11" << std::endl;
    tree.insert(10,11);
    std::cout << tree << std::endl;
   
    std::cout << "Remoção de 20,20" << std::endl;
    tree.remove(20,20);
    std::cout << tree << std::endl;

    std::cout << "Remoção de 20,13" << std::endl;
    tree.remove(20,13);
    std::cout << tree << std::endl;

    std::cout << "Lista de key_b de 20:"<< std::endl;
    list20 = tree.list(20);
    for(std::list<u_int>::const_iterator it = list20->begin();
        it != list20->end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl << std::endl;
    
    delete list20;
   
    std::cout << "Remoção de 20,21" << std::endl;
    tree.remove(20,21);
    std::cout << tree << std::endl;

    std::cout << "Remoção de 20,28" << std::endl;
    tree.remove(20,28);
    std::cout << tree << std::endl;
}

void manager_test()
{
    record<manager::project> project_record(2,"project_record.db");
    record<manager::task> task_record(2,"task_record.db");
    record<manager::partner> partner_record(2,"partner_record.db");


    manager::project project;
    project._name = "JasperGold CDC";

    manager::task task;
    task._description = "Corrigir combo_loop";
    task._project = 13;
    task._partner = 25;
    task._dead_line = 1479149210L;
    task._priority = manager::priority::high;

    manager::partner partner;
    partner._name = "Tales Bontempo";
    partner._email = "tabocu@gmail.com";

    std::cout << task    << std::endl << std::endl;

    std::cout << project << std::endl << std::endl;

    std::cout << partner << std::endl << std::endl;
}

void record_test()
{
    record<manager::partner> partner_record(2,"partner_record.db");

    manager::partner partner;

    partner._name = "Tales Bontempo Cunha";
    partner_record.insert(partner);
    partner._name = "Junnia";
    partner_record.insert(partner);
    partner._name = "Lidice";
    partner_record.insert(partner);
    partner._name = "Nina";
    partner_record.insert(partner);
    partner._name = "Zeldinha";
    partner_record.insert(partner);
    partner._name = "Cecilia";
    partner_record.insert(partner);
    partner._name = "Guilherme";
    partner_record.insert(partner);
    partner._name = "Jana";
    partner_record.insert(partner);
    partner._name = "Gustavo";
    partner_record.insert(partner);
    partner._name = "Luisa";
    partner_record.insert(partner);
    partner._name = "Isadora";
    partner_record.insert(partner);
    partner._name = "Leonardo";
    partner_record.insert(partner);
    partner._name = "Aline";
    partner_record.insert(partner);
    partner._name = "Estela";
    partner_record.insert(partner);
    partner._name = "Flavio";
    partner_record.insert(partner);
    partner._name = "Paola Bracho";
    partner_record.insert(partner);
    partner._name = "Carlos Daniel";
    partner_record.insert(partner);

    for(u_int i = 1; i <= 10; ++i)
    {
        manager::partner p = partner_record.search(i);
        
        p._name = p._name + "MOD";
        partner_record.update(p);
        
    }
    std::cout << partner_record._index << std::endl;
    partner_record.remove(partner_record.search(11));
    std::cout << partner_record._index << std::endl;
    partner_record.remove(partner_record.search(13));
    std::cout << partner_record._index << std::endl;

    for(u_int i = 1; i <= 14; ++i)
    {
        manager::partner p = partner_record.search(i);
        
        std::cout << p << std::endl;
    }
}

void database_test()
{
    database db;

    manager::partner p;

    p._name = "Tales";
    p._email = "tabocu@gmail.com";
    db.insert_partner(p);
    p._name = "Junnia";
    p._email = "ju.barcelos@gmail.com";
    db.insert_partner(p);
    p._name = "Gustavo";
    p._email = "gus.nowak@gmail.com";
    db.insert_partner(p);
    p._name = "Nilo";
    p._email = "nilold@gmail.com";
    db.insert_partner(p);
    
    manager::partner aux = db.search_partner("Nilo");
    std::cout << aux << std::endl;

    db.remove_partner(aux);

    manager::partner aux2 = db.search_partner("Nilo");
    std::cout << aux2 << std::endl;
    manager::partner aux3 = db.search_partner("Gustavo");
    aux3._email = "gustavo_nowak@gmail.com";
    db.update_partner(aux3);

    manager::partner aux4 = db.search_partner("Gustavo");
    std::cout << aux4 << std::endl;

    manager::partner aux5 = db.search_partner("Junnia");
    std::cout << aux5 << std::endl;
    aux5._name = "Ju";
    std::cout << db.update_partner(aux5) << std::endl;

    manager::partner aux6 = db.search_partner("Ju");
    std::cout << aux6 << std::endl;
    
    manager::project pj;
    pj._name = "Combo loop";
    pj._partners.insert(2);
    pj._partners.insert(3);

    std::cout << db.validate_project_partners(pj) << std::endl; 
    db.insert_project(pj);
    manager::project p2 = db.search_project("Combo loop");
    std::cout << p2 << std::endl;


}

void b_string_test_2()
{
    tree::b_string tree(2,10,"b_string.db");
    tree.insert("Tales",1);
    tree.insert("Junnia",2);
    tree.insert("Gustavo",3);
    tree.insert("Nilo",4);

    std::cout << tree << std::endl;

    tree.remove("Nilo");

    std::cout << tree << std::endl;

    tree.remove("Junnia");
    
    std::cout << tree << std::endl;

    tree.insert("Xuhh",2);

    std::cout << tree << std::endl;
    
    std::cout << tree.search("Xuhh") << std::endl;
}

void strcpy_test()
{
    char * buffer = new char[100];
    std::string word = "Tales";
    strcpy(buffer+10,word.c_str());
    std::string out(buffer+10);
    std::cout << out << std::endl;
    delete[] buffer;
}

void interface_test()
{
    database db;
    screen::init(db);
}

int main(int argc, char *argv[])
{
    #ifdef DEBUG
    std::cout << "Versão de depuração" << std::endl;
    #endif
    //b_test();
    //b_string_test();
    //b_plus_test();
   // manager_test();
   // record_test();
//   database_test();
    //strcpy_test();
    interface_test();

    return 0;
}
