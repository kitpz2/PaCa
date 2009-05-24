using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    class KodyBledu
    {
        static int Nieustawiony = -1;
        static int BlednePolecenie = 100;
        static int NiemozliwyRuch=101;
        static int NiemozliwyStrzal=102;
        static int NieoczekiwanyBlad=103;
        static int NieobslugiwanaWersja=104;
        static int NiemozliweUtworzenieGry=105;
        static int BrakGry=106;
        static int ZaczetaNormalnaGra=107;
        static int MaksymalnaLiczbaGraczy=108;
        static int ZaDuzoGraczyNaPlansze=109;
        static int BlednyRozmiarPlanszy = 110;
        static int ZaMaloGraczy=111;
        static int ZaMaloPunktow=112;
        static int BlednePolozenieStartowe=113;
        static int BledneIdKlienta = 114;
        public static int bledneIdKlienta
        {
            get
            {
                return BledneIdKlienta;
            }
        }
        public static int blednePolozenieStartowe
        {
            get
            {
                return BlednePolozenieStartowe;
            }
        }
        public static int zaMaloPunktow
        {
            get
            {
                return ZaMaloPunktow;
            }
        }
        public static int zaMaloGraczy
        {
            get
            {
                return ZaMaloGraczy;
            }
        }
        public static int blednyRozmiarPlanszy
        {
            get
            {
                return BlednyRozmiarPlanszy;
            }
        }
        public static int nieustawiony
        {
            get
            {
                return Nieustawiony;
            }
        }
        public static int blednePolecenie
        {
            get
            {
                return BlednePolecenie;
            }
        }
        public static int niemozliwyRuch
        {
            get
            {
                return NiemozliwyRuch;
            }
        }
        public static int niemozliwyStrzal
        {
            get
            {
                return NiemozliwyStrzal;
            }
        }
        public static int nieoczekiwanyBlad
        {
            get
            {
                return NieoczekiwanyBlad;
            }
        }
        public static int nieobslugiwanaWersja
        {
            get
            {
                return NieobslugiwanaWersja;
            }
        }
        public static int niemozliweUtworzenieGry
        {
            get
            {
                return NiemozliweUtworzenieGry;
            }
        }
        public static int brakGry
        {
            get
            {
                return BrakGry;
            }
        }
        public static int zaczetaNormalnaGra
        {
            get
            {
                return ZaczetaNormalnaGra;
            }
        }
        public static int maksymalnaLiczbaGraczy
        {
            get
            {
                return MaksymalnaLiczbaGraczy;
            }
        }
        public static int zaDuzoGraczyNaPlansze
        {
            get
            {
                return ZaDuzoGraczyNaPlansze;
            }
        }
    }
    class IdOdpowiedzi
    {
        static int Nieustawiony = -1;
        static int ChybionyStrzal = 200;
        static int Trafiony=201;
        static int PudloBliskoStatku=202;
        static int Zatopiony = 203;
        public static int zatopiony
        {
            get
            {
                return Zatopiony;
            }
        }
        public static int nieustawiony
        {
            get
            {
                return Nieustawiony;
            }
        }
        public static int chybionyStrzal
        {
            get
            {
                return ChybionyStrzal;
            }
        }
        public static int trafiony
        {
            get
            {
                return Trafiony;
            }
        }
        public static int pudloBliskoStatku
        {
            get
            {
                return PudloBliskoStatku;
            }
        }

    }
    class NumerOdpowiedzi
    {
        static int Nieustawiony = -1;
        static int MozliwyRuchBrakStatku = 300;
        static int MozliwyRuchStatekWpoblizu=301;
        public static int nieustawiony
        {
            get
            {
                return Nieustawiony;
            }
        }
        public static int mozliwyRuchBrakStatku
        {
            get
            {
                return MozliwyRuchBrakStatku;
            }
        }
        public static int mozliwyRuchStatekWpoblizu
        {
            get
            {
                return MozliwyRuchStatekWpoblizu;
            }
        }
    }
    class Kierunki
    {
        static int Nieustawiony = -1;
        static int Brak = 0;
        static int Gora=1;
        static int Prawo=2;
        static int Dol=3;
        static int Lewo=4;
        public static int nieustawiony
        {
            get
            {
                return Nieustawiony;
            }
        }
        public static int brak
        {
            get
            {
                return Brak;
            }
        }
        public static int gora
        {
            get
            {
                return Gora;
            }
        }
        public static int prawo
        {
            get
            {
                return Prawo;
            }
        }
        public static int dol
        {
            get
            {
                return Dol;
            }
        }
        public static int lewo
        {
            get
            {
                return Lewo;
            }
        }
    }
    class Tryby
    {
        static int Nieustawiony = -1;
        static int Normalny = 0;
        static int Deathmatch=1;
        public static int nieustawiony
        {
            get
            {
                return Nieustawiony;
            }
        }
        public static int normalny
        {
            get
            {
                return Normalny;
            }
        }
        public static int deathmatch
        {
            get
            {
                return Deathmatch;
            }
        }
    }
}
