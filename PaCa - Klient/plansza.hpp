#ifndef PLANSZA_HPP
#define PLANSZA_HPP
#include "Pole.h"
class Plansza
{
    private:
        Pole **pole;
        unsigned int x,y;
    public:
        inline Pole* Get(unsigned int nx, unsigned int ny)
        {
            if(nx>=0 && nx<=x && ny>=0 && ny<=y)
                return &pole[x][y];
            else
                return NULL;
        }
        inline void Set(unsigned int nx, unsigned int ny, char typ)
        {
            if(nx>=0 && nx<=x && ny>=0 && ny<=y)
                pole[nx][ny].Set(typ);
        }
}
#endif//PLANSZA_HPP
