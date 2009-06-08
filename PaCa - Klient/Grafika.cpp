#include "Grafika.hpp"
#include <cstdlib>
#include <cstdio>
#include "version.h"

#include <iostream>
using namespace std;
Grafika::Grafika(int x, int y, int bpp, Net *nserver)
{

    server=nserver;
    gra_wystartowala=false;
    poz_x=poz_y=0;
    oczekujace_klatki=0;
    pola_x=(x/ROZMIAR_X);
    pola_y=(y/ROZMIAR_Y);

    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        exit(1);
    }

    screen = SDL_SetVideoMode( x, y, bpp, SDL_HWSURFACE );
    if ( screen == NULL )
    {
        exit(2);
    }
    char nazwa[256]={0};
    dzialaj=true;
    sprintf(nazwa,"PaCa v%s",AutoVersion::FULLVERSION_STRING);
    SDL_WM_SetCaption( nazwa, NULL );
    woda = load_image( "woda.png" );
    ciemna_woda = load_image( "ciemna_woda.png" );
    strzal=load_image("strzal.png");
    gracz=load_image("gracz.bmp");
    przeciwnik=load_image("przeciwnik.bmp");
    wrak=load_image("wrak.bmp");
    widmo=load_image("widmo.bmp");
    if ( woda == NULL  )
    {
        exit(4);
    }
    if ( ciemna_woda == NULL  )
    {
        exit(5);
    }
    if ( strzal == NULL  )
    {
        exit(6);
    }


    mapa=new Pole*[MAPA_X];
    for (int i=0;i<MAPA_X;++i)
    {
        mapa[i]=new Pole[MAPA_Y];
        for (int j=0;j<MAPA_Y;++j)
            mapa[i][j].Set(i,j);
    }

}

Grafika::~Grafika()
{
    for (int i=0;i<MAPA_X;++i)
    {
        delete[] mapa;
    }
    delete[] mapa;
    //dtor
}

void Grafika::licz_fps()
{
    while (true)
    {
        fps_muteks.lock();
        cout<<fps<<endl;
        fps=0;
        fps_muteks.unlock();
        sleep(1);
    }
}

void Grafika::Petla_rysowania()
{
    //cout<<"Petla rysowania"<<endl;
    element_rysowania tmp;
    dzialaj_muteks.lock();
    while (dzialaj)
    {
        //cout<<"Klatka..."<<endl;
        dzialaj_muteks.unlock();
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        while (true)//(tmp=kolejka_rysowania->pop_front())->GetObraz()!=NULL)
        {

            kolejka_muteks.lock();
            if (kolejka_rysowania.empty())
            {
                kolejka_muteks.unlock();
                usleep(PAUSE_TIME);
                continue;
            }
            tmp=kolejka_rysowania.front();
            kolejka_rysowania.pop_front();
            kolejka_muteks.unlock();
            if (tmp.GetObraz()==NULL)
            {
                klatki_muteks.lock();
                --oczekujace_klatki;
                klatki_muteks.unlock();
                break;
            }

            SDL_Rect offset;
            //Get offsets
            offset.x = tmp.GetX()*ROZMIAR_X;
            offset.y = tmp.GetY()*ROZMIAR_Y;
            //Blit
            SDL_BlitSurface( tmp.GetObraz(), NULL, screen, &offset );

        }
        if ( SDL_Flip( screen ) == -1 )
        {
            exit(3);
        }
        dzialaj_muteks.lock();
    }
    dzialaj_muteks.unlock();


}


void Grafika::Rysuj(int x, int y, SDL_Surface* obraz)
{
    //cout<<"rysuje element na "<<x<<" "<<y<<endl;
    /*if(obraz==NULL)
    {
    kolejka_muteks.lock();
    ++oczekujace_klatki;
    kolejka_rysowania.push_back(element_rysowania(x,y,NULL));
    kolejka_muteks.unlock();
        cout<<oczekujace_klatki<<" klatek oczekuje"<<endl;
    }*/
    kolejka_muteks.lock();
    kolejka_rysowania.push_back(element_rysowania(x,y,obraz));
    kolejka_muteks.unlock();
    if (obraz==NULL)
    {
        kolejka_muteks.lock();
        ++oczekujace_klatki;
        kolejka_muteks.unlock();
    }

}

SDL_Surface *Grafika::load_image( std::string nazwa )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( nazwa.c_str() );
    if ( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        if ( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
        else
        {
            cout<<"Problem z wczytywaniem "<<nazwa<<endl;
        }
    }
    else
    {
        cout<<"Problem z wczytywaniem "<<nazwa<<endl;
    }
    return optimizedImage;
}

