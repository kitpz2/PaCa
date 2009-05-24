using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Threading;
namespace StatkiServ
{
    public class Gra
    {
        public static string wersjaSerwera = "v0.4.0";
        private Hashtable gry;
        int id = 0;
        public Gra()
        {
            gry = new Hashtable();
            start();
        }

        public void start()
        {
            IPAddress ip = IPAddress.Parse("127.0.0.1");
            TcpListener listener = new TcpListener(ip, 8000);
            listener.Start();
            

            while (true)
            {

                TcpClient klient = listener.AcceptTcpClient();
                Thread th = new Thread(new ParameterizedThreadStart(watekStart));
                th.Start(klient);
            }
        }
        void watekStart(Object k)
        {
            TcpClient klient = (TcpClient)k;
            string polecenie = PoleceniaSieciowe.odbierzPolecenie(klient.GetStream());
            Polecenia sparsowanePolecenie = null;
            try
            {
                sparsowanePolecenie = Parser.parsuj(polecenie);
            }
            catch (Wyjatki.BlednePolecenie)
            {
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.blednePolecenie));
                return;
            }
            catch (Exception)
            {
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.nieoczekiwanyBlad));
                return;
            }
            if (sparsowanePolecenie.Polecenie == "CRE")
            {
                if (sparsowanePolecenie.RozmiarPlanszy.x < 10 || sparsowanePolecenie.RozmiarPlanszy.y < 10)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.blednyRozmiarPlanszy));
                    return;
                }
                if (sparsowanePolecenie.MaksymalnaIloscGraczy > (int)(sparsowanePolecenie.RozmiarPlanszy.x * sparsowanePolecenie.RozmiarPlanszy.y / 50))
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.zaDuzoGraczyNaPlansze));
                    return;
                }
                if (sparsowanePolecenie.MaksymalnaIloscGraczy < 2)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.zaMaloGraczy));
                    return;
                }
                if (sparsowanePolecenie.Tryb == Tryby.deathmatch)
                    if (sparsowanePolecenie.PktDoZdobycia < 2 * 30 * sparsowanePolecenie.MaksymalnaIloscGraczy)
                    {
                        PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.zaMaloPunktow));
                        return;
                    }
                Plansza plansza = new Plansza(sparsowanePolecenie);
                try
                {
                    gry.Add(sparsowanePolecenie.NazwaGry, plansza);
                }
                catch (ArgumentException)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.niemozliweUtworzenieGry));
                    return;
                }
                catch (Exception)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.nieoczekiwanyBlad));
                    return;
                }
                WatekGracza nowyGracz = new WatekGracza(klient, id, plansza);
                plansza.dodajKlienta(new Thread(nowyGracz.start));
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("OK", id++, wersjaSerwera));
            }
            else if (sparsowanePolecenie.Polecenie == "CON")
            {
                Plansza plansza = null;
                plansza = (Plansza)gry[sparsowanePolecenie.NazwaGry];
                if (plansza == null)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.brakGry));
                    return;
                }
                WatekGracza nowyGracz = new WatekGracza(klient, id, plansza);
                plansza.dodajKlienta(new Thread(nowyGracz.start));
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("OK", id++, wersjaSerwera, plansza.rozmiarX, plansza.rozmiarY));
            }
            else if (sparsowanePolecenie.Polecenie == "LST")
            {
                List<listaPlansz> plansze = new List<listaPlansz>();
                foreach (Plansza p in gry.Values)
                {
                    try
                    {
                        if ((p.start == false && p.Tryb == Tryby.normalny) || (p.Tryb == Tryby.deathmatch))
                        {
                            plansze.Add(new listaPlansz(p.Nazwa, p.LiczbaGraczy, p.Tryb));
                        }
                    }
                    catch
                    {
                        PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.nieoczekiwanyBlad));
                        return;
                    }
                }
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("OK", plansze));
            }
            else
            {
                PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.blednePolecenie));
                return;
            }
        }
    }
}
