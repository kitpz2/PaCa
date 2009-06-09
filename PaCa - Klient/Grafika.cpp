#include "Grafika.hpp"
#include <cstdlib>
#include <cstdio>
#include "version.h"

#include <iostream>
using namespace std;
Grafika::Grafika(int nx, int ny, int bpp, Net *nserver, int nMAPA_X, int nMAPA_Y, int ngra)
{
    line;
    gra=ngra;
    MAPA_X=nMAPA_X;
    MAPA_Y=nMAPA_Y;
    server=nserver;
    text_pozycja=text_fps=text_punkty=NULL;
    x=nx;
    y=ny;
    gra_wystartowala=false;
    poz_x=poz_y=0;
    oczekujace_klatki=0;
    pola_x=(x/ROZMIAR_X);
    pola_y=(y/ROZMIAR_Y);

    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        cout<<"Błąd inicjalizacji SDL"<<endl;
        exit(1);
    }

    screen = SDL_SetVideoMode( x, y, bpp, SDL_HWSURFACE );
    if ( screen == NULL )
    {
        cout<<"BŁĄÐ inicjalizacji ekranu"<<endl;
        exit(2);
    }
    if ( TTF_Init() == -1 )
    {
        cout<<"BŁAD inicjalizacji TTF"<<endl;
        exit(3);
    }

    font = TTF_OpenFont( "arial.ttf", 14 );
    if ( font == NULL )
    {
        cout<<"BŁĄD wczytywania czcionki"<<endl;
        exit(4);
    }
    line;
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
    line;
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

    line;
    mapa=new Pole*[MAPA_X];
    line;
    for (int i=0;i<MAPA_X;++i)
    {
        line;
        mapa[i]=new Pole[MAPA_Y];
        line;
        for (int j=0;j<MAPA_Y;++j)
        {
            line;
            mapa[i][j].Set(i,j);
            line;
        }
        line;
    }
    line;
    char xyz[128];
    sprintf(xyz,"%d, Punkty: %d",0, 0);
    info(xyz);
    text_punkty=TTF_RenderText_Solid( font, xyz, textColor );
    line;

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
        //cout<<fps<<endl;
        char i[32];
#ifdef DEBUG
        sprintf(i,"FPS: %d",fps);
        text_fps=TTF_RenderText_Solid( font, i, textColor );
#endif//DEBUG
        fps=0;
        fps_muteks.unlock();
        sleep(1);
    }
}

