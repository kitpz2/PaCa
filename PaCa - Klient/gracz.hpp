#ifndef GRACZ_HPP
#define GRACZ_HPP
/**Klasa przechowująca pozycje, wykożystywana w róznych miejscach*/
class Pozycja
{
public:
    /**pozycja x i pozycja y na mapie*/
    int x,y;
    /**konstruktor
    \param x pozycja x
    \param y pozycja y
    */
    Pozycja(int x, int y):x(x),y(y) {};
};
/**Klasa przechoująca podstawowe informacje o graczu*/
class Gracz
{
private:
    /**Pozycja gracza*/
    int x,y;

public:
    /**Pozycja poszczególnych kawałków gracza (X)*/
    int pozx[4];
    /**Pozycja poszczególnych kawałków gracza (Y)*/
    int pozy[4];
    /**Przechowuje informacje o stanie danego kwadratu gracza*/
    bool stan[4];

    Gracz(int x=0, int y=0):x(x),y(y)
    {
        stan[0]=stan[1]=stan[2]=stan[3]=true;

        pozx[0]=x;
        pozy[0]=y;

        pozx[1]=x+1;
        pozy[1]=y;

        pozx[2]=x;
        pozy[2]=y+1;

        pozx[3]=x+1;
        pozy[3]=y+1;
    };
    void Set(int nx, int ny)
    {
        x=nx;
        y=ny;
        pozx[0]=x;
        pozy[0]=y;

        pozx[1]=x+1;
        pozy[1]=y;

        pozx[2]=x;
        pozy[2]=y+1;

        pozx[3]=x+1;
        pozy[3]=y+1;
    };

    /**Zeraca referencję do lewego górnego kwadratu gracza
    \return referencja do współżędnej X
    */
    int& GetX()
    {
        return x;
    };
    /**Zeraca referencję do lewego górnego kwadratu gracza
    \return referencja do współżędnej Y
    */
    int& GetY()
    {
        return y;
    };

};
///A to do czegoś miało być ale już nie jest :P
class Przeciwnik
{
public:
    int x, y;
};


#endif//GRACZ_HPP
