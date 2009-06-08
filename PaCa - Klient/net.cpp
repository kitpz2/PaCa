#include "net.hpp"
#include <cstdlib>
#include <cstdio>
bool Net::Send(string dane)
{
    try
    {
        info2("Wysylam",dane.c_str());
        write(*s,boost::asio::buffer(dane),boost::asio::transfer_all());
    }
    catch (exception &e)
    {
        cout<<"Wystąpił błąd wysyłania danych! e.what()"<<endl;
        exit(2);
    }
    return true;
}

bool Net::PrzesunGracza(char kierunek)
{
    if (kierunek<1 && kierunek > 4)
        return true;
    else
    {

        char buf[128];
        int pol_tmp=losuj_id_polecenia(1);

        sprintf(buf,"MOV %d %d %d#",pol_tmp,id_klienta,int(kierunek));
        oczekujace.lock();
        polecenia_oczekujace.push_back(string(buf));
        oczekujace.unlock();
    }
    return true;
}

bool Net::Strzal(int x, int y)
{
    char buf[128];
    int pol_tmp=losuj_id_polecenia(2);

    sprintf(buf,"SHO %d %d %d %d#",pol_tmp,id_klienta,x,y);
    oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();
    return true;
}
bool Net::NowaGra(std::string nazwa_gry, int plansza_x, int plansza_y,\
                  char tryb_gry, int max_graczy)
{
    char buf[128];
    //int pol_tmp=losuj_id_polecenia(1);

    sprintf(buf,"CRE %s %d %d %d %d %s#",nazwa_gry.c_str(),plansza_x, plansza_y,tryb_gry,max_graczy,"0.5");
    /*oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();*/
    Send(string(buf));
    string temp=Odbierz();
    string t1=temp.substr(0,2);
    info(t1.c_str());
    if (t1.compare("OK")==0)
    {
        size_t p1=0,p2=0;
        p1=temp.find(' ',p2)+1;
        p2=temp.find(' ',p1);
        t1=temp.substr(p1,p2);
        info(t1.c_str());
        id_klienta = atoi(t1.c_str());
    }
    else
        return false;

    return true;
}
bool Net::Dolacz(std::string nazwa_gry)
{
    char buf[128];
    sprintf(buf,"CON %s %s#",nazwa_gry.c_str(),"0.5");
    Send(string(buf));
    string temp=Odbierz();
    string t1=temp.substr(0,2);
    info(t1.c_str());
    if (t1.compare("OK")==0)
    {
        size_t p1=0,p2=0;
        p1=temp.find(' ',p2)+1;
        p2=temp.find(' ',p1);
        t1=temp.substr(p1,p2);
        info(t1.c_str());
        id_klienta = atoi(t1.c_str());
        cout<<"Dolacozno"<<endl;
        return true;
    }
    else
        return false;
    return true;
}
bool Net::Lista()
{
    char buf[128];
    sprintf(buf,"LST#");
    oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();
    return true;

}
bool Net::SET(int x, int y)
{
    char buf[128];
    int pol_tmp=losuj_id_polecenia(6);

    sprintf(buf,"SET %d %d %d %d %d %d %d %d %d %d#",pol_tmp,id_klienta,x,y,x+1,y,x,y+1,x+1,y+1);
    oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();
    return true;
}

bool Net::PTS()
{
    char buf[128];
    int pol_tmp=losuj_id_polecenia(5);

    sprintf(buf,"PTS %d %d#",pol_tmp,id_klienta);
    oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();
    return true;
}
bool Net::Get()
{
    char buf[128];
    int pol_tmp=losuj_id_polecenia(7);

    sprintf(buf,"GET %d %d#",pol_tmp,id_klienta);
    oczekujace.lock();
    polecenia_oczekujace.push_back(string(buf));
    oczekujace.unlock();
    return true;
}


