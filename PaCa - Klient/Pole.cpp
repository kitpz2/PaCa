#include "Pole.h"

Pole::Pole(int x, int y, char typ):x(x),y(y),typ(typ)
{
    int *global;
    polozenie.x=x*ROZMIAR_X;
    polozenie.y=y*ROZMIAR_Y;
    polozenie.w=ROZMIAR_X;
    polozenie.h=ROZMIAR_Y;

    //wyswietlany=NULL;
}

Pole::~Pole()
{
    //dtor
}

int Pole::handle_events(SDL_Event& event, int przesuniecie_x, int przesuniecie_y)
{
    if ( event.type == SDL_MOUSEMOTION )
    {
        x = event.motion.x+(przesuniecie_x*ROZMIAR_X);
        y = event.motion.y+(przesuniecie_y*ROZMIAR_Y);

        if ( ( x > polozenie.x ) && ( x < polozenie.x + polozenie.w ) && ( y > polozenie.y ) && ( y < polozenie.y + polozenie.h ) )
        {
            if (typ==0)
            {
                typ=1;
            }
        }
        else
        {
            if (typ==1)
            {
                typ=0;
            }
        }
    }

    if ( event.type == SDL_MOUSEBUTTONDOWN )
    {
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
            x = event.button.x+(przesuniecie_x*ROZMIAR_X);
            y = event.button.y+(przesuniecie_y*ROZMIAR_Y);
            //cout<<(x/ROZMIAR_X)+1<<" "<<(y/ROZMIAR_Y)+1<<endl;
            if ( ( x > polozenie.x ) && ( x < polozenie.x + polozenie.w ) && ( y > polozenie.y ) && ( y <polozenie.y + polozenie.h ) )
            {
                /*if(typ!=3)
                {
                    typ=3;
                }
                else if(typ==3)
                {
                    typ=1;
                }*/
                return 3;
                //klikniety();
            }
        }
    }

}
