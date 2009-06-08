#ifndef NET_HPP
#define NET_HPP

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <string>
#include "gracz.hpp"
#include <list>
#include "debug.hpp"
#include "globals.h"
using namespace std;
using boost::asio::ip::tcp;
//using namespace boost::asio;

class OdebranyWynikPolecenia
{
public:

    /**Do którego polecenia odnosi się ta odpowiedź
    polecenia:
    1 - przesuniece gracza
    2 - strzal
    3 - statek gracza zostal trafiony
    4 - lista gier
    5 - punktacja
    6 - ustawienie gracza
    7 - prosba o pozycje gracza
    8 - wrogi strzal w poblizu
    9 - inny gracz w poblizu
    10 - błąd
    11 - Start
    100 - zwycęstwo
    101 - przegrana
    */
    int polecenie;
    int kod_bledu;
    int id_odpowiedzi,x,y;
    list<Pozycja> pozycje;
    /** Dla:
        Listy gier
        Punktacji
    */
    string informacje;
    bool ok;
    //OdebranyWynikPolecenia( int polecenie, int id_odpowiedzi, int x=0, int y=0, list<Pozycja> pozycje=list<Pozycja>(), string informacje="", bool ok=true);
    OdebranyWynikPolecenia( int polecenie, int id_odpowiedzi, int x=0,\
                            int y=0, list<Pozycja> pozycje=list<Pozycja>(), string informacje="", bool ok=true):\
            polecenie(polecenie),id_odpowiedzi(id_odpowiedzi), x(x),y(y),pozycje(pozycje),informacje(informacje), ok(ok)
    {
        ;
    };
    OdebranyWynikPolecenia(int polecenie,bool ok=false):polecenie(polecenie),kod_bledu(kod_bledu),ok(ok)
    {
        ;
    };
    OdebranyWynikPolecenia()
    {
        ;
    };

};



class Net
{
private:
    boost::asio::io_service io_service;
    tcp::resolver *resolver;
    tcp::resolver::query *query;
    tcp::resolver::iterator iterator;
    tcp::socket *s;
    int id_klienta;
    list<string> polecenia_oczekujace;
    list<OdebranyWynikPolecenia>wyniki_zapytan;
    boost::mutex oczekujace;
    boost::mutex odebrane;
    boost::mutex numery;
    int id_polecen[1024];
    int losuj_id_polecenia(int polecenie)
    {
        int id_polecenia_tmp=0;
        numery.lock();
        while (id_polecenia_tmp==0 && id_polecen[id_polecenia_tmp]!=0)
            id_polecenia_tmp=rand()%1024;
        id_polecen[id_polecenia_tmp]=polecenie;
        numery.unlock();
        return id_polecenia_tmp;
    }
public:
    Net(char *ip, char *port)
    {
        try
        {

            resolver=new tcp::resolver(io_service);
            query=new tcp::resolver::query(tcp::v4(), ip, port);

            iterator= resolver->resolve(*query);
            s=new tcp::socket(io_service);
            s->connect(*iterator);
        }
        catch (exception &e)
        {
            cout<<"Błąd: "<<e.what()<<endl;
            exit(3);
        }
        for (int i=0;i<1024;++i)
        {
            id_polecen[i]=-1;
        }
    }

    bool Send(string dane);
    bool PrzesunGracza(char kierunek);
    bool Strzal(int x, int y);
    bool NowaGra(std::string nazwa_gry, int plansza_x, int plansza_y,\
                 char tryb_gry, int max_graczy);
    bool NowaGra(std::string nazwa_gry, int plansza_x, int plansza_y,\
                 char tryb_gry, int max_graczy, int max_punktow);
    bool Dolacz(std::string nazwa_gry);
    bool Lista();
    bool SET(int x, int y);
    bool PTS();
    bool Get();

    OdebranyWynikPolecenia GetOdebrane()
    {
        OdebranyWynikPolecenia wynik;
        odebrane.lock();
        if (!wyniki_zapytan.empty())
        {
            wynik=wyniki_zapytan.front();
            wyniki_zapytan.pop_front();
        }
        odebrane.unlock();
        return wynik;
    }

    bool WynikiEmpty()
    {
        bool wynik;
        odebrane.lock();
        wynik=wyniki_zapytan.empty();
        odebrane.unlock();
        return wynik;

    }

    void Odbieracz();
    string Odbierz();




};

#endif//NET_HPP