void Net::Odbieracz()
{
    while (true)
    {
        oczekujace.lock();
        if (!polecenia_oczekujace.empty())
        {
            Send(polecenia_oczekujace.front());
            polecenia_oczekujace.pop_front();

        }
        oczekujace.unlock();
        if (s->available()>0)
        {
            string temp=Odbierz();
            string t1=temp.substr(0,temp.find(' '));
            info2("Porównuję",t1.c_str());
            if (t1.compare("OK")==0)
            {
                size_t p1=0,p2=0;
                p1=temp.find(' ',p2)+1;
                p2=temp.find(' ',p1);
                t1=temp.substr(p1,p2);
                info(t1.c_str());
                int id_polecenia = atoi(t1.c_str());
                numery.lock();
                int polecenie=id_polecen[id_polecenia];
                id_polecen[id_polecenia]=0;
                numery.unlock();
                if (polecenie==1)
                {
                    p1=temp.find(' ',p2)+1;
                    p2=temp.find(' ',p1);
                    t1=temp.substr(p1,p2);
                    info(t1.c_str());
                    int nr_odpowiedzi = atoi(t1.c_str());
                    if (nr_odpowiedzi==300)
                    {
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(1,300));
                        odebrane.unlock();
                    }
                    if (nr_odpowiedzi==301)
                    {
                        list<Pozycja> pozycje;
                        while (p2!=string::npos)
                        {
                            p1=temp.find(' ',p2)+1;
                            p2=temp.find(' ',p1);
                            t1=temp.substr(p1,p2);
                            info(t1.c_str());
                            int x = atoi(t1.c_str());

                            p1=temp.find(' ',p2)+1;
                            p2=temp.find(' ',p1);
                            t1=temp.substr(p1,p2);
                            info(t1.c_str());
                            int y = atoi(t1.c_str());
                            pozycje.push_back(Pozycja(x,y));
                        }
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(1,301,0,0,pozycje));
                        odebrane.unlock();
                    }


                }
                else if (polecenie==2)
                {
                    p1=temp.find(' ',p2)+1;
                    p2=temp.find(' ',p1);
                    t1=temp.substr(p1,p2);
                    info(t1.c_str());
                    int nr_odpowiedzi = atoi(t1.c_str());
                    if (nr_odpowiedzi==200)
                    {
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(2,200));
                        odebrane.unlock();
                    }
                    else if (nr_odpowiedzi==201)
                    {
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(2,201));
                        odebrane.unlock();
                    }
                    else if (nr_odpowiedzi==202)
                    {
                        list<Pozycja> pozycje;
                        temp[temp.size()-1]=' ';
                        while (p2!=string::npos)
                        {
                            p1=temp.find(' ',p2)+1;
                            p2=temp.find(' ',p1)-1;
                            t1=temp.substr(p1,p2);
#ifdef DEBUG
                            char debu[128];
                            sprintf(debu,"Wyciętę od %d do %d",p1,p2);
                            info2(debu,t1.c_str());
#endif
                            if (t1.empty())
                            {
                                break;
                            }

                            int x = atoi(t1.c_str());

                            p1=temp.find(' ',p2)+1;
                            p2=temp.find(' ',p1);
                            t1=temp.substr(p1,p2);
                            info(t1.c_str());

                            int y = atoi(t1.c_str());
                            pozycje.push_back(Pozycja(x,y));
#ifdef DEBUG
                            char tmp[128];
                            sprintf(tmp,"dodane: x=%d y=%d",x,y);
                            info(tmp);

#endif//DEBUG
                        }
                        line
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(2,202,0,0,pozycje));
                        odebrane.unlock();
                        line
                    }
                    else if (nr_odpowiedzi==203)
                    {
                        odebrane.lock();
                        wyniki_zapytan.push_back(OdebranyWynikPolecenia(2,203));
                        odebrane.unlock();
                    }

                }
                else if (polecenie==6)
                {
                    odebrane.lock();
                    wyniki_zapytan.push_back(OdebranyWynikPolecenia(6,0));
                    odebrane.unlock();
                }
            }
            else if (t1.compare("NO")==0)
            {
                odebrane.lock();
                wyniki_zapytan.push_back(OdebranyWynikPolecenia(10));
                odebrane.unlock();
                info("Błąd od serwera")
            }
            else if (t1.substr(0,t1.size()-1).compare("START")==0)
            {
                odebrane.lock();
                wyniki_zapytan.push_back(OdebranyWynikPolecenia(11));
                odebrane.unlock();
            }
            else if (t1.compare("CLS")==0)
            {
                size_t p1=0,p2=0;
                p1=temp.find(' ',p2)+1;
                p2=temp.find(' ',p1);
                t1=temp.substr(p1,p2);
                info(t1.c_str());
                while (p2!=string::npos)
                {
                    p1=temp.find(' ',p2)+1;
                    p2=temp.find(' ',p1);
                    t1=temp.substr(p1,p2);
                    info(t1.c_str());
                    int x = atoi(t1.c_str());

                    p1=temp.find(' ',p2)+1;
                    p2=temp.find(' ',p1);
                    t1=temp.substr(p1,p2);
                    info(t1.c_str());

                    int y = atoi(t1.c_str());
                    odebrane.lock();
                    wyniki_zapytan.push_back(OdebranyWynikPolecenia(8,x,y));
                    odebrane.unlock();
#ifdef DEBUG
                    char tmp[128];
                    sprintf(tmp,"dodane: x=%d y=%d",x,y);
                    info(tmp);

#endif//DEBUG

                }

            }
        }

        //}
        //oczekujace.unlock();
    }
}

string Net::Odbierz()
{
    string wynik;
    boost::asio::streambuf b;
    int przeczytano;
    try
    {
        przeczytano=boost::asio::read_until(*s, b, "#");
    }
    catch (const std::exception &e)
    {
        fprintf(stderr,"Wystąpił wyjątek przy czytaniu z socketa: %s\n",e.what());
        exit(0);
    }
    std::istream buf(&b);
    char *buf_c=new char[przeczytano+1];
    buf_c[przeczytano]=0;

    buf.read(buf_c,przeczytano);
    info2("odebrano",buf_c)
    string temp=buf_c;
    return temp;
}
