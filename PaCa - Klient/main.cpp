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
    if (argc<5)
    {
        cout<<"Podaj ip i port servera"<<endl;
        exit(1);
    }
    char ip[128], port[128];
    strcpy(ip,argv[3]);
    strcpy(port,argv[4]);
    Net server(ip,port);
    char buf[128];
    srand(time(NULL));
    if (string(argv[1]).compare("create")==0)
    {
        sprintf(buf,"%s",argv[2]);
        if (!server.NowaGra(string(buf),MAPA_X,MAPA_Y,0,10))
        {
            cout<<"Serwer odmawia utworzenia gry"<<endl;
            exit(0);
        }
    }
    if (string(argv[1]).compare("connect")==0)
    {
        sprintf(buf,"%s",argv[2]);
        if (!server.Dolacz(string(buf)))
        {
            cout<<"Serwer odmawia dolaczenia do gry"<<endl;
            exit(0);
        }
    }
    Grafika grafika(640,480,32,&server);
    grafika.Start();

    return 0;
}
