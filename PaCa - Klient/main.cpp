#include <cstdlib>

#include <SDL.h>
#include "Pole.h"
#include "globals.h"
#include "Grafika.hpp"
#include "net.hpp"
#include <ctime>
using namespace std;
int main ( int argc, char** argv )
{
    int gra=1;
    if (argc<5)
    {
        cout<<"Użycie:\nPaca - Klient [polecenie] [nazwa_gry] [typ_gry] [plansza_x] [plansza_y] [ip] [port] [max_graczy] [max_punktów]\n\
            polecenie\t- conect - dołącza do gry, create - tworzy grę\n\
            nazwa_gry\t- pod jaką nazwą będzie utworzona gra\n\
            typ gry\t- 0 - zwykła gra, 1 - deathmatch\n\
            plansza_x\t- wielkość planszy w pozioime\n\
            plansza_y\t- wielkość planszy w pionie\n\
            ip\t\t- ip serwera\n\
            port\t- port na którym pracuje serwer\n\
            max_graczy\t- maksymalna ilosc graczy w grze\n\
            max_punktów\t- maksymalna ilość punktów w trybe deathmatch, jedak nie mniej niż max_graczy*60\n"<<endl;
        exit(1);
    }
    if (string(argv[1]).compare("create")==0)
    {
        if (argc<9)
        {
            cout<<"Użycie:\nPaca - Klient [polecenie] [nazwa_gry] [typ_gry] [plansza_x] [plansza_y] [ip] [port] [max_graczy] [max_punktów]\n\
            polecenie\t- conect - dołącza do gry, create - tworzy grę\n\
            nazwa_gry\t- pod jaką nazwą będzie utworzona gra\n\
            typ gry\t- 0 - zwykła gra, 1 - deathmatch\n\
            plansza_x\t- wielkość planszy w pozioime\n\
            plansza_y\t- wielkość planszy w pionie\n\
            ip\t\t- ip serwera\n\
            port\t- port na którym pracuje serwer\n\
            max_graczy\t- maksymalna ilosc graczy w grze\n\
            max_punktów\t- maksymalna ilość punktów w trybe deathmatch, jedak nie mniej niż max_graczy*60\n"<<endl;
            exit(1);
        }
        gra=atoi(argv[3]);
        if (gra==1 && argc<10)
        {
            cout<<"Użycie:\nPaca - Klient [polecenie] [nazwa_gry] [typ_gry] [plansza_x] [plansza_y] [ip] [port] [max_graczy] [max_punktów]\n\
            polecenie\t- conect - dołącza do gry, create - tworzy grę\n\
            nazwa_gry\t- pod jaką nazwą będzie utworzona gra\n\
            typ gry\t- 0 - zwykła gra, 1 - deathmatch\n\
            plansza_x\t- wielkość planszy w pozioime\n\
            plansza_y\t- wielkość planszy w pionie\n\
            ip\t\t- ip serwera\n\
            port\t- port na którym pracuje serwer\n\
            max_graczy\t- maksymalna ilosc graczy w grze\n\
            max_punktów\t- maksymalna ilość punktów w trybe deathmatch, jedak nie mniej niż max_graczy*60\n"<<endl;
            exit(1);
        }
    }

    char ip[128], port[128];
    strcpy(ip,argv[6]);
    strcpy(port,argv[7]);
    Net *server;
    char buf[128];
    srand(time(NULL));
    int MAPA_X, MAPA_Y;

    if (string(argv[1]).compare("create")==0)
    {
        strcpy(ip,argv[6]);
        strcpy(port,argv[7]);
        server=new Net(ip,port);
        MAPA_X=atoi(argv[4]);
        MAPA_Y=atoi(argv[5]);
        int MAX_GRACZY, MAX_PUNKTOW;
        MAX_GRACZY=atoi(argv[8]);
        if (gra==0)
        {
            if (!server->NowaGra(string(argv[2]),MAPA_X,MAPA_Y,0,MAX_GRACZY))
            {
                cout<<"Serwer odmawia utworzenia gry"<<endl;
                exit(0);
            }
        }
        else if (gra==1)
        {
            MAX_PUNKTOW=atoi(argv[9]);
            if (!server->NowaGra(string(argv[2]),MAPA_X,MAPA_Y,1,MAX_GRACZY,MAX_PUNKTOW))
            {
                cout<<"Serwer odmawia utworzenia gry"<<endl;
                exit(0);
            }
            line;
        }
    }
    if (string(argv[1]).compare("connect")==0)
    {
        //sprintf(buf,"%s",argv[2]);
        strcpy(ip,argv[3]);
        strcpy(port,argv[4]);
        server=new Net(ip,port);
        if (!server->Dolacz(string(argv[2])))
        {
            cout<<"Serwer odmawia dolaczenia do gry"<<endl;
            exit(0);
        }
        MAPA_X=server->GetMapaX();
        MAPA_Y=server->GetMapaY();
    }
    line;
    Grafika grafika(800,600,32,server, MAPA_X, MAPA_Y,gra);
    line;
    grafika.Start();

    return 0;
}
