#ifndef GRAFIKA_H
#define GRAFIKA_H

#include <list>
#include "globals.h"
#include "Pole.h"
#include "element_rysowania.hpp"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "gracz.hpp"
#include "net.hpp"
#include "SDL/SDL_ttf.h"
class Grafika
{
public:
    /** Default constructor */
    Grafika(int x, int y, int bpp,Net *nerver, int nMAPA_X, int nMAPA_Y, int ngra);
    /** Default destructor */
    ~Grafika();
    void Rysuj(int x, int y, SDL_Surface* obraz);
    void Petla_rysowania();
    void Start();
    void licz_fps();
    void Informacja(string i);

protected:
private:
    int MAPA_X, MAPA_Y;
    int gra;
    Net *server;
    list <SDL_Surface*> informacje;
    int liczba_informacji;
    SDL_Surface *screen;
    SDL_Surface *load_image(std::string nazwa);
    std::list<element_rysowania> kolejka_rysowania;
    boost::mutex kolejka_muteks;
    boost::mutex dzialaj_muteks;
    boost::mutex fps_muteks;
    boost::mutex klatki_muteks;
    ///POzycja x,y prawego górnego rogu;
    int poz_x, poz_y;
    ///Ile pól sie mieści na ekranie przy aktualnej rozdzielczosci;
    int pola_x, pola_y;
    bool dzialaj;
    SDL_Surface *woda;
    SDL_Surface *ciemna_woda;
    SDL_Surface *gracz;
    SDL_Surface *strzal;
    SDL_Surface *przeciwnik;
    SDL_Surface *widmo;
    SDL_Surface *wrak;

    SDL_Surface *text_pozycja, *text_fps, *text_punkty;
    TTF_Font *font;
    SDL_Color textColor;
    //SDL_Surface *wrog;
    SDL_Event event;

    Gracz player;
    int oczekujace_klatki;
    int time_old;
    int fps;
    int x,y;
    bool lewy,prawy,gora,dol;
    bool czekamy, strzal_czekamy;
    bool gra_wystartowala;
    int strzal_x, strzal_y;
    void wysrodkuj_mape()
    {
        int nprzesx=player.GetX()-pola_x/2;
        int nprzesy=player.GetY()-pola_y/2;
        #ifdef DEBUG
        char debug[256];
        sprintf(debug,"npozx=%d npozy=%d",nprzesx,nprzesy);
        info(debug);
        #endif//DEBUG
        if (nprzesx<0)
            nprzesx=0;
        if (nprzesy<0)
            nprzesy=0;
        if (nprzesx>=MAPA_X-pola_x)
            nprzesy=MAPA_X-pola_y;
        if (nprzesy>=MAPA_Y-pola_y)
            nprzesy=MAPA_Y-pola_y;
        #ifdef DEBUG
        char debu[256];
        sprintf(debu,"poz_x=%d poz_y=%d",nprzesx,nprzesy);
        info(debu);
        #endif//DEBUG
        poz_x=nprzesx;
        poz_y=nprzesy;
    }
    void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

    Pole **mapa;
};

#endif // GRAFIKA_H
