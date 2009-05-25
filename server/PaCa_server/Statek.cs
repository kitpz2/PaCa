using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Threading;
namespace StatkiServ
{
    public class Statek
    {
        List<Pozycja> pPozycje;
        List<Pozycja> pPelnePozycje;
        NetworkStream pStream;
        int id;
        public bool zatopiono = false;
        public int punkty = 0;
        public bool dodano = false;
        DateTime czasRuchu = new DateTime(1, 1, 1);
        DateTime czasStrzalu = new DateTime(1, 1, 1);
        public Mutex mutexPozycje;
        public bool przegrana = false;
        public bool wygrana = false;
        public Statek() { }
        public Statek(int idKlienta, NetworkStream stream, List<Pozycja> pozycje)
        {
            pPozycje = pozycje;
            pPelnePozycje = new List<Pozycja>();
            mutexPozycje = new Mutex();
            foreach (Pozycja p in pozycje)
                pPelnePozycje.Add(new Pozycja(p.x, p.y));
            pStream = stream;
            id = idKlienta;
        }
        public static bool operator ==(Statek st1, Statek st2)
        {
            if (object.ReferenceEquals(st1, null) && object.ReferenceEquals(st2, null))
                return true;
            if (object.ReferenceEquals(st1, null))
                return false;
            if (object.ReferenceEquals(st2, null))
                return false;
            foreach (Pozycja p1 in st1.pPozycje)
                foreach (Pozycja p2 in st2.pPozycje)
                    if (p1 == p2) return true;
            return false;
        }
        public static bool operator !=(Statek st1, Statek st2)
        {
            return !(st1 == st2);
        }

