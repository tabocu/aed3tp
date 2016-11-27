#include "database.hpp"
#include "screen.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    database db;
    screen::init(db);
    return 0;
}
