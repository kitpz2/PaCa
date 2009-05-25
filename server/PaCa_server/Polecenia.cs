using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    class listaPlansz
    {
        string nazwa;
        int wolneMiejsca;
        int tryb;
        public listaPlansz(string n, int wm,int t)
        {
            nazwa = n;
            wolneMiejsca = wm;
            tryb = t;
        }
        public override string ToString()
        {
            return " "+nazwa+" "+wolneMiejsca.ToString()+" "+tryb.ToString();
        }
    }
    class Base
    {
        protected string polecenie;
        protected string nazwaGry;
        protected Rozmiar rozmiarPlanszy;
        protected string wersjaKlienta;
        protected int idPolecenia;
        protected int idKlienta;
        protected Pozycja pozycja;
        protected int kodBledu;
        protected int numerOdpowiedzi;
        protected int idOdpowiedzi;
        protected int kierunek;
        protected int punkty;
        protected List<Pozycja> pozycje;
        protected int trybGry;
        protected int maxGraczy;
        protected int pktDoZdobycia;
        protected string wersjaSerwera;
        protected List<listaPlansz> ListaPlansz;
        protected int ranking;
        public Base()
        {
            polecenie = null;
            nazwaGry = null;
            wersjaSerwera = null;
            rozmiarPlanszy = null;
            wersjaKlienta = null;
            idPolecenia = -1;
            idKlienta = -1;
            pozycja = null;
            pozycje=null;
            kodBledu = KodyBledu.nieustawiony;
            numerOdpowiedzi = NumerOdpowiedzi.nieustawiony;
            idOdpowiedzi = IdOdpowiedzi.nieustawiony;
            kierunek = Kierunki.nieustawiony;
            punkty = -1;
            trybGry = Tryby.nieustawiony;
            maxGraczy = -1;
            pktDoZdobycia = -1;
            ranking = -1;
        }
    }

    class Polecenia : Base
    {
        public Polecenia()
            : base()
        {
        }
        public Polecenia(string pol, int kod)
        {
            polecenie = pol;
            if (pol == "NO")
                kodBledu = kod;
            else
                idPolecenia = kod;
        }
        public Polecenia(string pol)
        {
            polecenie = pol;
        }
        public Polecenia(string pol, int idk, string wersja)
        {
            polecenie = pol;
            idKlienta = idk;
            wersjaSerwera = wersja;
        }

        public Polecenia(string pol, List<listaPlansz> lista)
        {
            polecenie = pol;
            ListaPlansz = lista;
        }
        public Polecenia(string pol, int idK, string wersja, int x, int y)
        {
            polecenie = pol;
            idKlienta = idK;
            wersjaSerwera = wersja;
            rozmiarPlanszy = new Rozmiar(x, y);
        }
        //MOV
        public Polecenia(string pol, int idP, int numer, List<Pozycja> poz)
        {
            polecenie = pol;
            idPolecenia = idP;
            numerOdpowiedzi = numer;
            pozycje = poz;
        }
        //SHO
        public Polecenia(string pol, int idP, int idO, int x1,int y1,int x2,int y2,int x3,int y3)
        {
            polecenie = pol;
            idPolecenia = idP;
            idOdpowiedzi = idO;
            pozycje = new List<Pozycja>();
            pozycje.Add(new Pozycja(x1,y1));
            pozycje.Add(new Pozycja(x2,y2));
            pozycje.Add(new Pozycja(x3,y3));
        }
        public Polecenia(string pol, int idP, int pkt, int poz)
        {
            polecenie = pol;
            idPolecenia = idP;
            punkty = pkt;
            ranking = poz;
        }
        public Polecenia(string pol, int idP, Pozycja poz)
        {
            polecenie = pol;
            idPolecenia = idP;
            pozycja = poz;
        }
        public Polecenia(string pol, Pozycja poz)
        {
            polecenie = pol;
            pozycja = poz;
        }
        public Polecenia(string pol, int idP,List<Pozycja> poz)
        {
            idPolecenia = idP;
            polecenie = pol;
            pozycje = poz;
        }
        public Polecenia(string pol, List<Pozycja> poz)
        {
            polecenie = pol;
            pozycje = poz;
        }
        public void ustawCRE(string pol, string nazwa, int x, int y, int tryb, int max, string wersja, int pkt)
        {

            polecenie = pol;
            nazwaGry = nazwa;
            rozmiarPlanszy = new Rozmiar(x, y);
            wersjaKlienta = wersja;
            trybGry = tryb;
            maxGraczy = max;
            pktDoZdobycia = pkt;
        }
        public void ustawCRE(string pol, string nazwa, int x, int y, int tryb, int max, string wersja)
        {

            polecenie = pol;
            nazwaGry = nazwa;
            rozmiarPlanszy = new Rozmiar(x, y);
            wersjaKlienta = wersja;
            trybGry = tryb;
            maxGraczy = max;
        }
        public void ustawCON(string pol, string nazwa, string wersja)
        {
            polecenie = pol;
            nazwaGry = nazwa;
            wersjaKlienta = wersja;
        }
        public void ustawLST(string pol)
        {
            polecenie = pol;
        }
        public void ustawSET(string pol, int idp, int idk, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
        {
            polecenie = pol;
            idPolecenia = idp;
            idKlienta = idk;
            pozycje = new List<Pozycja>();
            pozycje.Add(new Pozycja(x1, y1));
            pozycje.Add(new Pozycja(x2, y2));
            pozycje.Add(new Pozycja(x3, y3));
            pozycje.Add(new Pozycja(x4, y4));
        }
        public void ustawMOV(string pol, int idp, int idk, int kier)
        {
            polecenie = pol;
            idPolecenia = idp;
            idKlienta = idk;
            kierunek = kier;
        }
        public void ustawSHO(string pol, int idp, int idk, int x, int y)
        {
            polecenie = pol;
            idPolecenia = idp;
            idKlienta = idk;
            pozycja = new Pozycja(x, y);
        }
        public void ustawPTSGET(string pol, int idp, int idk)
        {
            polecenie = pol;
            idPolecenia = idp;
            idKlienta = idk;
        }
        public List<Pozycja> Pozycje
        {
            get
            {
                return pozycje;
            }
        }
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder("");
            sb.Append(polecenie);
            if (idKlienta != -1)
                sb.Append(" " + idKlienta.ToString());
            if (idPolecenia != -1)
                sb.Append(" " + idPolecenia.ToString());
            if (ListaPlansz != null)
                foreach (listaPlansz lp in ListaPlansz)
                {
                    sb.Append(lp.ToString());
                }
            if (wersjaSerwera != null)
                sb.Append(" "+wersjaSerwera);
            if (numerOdpowiedzi != NumerOdpowiedzi.nieustawiony)
                sb.Append(" "+numerOdpowiedzi.ToString());
            if (idOdpowiedzi != IdOdpowiedzi.nieustawiony)
                sb.Append(" " + idOdpowiedzi.ToString());
            if (punkty != -1)
                sb.Append(" " + punkty.ToString());
            if (ranking != -1)
                sb.Append(" " + ranking.ToString());
            if (rozmiarPlanszy != null)
            {
                sb.Append(rozmiarPlanszy.ToString());
            }
            if (kodBledu != KodyBledu.nieustawiony)
                sb.Append(" " + kodBledu.ToString());
            if (pozycja != null)
                sb.Append(" "+pozycja.ToString());
            int i=0;
            if (pozycje != null)
                foreach (Pozycja p in pozycje)
                    if (i++ == 0)
                        sb.Append(" " + p.ToString());
                    else
                        sb.Append("&" + p.ToString());
            sb.Append("#");
            return sb.ToString();

        }
        public string Polecenie
        {
            get
            {
                return polecenie;
            }
        }
        public string NazwaGry
        {
            get
            {
                return nazwaGry;
            }
        }
        public int PktDoZdobycia
        {
            get
            {
                return pktDoZdobycia;
            }
        }
        public Rozmiar RozmiarPlanszy
        {
            get
            {
                return rozmiarPlanszy;
            }
        }
        public int Tryb
        {
            get
            {
                return trybGry;
            }
        }
        public int MaksymalnaIloscGraczy
        {
            get
            {
                return maxGraczy;
            }
        }
        public string WersjaKlienta
        {
            get
            {
                return wersjaKlienta;
            }
        }
        public int IdPolecenie
        {
            get
            {
                return idPolecenia;
            }
        }
        public int IdKlienta
        {
            get
            {
                return idKlienta;
            }
        }
        public Pozycja nPozycja
        {
            get
            {
                return pozycja;
            }
        }
        public int Kierunek
        {
            get
            {
                return kierunek;
            }
        }

    }
}