void Grafika::apply_surface( int tx, int ty, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = tx;
    offset.y = ty;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
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
        if (text_pozycja!=NULL)
            apply_surface( 0, y-40, text_pozycja, screen );
        if (text_fps!=NULL)
            apply_surface( 0, 0, text_fps, screen );
        if (gra==1 && text_pozycja!=NULL)
        {
            apply_surface( y/2-100, 0, text_punkty, screen );
        }
        /*if(text_info!=NULL)
            apply_surface(x-200, y-100, text_info, screen );*/
        list<SDL_Surface*>::iterator it;
        int n=1;
        for (it=informacje.begin();it!=informacje.end();it++)
        {
            apply_surface(x-200, y-((n)*20), *it, screen );
            ++n;
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
    textColor.r=255;
    textColor.g=255;
    textColor.b=255;
    //line;
    liczba_informacji=0;
    int max_x=pola_x+poz_x;
    int max_y=pola_y+poz_y;
    //line;
    //cout<<poz_x<<" "<<poz_y<<"---"<<max_x<<" "<<max_y<<endl;
    if (max_x>=MAPA_X)
        max_x=MAPA_X;
    if (max_y>=MAPA_Y)
        max_y=MAPA_Y;
    //line;
    lewy=prawy=gora=dol=czekamy=false;
    //server->SET(gx,gy);
    boost::thread rysowanie(boost::bind(&Grafika::Petla_rysowania,this));
    boost::thread liczenie_fps(boost::bind(&Grafika::licz_fps,this));

    boost::thread odbieranie(boost::bind(&Net::Odbieracz,server));
    //line;
    dzialaj_muteks.lock();
    int gx,gy;
    while (dzialaj)
    {
        //line;
        dzialaj_muteks.unlock();
        //usleep(PAUSE_TIME);
        while (!server->WynikiEmpty())
        {
            OdebranyWynikPolecenia wynik=server->GetOdebrane();
            if (wynik.polecenie==7)
            {
                player.Set(gx,gy);
                //gra_wystartowala=true;
                wysrodkuj_mape();
                Informacja("Respawn!");
                char tmpp[128];
                sprintf(tmpp,"POZYCJA: %d,%d",player.GetX(),player.GetY());
                text_pozycja = TTF_RenderText_Solid( font, tmpp, textColor );
            }
            if (wynik.polecenie==6 && wynik.ok==true)
            {
                player.Set(gx,gy);
                gra_wystartowala=true;
                wysrodkuj_mape();
                Informacja("Gra sie rozpoczyna!");
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
                char tmpp[128];
                sprintf(tmpp,"POZYCJA: %d,%d",player.GetX(),player.GetY());
                text_pozycja = TTF_RenderText_Solid( font, tmpp, textColor );
                //info(tmp);

            }
            else if (wynik.polecenie==10)
            {
                gora=dol=lewy=prawy=czekamy=strzal_czekamy=false;
                if (gra_wystartowala==false)
                {
                    if (wynik.kod_bledu==100)
                    {
                        gra_wystartowala=true;
                        Informacja("Gra sie rozpoczyna!");
                    }
                }
            }
            else if (wynik.polecenie==11)
            {
                gora=dol=lewy=prawy=czekamy=strzal_czekamy=false;
                gra_wystartowala=true;
                Informacja("Gra sie rozpoczyna!");
            }
            else if (wynik.polecenie==3)
            {
                for (int i=0;i<4;++i)
                {
                    if (player.pozx[i]==wynik.x && player.pozy[i]==wynik.y)
                    {
                        player.stan[i]=false;
                        Informacja("Twoj statek zostal trafiony!");
                    }
                }
                if(player.stan[0]==false &&
                    player.stan[1]==false&&
                    player.stan[2]==false&&
                    player.stan[3]==false&& gra==1)
                    {
                        server->Get();
                        Informacja("Zginąłeś, za chwile nastąpi respawn!");
                    }
            }
            else if (wynik.polecenie==2)
            {
                ////line;
                strzal_czekamy=false;
                if (wynik.id_odpowiedzi==200)
                {
                    Informacja("Pudlo");

                }
                else if (wynik.id_odpowiedzi==201)
                {
                    ////line;
                    Informacja("Trafiles w statek przeciwnika");
                    mapa[strzal_x][strzal_y].Set(6);
                    if(gra==1)
                        server->PTS();
                }
                else if (wynik.id_odpowiedzi==202)
                {
                    ////line;
                    Informacja("Pudlo. Ale w poblizu wykryto satek!");
                    //Informacja(string(t));
                    while (!wynik.pozycje.empty())
                    {
                        ////line;
                        int tmpx, tmpy;
                        tmpx=wynik.pozycje.front().x;
                        tmpy=wynik.pozycje.front().y;
                        wynik.pozycje.pop_front();
                        if ((tmpx==0 && tmpx==0) || (tmpx>=MAPA_X || tmpy>=MAPA_Y))
                            continue;
                        mapa[tmpx][tmpy].Set(4);
                        char t[128];
                        //sprintf(t,"Statek na %d,%d!",tmpx,tmpy);

                        ////line;
                    }
                }
                else if (wynik.id_odpowiedzi==203)
                {
                    ////line;
                    Informacja("Statek przeciwnika zatopiony!");
                    mapa[strzal_x][strzal_y].Set(6);
                    if(gra==1)
                        server->PTS();
                }
                ////line;
            }
            else if (wynik.polecenie==8)
            {
                ////line;
                char tmp[128];
                sprintf(tmp,"strzelono obok na pozycje %d,%d",wynik.x, wynik.y);
                info(tmp);
                Informacja(string(tmp));
                mapa[wynik.x][wynik.y].Set(3);
                //line;
            }
            else if (wynik.polecenie==5)
            {
                ////line;
                char tmp[128];
                sprintf(tmp,"%d, Punkty: %d",wynik.y, wynik.x);
                info(tmp);
                text_punkty=TTF_RenderText_Solid( font, tmp, textColor );
                //line;
            }
            if (wynik.polecenie==9 || (wynik.polecenie==1 && wynik.id_odpowiedzi==301))
            {
                Informacja(string("Wykryto w poblizu wroga!"));
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
                    //line;
                    int tmpx, tmpy;
                    //line;
                    tmpx=wynik.pozycje.front().x;
                    //line;
                    tmpy=wynik.pozycje.front().y;
                    //line;
                    wynik.pozycje.pop_front();
                    //line;
                    //char tm[128];
                    //info(tm);
                    //Informacja(string(tm));
                    if (tmpx==0 && tmpy==0)
                        continue;
                    if (tmpx>=MAPA_X || tmpy>=MAPA_Y)
                        continue;
                    //sprintf(tm,"Wykryto wroga na %d,%d",tmpx,tmpy);
                    mapa[tmpx][tmpy].Set(9);
                    //line;
                }
            }
            else if (wynik.polecenie==100)
            {
                cout<<"Brawo, wygrałeś"<<endl;
                exit(0);
            }
            else if (wynik.polecenie==101)
            {
                cout<<"Niestety, zostales zniszcozny... Sprobuj ponownie"<<endl;
                exit(0);
            }
            //line;
        }
        SDL_Surface *bitmapa;
        //line;
        for (int i=poz_y;i<=pola_y+poz_y;++i)
        {
            //line
            for (int j=poz_x;j<=pola_x+poz_x;++j)
            {

                //line
#ifdef DEBUG

                char t[256];
                sprintf(t,"j=%d, i%d, poz_x=%d poz_y=%d, pola_x=%d, pola_y=%d, MAPA_X=%d MAPA_Y=%d, ",j,i,\
                        poz_x, poz_y, pola_x, pola_y, MAPA_X, MAPA_Y);
                //info(t);
#endif//DEBUG
                char tmp=mapa[j][i].Get();
                //line
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
                //line
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
                else if (poz_y>=MAPA_Y-pola_y)
                    poz_y=(MAPA_Y-pola_y)-1;

                else if (poz_x<0)
                    poz_x=0;
                else if (poz_x>=MAPA_X-pola_x)
                    poz_x=(MAPA_X-pola_x)-1;
                if (!gra_wystartowala)
                {
                    char tmp[128];
                    sprintf(tmp,"POZYCJA: %d,%d",poz_x,poz_y);
                    text_pozycja = TTF_RenderText_Solid( font, tmp, textColor );
                }
                //cout<<"poz_x="<<poz_x<<" poz_y="<<poz_y<<endl;

            }
            else if ( event.type == SDL_QUIT )  //Quit the program quit = true; }
            {
                exit(0);

            }
        }

        /*if(informacje.size()!=liczba_informacji)
        {
            while(informacje.size()>15)
            {
                informacje.pop_front();
            }
            list<string>::iterator it;
            string temp;
            for(it=informacje.begin();it!=informacje.end();it++)
            {
                temp.append(*it);
                temp.append("\r\n");
            }
            text_info = TTF_RenderText_Solid( font, temp.c_str(), textColor );
            liczba_informacji=informacje.size();
        }*/
        //line
        klatki_muteks.lock();
        while (oczekujace_klatki>2)
        {
            klatki_muteks.unlock();
            usleep(PAUSE_TIME);
            klatki_muteks.lock();
            continue;
        }
        //line;
        klatki_muteks.unlock();
        dzialaj_muteks.lock();
        fps_muteks.lock();
        ++fps;
        fps_muteks.unlock();
    }
    dzialaj_muteks.unlock();
}

void Grafika::Informacja(string i)
{
    informacje.push_back(TTF_RenderText_Solid( font, i.c_str(), textColor ));
    while (informacje.size()>15)
        informacje.pop_front();
}
