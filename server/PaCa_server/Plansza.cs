using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace StatkiServ
{
    class Plansza
    {
        List<Statek> statki;
        Rozmiar rozmiarPlanszy;
        Polecenia polecenie;
        public List<Thread> klienci;
        public bool start = false;
        public delegate void gotoweGry();
        public Mutex mutexRuch;
        public Mutex mutexStrzal;
        event gotoweGry gry;
        delegate void widoczneStatkiInformacja(int kierunek, Statek statek);
        Semaphore sem;
        public Plansza(Polecenia p)
        {
            sem = new Semaphore(0, 1);
            mutexStrzal = new Mutex();
            mutexRuch = new Mutex();
            statki = new List<Statek>();
            polecenie = p;
            rozmiarPlanszy = new Rozmiar(p.RozmiarPlanszy);
            klienci = new List<Thread>();
            Thread th = new Thread(this.sprawdzajPlansze);
            th.Start();
        }
        delegate void strzalyWpoblizu(Pozycja p);
        event strzalyWpoblizu strzaly;
        public int Tryb
        {
            get
            {
                return polecenie.Tryb;
            }
        }
        public void usunStatek(Statek st)
        {
            statki.Remove(st);
        }
        public string Nazwa
        {
            get
            {
                return polecenie.NazwaGry;
            }
        }
        public int LiczbaGraczy
        {
            get
            {
                return klienci.Count;
            }
        }
        public void zglosGotowosc(gotoweGry g)
        {
            gry += g;
            if (Tryb == Tryby.deathmatch)
                if (gry.GetInvocationList().Length == 2)
                    sem.Release(1);
            if (Tryb == Tryby.normalny)
                if (gry.GetInvocationList().Length >= 2 && gry.GetInvocationList().Length == klienci.Count)
                    sem.Release(1);
        }
        public int rozmiarX
        {
            get
            {
                return rozmiarPlanszy.x;
            }
        }
        public int rozmiarY
        {
            get
            {
                return rozmiarPlanszy.y;
            }
        }
        public bool dodajKlienta(Thread th)
        {
            try
            {
                klienci.Add(th);
                th.Start();
            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }
        private void sprawdzajPlansze()
        {
            sem.WaitOne();
            start = true;
            foreach (gotoweGry gg in gry.GetInvocationList())
            {
                Thread th = new Thread(new ThreadStart(gg));
                th.Start();
            }
        }

        public bool dodajStatek(Statek statek)
        {
            try
            {
                statki.Add(statek);

            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }
        public bool moznaDodacStatek(Statek statek)
        {
            if (statki.Count >= polecenie.MaksymalnaIloscGraczy)
                return false;
            foreach (Statek s in statki)
                if (s == statek) return false;
            return true;
        }
        public List<Pozycja> wykonajRuch(Statek statek, int kierunek)
        {
            if (statki.IndexOf(statek) == -1)
                return null;
            if (!statek.ruchWarunkowy())
            {
                return null;
            }
            List<Pozycja> widoczneStatki = new List<Pozycja>();
            if (!statek.przegrana)
            {
                statek.mutexPozycje.WaitOne();
                foreach (Statek s in statki)
                {
                    if (s == statek) continue;
                    s.mutexPozycje.WaitOne();
                    if (s.kolizja(statek, kierunek, rozmiarPlanszy))
                        return null;
                    statek.wykonajRuch(kierunek, rozmiarPlanszy);
                    List<Pozycja> p = statek.wZasiegu(s, rozmiarPlanszy);
                    foreach (Pozycja pp in p)
                        widoczneStatki.Add(pp);
                    p = s.wZasiegu(statek, rozmiarPlanszy);
                    if (p.Count != 0)
                    {
                        s.widocznyStatek(p);
                    }
                    s.mutexPozycje.ReleaseMutex();
                }
                statek.mutexPozycje.ReleaseMutex();
            }
            return widoczneStatki;
        }
        public List<Pozycja> wykonajStrzal(Statek statek, Pozycja pozycja,int idPolecenia)
        {
            statek.zatopiono = false;
            strzaly = null;
            statek.mutexPozycje.WaitOne();
            if (statek.strzalWsiebie(pozycja) || !statek.zasiegStrzalu(pozycja) || !statek.strzalWarunkowy())
                return null;
            Statek trafionyStatek = strzelWstatek(pozycja);
            List<Pozycja> pozostalePola = new List<Pozycja>();
            bool wygrana = false;
            Console.WriteLine("______________________");
            if (trafionyStatek!= null)
            {
                Console.WriteLine("-------------------");
                trafionyStatek.mutexPozycje.WaitOne();
                trafionyStatek.trafionoStatek(pozycja,idPolecenia);
                if (trafionyStatek.pozycje.Count == 1)
                {
                    statek.zatopiono = true;
                    if (polecenie.Tryb == Tryby.normalny)
                    {
                        trafionyStatek.zatopionoStatek();
                        if (klienci.Count == 2)
                            statek.wygrana = wygrana = true;
                        trafionyStatek.przegrana = true;
                    }
                    else
                    {
                        statek.punkty += 10;
                        if (statek.punkty < polecenie.PktDoZdobycia)
                            trafionyStatek.odrodzenie(statki, rozmiarPlanszy);
                        else
                            statek.wygrana = wygrana = true;
                    }
                }
                else
                {
                    Pozycja tmp = trafionyStatek.pozycje.Find(delegate(Pozycja poz) { return poz.x == pozycja.x && poz.y == pozycja.y; });
                    trafionyStatek.pozycje.Remove(tmp);
                    pozostalePola = trafionyStatek.pozycje;
                    if (polecenie.Tryb == Tryby.deathmatch)
                    {
                        statek.punkty += 5;
                    }
                }
                trafionyStatek.mutexPozycje.ReleaseMutex();
            }
            else pozostalePola.Add(new Pozycja(-1, -1));
            if (klienci.Count == 2 && trafionyStatek != null)
                if (!trafionyStatek.przegrana)
                    wygrana = true;

            if (!wygrana)
            {
                rozglaszajStrzal(pozycja, trafionyStatek, statek);
                if (trafionyStatek == null)
                    statkiBlisko(pozostalePola, pozycja, statek);
            }
            statek.mutexPozycje.ReleaseMutex();
            if (strzaly != null)
                strzaly(pozycja);
            return pozostalePola;
        }
        public Statek strzelWstatek(Pozycja poz)
        {
            foreach (Statek s in statki)
            {
                foreach (Pozycja p in s.pozycje)
                    if (poz == p)
                        return s;
            }
            return null;
        }
        public int modulo(int a, int m)
        {
            int tmp = a % m;
            if (tmp < 0)
                return m + tmp;
            else
                return tmp;
        }
        private void rozglaszajStrzal(Pozycja poz, Statek trafiony, Statek strzelajacy)
        {
            foreach (Statek st in statki)
            {
                if (st == trafiony || st == strzelajacy) continue;
                bool blisko = false;
                foreach (Pozycja p in st.pozycje)
                {
                    for (int i = 0; i < 11; i++)
                    {
                        for (int j = 0; j < 11; j++)
                        {
                            if (i == 0 && j == 0)
                                continue;
                            if (p.x == modulo(poz.x + i, rozmiarPlanszy.x) && p.y == modulo(poz.y + j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x + i, rozmiarPlanszy.x) && p.y == modulo(poz.y - j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x - i, rozmiarPlanszy.x) && p.y == modulo(poz.y - j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x - i, rozmiarPlanszy.x) && p.y == modulo(poz.y + j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                        }
                        if (blisko) break;
                    }
                    if (blisko) break;
                }
                if (blisko)
                    strzaly += st.strzalWpoblizu;
            }

        }
        private void statkiBlisko(List<Pozycja> widocznePozycje, Pozycja poz, Statek strzelajacy)
        {
            foreach (Statek st in statki)
            {
                if (st == strzelajacy) continue;
                bool blisko = false;
                foreach (Pozycja p in st.pozycje)
                {
                    for (int i = 0; i < 7; i++)
                    {
                        for (int j = 0; j < 7; j++)
                        {
                            if (p.x == modulo(poz.x + i, rozmiarPlanszy.x) && p.y == modulo(poz.y + j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x + i, rozmiarPlanszy.x) && p.y == modulo(poz.y - j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x - i, rozmiarPlanszy.x) && p.y == modulo(poz.y - j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                            if (p.x == modulo(poz.x - i, rozmiarPlanszy.x) && p.y == modulo(poz.y + j, rozmiarPlanszy.y))
                            {
                                blisko = true;
                                break;
                            }
                        }
                        if (blisko) break;
                    }
                    if (blisko)
                        widocznePozycje.Add(p);
                }

            }
        }
    }
}