        public override bool Equals(object obj)
        {
            return obj is Statek && (Statek)obj == this;
        }
        public override string ToString()
        {
            return "Statek " + id.ToString();
        }
        public override int GetHashCode()
        {
            return this.ToString().GetHashCode();
        }
        public static bool operator >(Statek st1, Rozmiar rozmiar)
        {
            foreach (Pozycja p in st1.pPozycje)
                if (p > rozmiar)
                    return true;
            return false;
        }
        public static bool operator <(Statek st1, Rozmiar rozmiar)
        {
            foreach (Pozycja p in st1.pPozycje)
                if (p < rozmiar)
                    return true;
            return false;
        }
        public bool ruchWarunkowy()
        {
            return (DateTime.Now - czasRuchu) >= new TimeSpan(0, 0, 1);
        }
        public List<Pozycja> pozycje
        {
            get
            {
                return pPozycje;
            }
        }
        public int modulo(int a, int m)
        {
            int tmp = a % m;
            if (tmp < 0)
                return m + tmp;
            else
                return tmp;
        }
        public bool kolizja(Statek s, int kierunek, Rozmiar rozmiar)
        {
            foreach (Pozycja p in s.pozycje)
            {
                foreach (Pozycja pp in pozycje)
                {
                    if (kierunek == Kierunki.gora)
                        return (modulo((p.y + 1), rozmiar.y) == pp.y && (p.x == pp.x));
                    if (kierunek == Kierunki.dol)
                        return (modulo((p.y - 1), rozmiar.y) == pp.y && (p.x == pp.x));
                    if (kierunek == Kierunki.prawo)
                    {
                        if (modulo((p.x + 1), rozmiar.x) == pp.x && (p.y == pp.y))
                            Console.WriteLine("1. " + p.ToString() + "2. " + pp.ToString());
                        return (modulo((p.x + 1), rozmiar.x) == pp.x && (p.y == pp.y));
                    }
                    if (kierunek == Kierunki.lewo)
                        return (modulo((p.x - 1), rozmiar.x) == pp.x && (p.y == pp.y));
                }
            }
            mutexPozycje.ReleaseMutex();
            s.mutexPozycje.ReleaseMutex();
            return false;
        }
        public void widocznyStatek(List<Pozycja> poz)
        {
            PoleceniaSieciowe.wyslijPolecenie(pStream, new Polecenia("NES", poz));
        }
        public void wykonajRuch(int kierunek, Rozmiar rozmiar)
        {
            if (kierunek == Kierunki.gora)
            {
                foreach (Pozycja p in pozycje)
                    p.y = modulo((++p.y), rozmiar.y);
                foreach (Pozycja p in pPelnePozycje)
                    p.y = modulo((++p.y), rozmiar.y);
            }
            if (kierunek == Kierunki.dol)
            {
                foreach (Pozycja p in pozycje)
                    p.y = modulo((--p.y), rozmiar.y);
                foreach (Pozycja p in pPelnePozycje)
                    p.y = modulo((--p.y), rozmiar.y);
            }
            if (kierunek == Kierunki.prawo)
            {
                foreach (Pozycja p in pozycje)
                    p.x = modulo((++p.x), rozmiar.x);
                foreach (Pozycja p in pPelnePozycje)
                    p.x = modulo((++p.x), rozmiar.x);
            }
            if (kierunek == Kierunki.lewo)
            {
                foreach (Pozycja p in pozycje)
                    p.x = modulo((--p.x), rozmiar.x);
                foreach (Pozycja p in pPelnePozycje)
                    p.x = modulo((--p.x), rozmiar.x);
            }
        }
        public List<Pozycja> wZasiegu(Statek statek, Rozmiar rozmiar)
        {
            List<Pozycja> poz = new List<Pozycja>();
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (j == 0 && i == 0)
                        continue;
                    foreach (Pozycja p in statek.pozycje)
                        if (modulo((pPelnePozycje[3].x + i), rozmiar.x) == p.x && (modulo(pPelnePozycje[3].y + j, rozmiar.y) == p.y))
                            poz.Add(p);
                }
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (j == 0 && i == 0)
                        continue;
                    foreach (Pozycja p in statek.pozycje)
                        if (modulo((pPelnePozycje[1].x - i), rozmiar.x) == p.x && (modulo(pPelnePozycje[1].y + j, rozmiar.y) == p.y))
                            poz.Add(p);
                }
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (j == 0 && i == 0)
                        continue;
                    foreach (Pozycja p in statek.pozycje)
                        if (modulo((pPelnePozycje[0].x - i), rozmiar.x) == p.x && (modulo(pPelnePozycje[0].y - j, rozmiar.y) == p.y))
                            poz.Add(p);
                }
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (j == 0 && i == 0)
                        continue;
                    foreach (Pozycja p in statek.pozycje)
                        if (modulo((pPelnePozycje[2].x + i), rozmiar.x) == p.x && (modulo(pPelnePozycje[2].y - j, rozmiar.y) == p.y))
                            poz.Add(p);
                }
            return poz;
        }
        public void aktualizujCzasRuchu()
        {
            czasRuchu = DateTime.Now;
        }

        public bool kolizja(List<Statek> statki, Pozycja poz)
        {
            foreach (Statek s in statki)
            {
                s.mutexPozycje.WaitOne();
                foreach (Pozycja p in s.pozycje)
                {
                    if (poz == p)
                        return true;
                    if (poz.x + 1 == p.x && poz.y + 1 == p.y)
                        return true;
                    if (poz.x == p.x && poz.y + 1 == p.y)
                        return true;
                    if (poz.x + 1 == p.x && poz.y == p.y)
                        return true;
                }
                s.mutexPozycje.ReleaseMutex();
            }
            return false;
        }

        public void trafionoStatek(Pozycja pozycja, int idPolecenia)
        {
            PoleceniaSieciowe.wyslijPolecenie(pStream, new Polecenia("TRAF", idPolecenia, pozycja));
        }
        public void strzalWpoblizu(Pozycja pozycja)
        {
            PoleceniaSieciowe.wyslijPolecenie(pStream, new Polecenia("CLS", pozycja));
        }

        public void zatopionoStatek()
        {
            PoleceniaSieciowe.wyslijPolecenie(pStream, new Polecenia("LOSE"));
        }
        public void wygranaWyslij()
        {
            PoleceniaSieciowe.wyslijPolecenie(pStream, new Polecenia("WIN"));
        }
        public void odrodzenie(List<Statek> statki, Rozmiar rozmiar)
        {
            pozycje.RemoveAll(delegate(Pozycja p) { return true; });
            Random rand = new Random(DateTime.Now.Millisecond);
            int x = rand.Next() % rozmiar.x;
            int y = rand.Next() % rozmiar.y;
            while (true)
            {
                x = rand.Next() % rozmiar.x;
                y = rand.Next() % rozmiar.y;
                Pozycja poz = new Pozycja(x, y);
                if (kolizja(statki, poz)) continue;
                break;
            }
            List<Pozycja> lista = new List<Pozycja>();
            pozycje.Add(new Pozycja(x, y));
            pozycje.Add(new Pozycja(x, y + 1));
            pozycje.Add(new Pozycja(x + 1, y));
            pozycje.Add(new Pozycja(x + 1, y + 1));
        }

        public void aktualizujCzasStrzalu()
        {
            czasStrzalu = DateTime.Now;
        }
        public bool strzalWsiebie(Pozycja poz)
        {
            foreach (Pozycja p in pozycje)
                if (p == poz)
                    return true;
            return false;
        }
        public bool zasiegStrzalu(Pozycja poz)
        {
            bool ok = false;
            foreach (Pozycja p in pPelnePozycje)
            {
                if (Math.Abs(p.x - poz.x) < 11 && Math.Abs(p.y - poz.y) < 11)
                    ok = true;
            }
            return ok;
        }
        public bool strzalWarunkowy()
        {
            return (DateTime.Now - czasStrzalu) >= new TimeSpan(0, 0, 1);
        }
    }
}
