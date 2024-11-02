#include "application.h"
#include "player/player.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    Player p(nullptr, &a);
    a.setStyle("fusion");
    a.setWindow(&p);
    a.showWindow();

    return a.exec();
}
