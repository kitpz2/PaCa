#ifndef GRACZ_HPP
#define GRACZ_HPP

class Pozycja
{
    public:
        int x,y;
        Pozycja(int x, int y):x(x),y(y){};
};

class Gracz
{
    ///Pozycja gracza (współżędne na mapie);
    private:
        int x,y;
    public:
        Gracz(int x=0, int y=0):x(x),y(y){;};
        void Set(int nx, int ny){x=nx;y=ny;};


        int& GetX(){return x;};
        int& GetY(){return y;};

};

class Przeciwnik
{
    public:
        int x, y;
};


#endif//GRACZ_HPP