void Grafika::Start()
{
    int max_x=pola_x+poz_x;
    int max_y=pola_y+poz_y;
    //cout<<poz_x<<" "<<poz_y<<"---"<<max_x<<" "<<max_y<<endl;
    if (max_x>=MAPA_X)
        max_x=MAPA_X;
    if (max_y>=MAPA_Y)
        max_y=MAPA_Y;
    lewy=prawy=gora=dol=czekamy=false;
    //server->SET(gx,gy);
    boost::thread rysowanie(boost::bind(&Grafika::Petla_rysowania,this));
    boost::thread liczenie_fps(boost::bind(&Grafika::licz_fps,this));

    boost::thread odbieranie(boost::bind(&Net::Odbieracz,server));
    dzialaj_muteks.lock();
    int gx,gy;
    while (dzialaj)
    {
        dzialaj_muteks.unlock();
        usleep(PAUSE_TIME);
        while (!server->WynikiEmpty())
        {
            OdebranyWynikPolecenia wynik=server->GetOdebrane();
            if (wynik.polecenie==6 && wynik.ok==true)
            {
                player.Set(gx,gy);
                wysrodkuj_mape();
            }
            if (wynik.polecenie==1 && wynik.ok==true)
            {
                info("Przesuwam gracza");
                int nx=player.GetX();
                int ny=player.GetY();
                //czekamy=false;
                if (gora==true)
                    player.Set(nx,ny-1);
                else if (dol==true)
                    player.Set(nx,ny+1);
                else if (lewy==true)
                    player.Set(nx-1,ny);
                else if (prawy==true)
                    player.Set(nx+1,ny);
                gora=dol=lewy=prawy=czekamy=false;
                wysrodkuj_mape();
                char tmp[128];
                sprintf(tmp,"Gracz jest na x= %d y= %d",player.GetX(),player.GetY());
                info(tmp);

            }
            else if (wynik.polecenie==10)
            {
                gora=dol=lewy=prawy=czekamy=strzal_czekamy=false;
            }
            else if (wynik.polecenie==11)
            {
                gora=dol=lewy=prawy=czekamy=strzal_czekamy=false;
                gra_wystartowala=true;
            }
            else if (wynik.polecenie==2)
            {
                //line;
                strzal_czekamy=false;
                if (wynik.id_odpowiedzi==200)
                {
                    ;

                }
                else if (wynik.id_odpowiedzi==201)
                {
                    //line;
                    mapa[strzal_x][strzal_y].Set(6);
                }
                else if (wynik.id_odpowiedzi==202)
                {
                    //line;
                    while (!wynik.pozycje.empty())
                    {
                        //line;
                        int tmpx, tmpy;
                        tmpx=wynik.pozycje.front().x;
                        tmpy=wynik.pozycje.front().y;
                        wynik.pozycje.pop_front();
                        mapa[tmpx][tmpy].Set(4);
                        //line;
                    }
                }
                else if (wynik.id_odpowiedzi==203)
                {
                    //line;
                    mapa[strzal_x][strzal_y].Set(6);
                }
                //line;
            }
            else if (wynik.polecenie==8)
            {
                //line;
                char tmp[128];
                sprintf(tmp,"strzelono obok na pozycje x: %d y %d",wynik.x, wynik.y);
                info(tmp);
                mapa[wynik.x][wynik.y].Set(3);
                line;
            }
            if (wynik.polecenie==9 || (wynik.polecenie==1 && wynik.id_odpowiedzi==301))
            {
                for (int i=poz_y;i<pola_y+poz_y;++i)
                {
                    for (int j=poz_x;j<pola_x+poz_x;++j)
                    {
                        if (mapa[j][i].Get()==9)
                        {
                            mapa[j][i].Set(0);
                        }
                    }
                }
                while (!wynik.pozycje.empty())
                {
                    line;
                    int tmpx, tmpy;
                    line;
                    tmpx=wynik.pozycje.front().x;
                    line;
                    tmpy=wynik.pozycje.front().y;
                    line;
                    wynik.pozycje.pop_front();
                    line;
                    char tm[128];
                    sprintf(tm,"Ustawiam x=%d y=%d na %d",tmpx,tmpy,9);
                    info(tm);
                    if (tmpx==0 && tmpy==0)
                        continue;
                    if (tmpx>=MAPA_X || tmpy>=MAPA_Y)
                        continue;
                    mapa[tmpx][tmpy].Set(9);
                    line;
                }
            }
            else if (wynik.polecenie==100)
            {
                cout<<"Brawo, wygrałeś"<<endl;
                exit(0);
            }
            else if (wynik.polecenie==101)
            {
                cout<<"Niestety, zostałeś zniszcozny...\nSpróbuj ponownie"<<endl;
                exit(0);
            }
            //line;
        }
        SDL_Surface *bitmapa;
        //line;
        for (int i=poz_y;i<=pola_y+poz_y;++i)
        {
            for (int j=poz_x;j<=pola_x+poz_x;++j)
            {
                char tmp=mapa[j][i].Get();
                if (tmp==0)
                    bitmapa=ciemna_woda;
                else if (tmp==1)
                    bitmapa=woda;
                else if (tmp==3)
                    bitmapa=strzal;
                else if (tmp==4)
                    bitmapa=widmo;
                else if (tmp==5)
                    bitmapa=przeciwnik;
                else if (tmp==6)
                    bitmapa=wrak;
                else if (tmp==9)
                    bitmapa=przeciwnik;
                Rysuj(j-poz_x,i-poz_y,bitmapa);
            }
        }
        //line;
        //Rysuj(player.GetX()-poz_x,player.GetY()-poz_y,gracz);
        for (int i=0;i<4;++i)
        {
            if (player.stan[i])
                Rysuj(player.pozx[i]-poz_x,player.pozy[i]-poz_y,gracz);
        }
        Rysuj(0,0,NULL);
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_MOUSEMOTION || event.type==SDL_MOUSEBUTTONDOWN )
            {
                for (int i=poz_y;i<pola_y+poz_y;++i)
                {
                    for (int j=poz_x;j<pola_x+poz_x;++j)
                    {
                        {
                            if (mapa[j][i].handle_events(event,poz_x,poz_y)==3)
                            {
                                if (gra_wystartowala)
                                {
                                    server->Strzal(j,i);
                                    strzal_x=j;
                                    strzal_y=i;

                                }
                                else
                                {
                                    server->SET(j,i);
                                    gx=j;
                                    gy=i;
                                }
                            }

                        }
                    }
                }
            }
            else if (event.type==SDL_KEYDOWN && !czekamy)
            {
                if ( event.key.keysym.sym==SDLK_UP )
                {
                    //czekamy=true;
                    if (gra_wystartowala)
                    {
                        server->PrzesunGracza(1);
                        gora=true;
                        lewy=prawy=dol=false;
                    }
                    else
                    {
                        --poz_y;

                    }
                }
                else if ( event.key.keysym.sym==SDLK_DOWN)
                {
                    //czekamy=true;
                    if (gra_wystartowala)
                    {
                        server->PrzesunGracza(3);
                        dol=true;
                        gora=lewy=prawy=false;
                    }
                    else
                    {
                        ++poz_y;
                    }
                }
                else if ( event.key.keysym.sym==SDLK_LEFT)
                {
                    //czekamy=true;
                    if (gra_wystartowala)
                    {
                        server->PrzesunGracza(4);
                        lewy=true;
                        gora=dol=prawy=false;
                    }
                    else
                    {
                        --poz_x;
                    }
                }
                else if ( event.key.keysym.sym==SDLK_RIGHT)
                {
                    //czekamy=true;
                    if (gra_wystartowala)
                    {
                        server->PrzesunGracza(2);
                        prawy=true;
                        gora=dol=lewy=false;
                    }
                    else
                    {
                        ++poz_x;
                    }
                }
                if (poz_y<0)
                    poz_y=0;
                else if (poz_y>=MAPA_Y)
                    poz_y=MAPA_Y-1;
                else if (poz_x<0)
                    poz_x=0;
                else if (poz_x>=MAPA_X)
                    poz_x=MAPA_X-1;

            }
            else if ( event.type == SDL_QUIT )  //Quit the program quit = true; }
            {
                exit(0);

            }
        }



        klatki_muteks.lock();
        while (oczekujace_klatki>2)
        {
            klatki_muteks.unlock();
            usleep(PAUSE_TIME);
            klatki_muteks.lock();
            continue;
        }
        klatki_muteks.unlock();
        dzialaj_muteks.lock();
        fps_muteks.lock();
        ++fps;
        fps_muteks.unlock();
    }
    dzialaj_muteks.unlock();
}
