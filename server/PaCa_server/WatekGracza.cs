﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Net;
using System.Threading;
using System.Net.Sockets;
namespace StatkiServ
{
    class WatekGracza
    {
        private TcpClient klient;
        public int idKlienta;
        Plansza plansza;
        Statek statek;
        NetworkStream stream;
        Thread th;
        Mutex mutex;
        public WatekGracza(TcpClient k, int id, Plansza p)
        {
            plansza = p;
            idKlienta = id;
            klient = k;
            stream = klient.GetStream();
            mutex = new Mutex();
        }
        public void start()
        {
            statek = new Statek();
            int idPolecenia;
            while (true)
            {
                string pozycjaStatku = PoleceniaSieciowe.odbierzPolecenie(stream);
                Polecenia polecenie = null;
                try
                {
                    polecenie = Parser.parsuj(pozycjaStatku);
                }
                catch (Wyjatki.BlednePolecenie)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.blednePolecenie));
                    continue;
                }
                if (polecenie.Polecenie != "SET")
                {
                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.blednePolecenie));
                    continue;
                }
                if (polecenie.IdKlienta != idKlienta)
                {
                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.bledneIdKlienta));
                    continue;
                }
                List<Pozycja> pozycje = polecenie.Pozycje;
                pozycje.Sort();
                foreach (Pozycja p in pozycje)
                {
                    if (p.x < 0 || p.x > plansza.rozmiarX - 1 || p.y < 0 || p.y > plansza.rozmiarY - 1)
                    {
                        PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.blednePolozenieStartowe));
                        continue;
                    }
                }
                bool blad = false;
                for (int i = 0; i < 3; i++)
                    for (int j = i + 1; j < 4; j++)
                    {
                        if (pozycje[i] == pozycje[j]) blad = true;
                        if (Math.Abs(pozycje[i].x - pozycje[j].x) > 1)
                            if ((pozycje[i].x != 0 && pozycje[i].x != plansza.rozmiarX - 1) || (pozycje[j].y != 0 && pozycje[j].y != plansza.rozmiarX - 1))
                            {
                                blad = true;
                                break;
                            }
                        if (Math.Abs(pozycje[i].y - pozycje[j].y) > 1)
                            if ((pozycje[i].y != 0 && pozycje[i].y != plansza.rozmiarY - 1) || (pozycje[j].y != 0 && pozycje[j].y != plansza.rozmiarY - 1))
                            {
                                blad = true;
                                break;
                            }
                        if (blad) break;
                    }
                if (blad)
                {
                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.blednePolozenieStartowe));
                    continue;
                }
                statek = new Statek(idKlienta, stream, pozycje);
                if (plansza.moznaDodacStatek(statek))
                {
                    plansza.dodajStatek(statek);
                }
                else
                {
                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.maksymalnaLiczbaGraczy));
                    continue;
                }
                idPolecenia = polecenie.IdPolecenie;
                break;
            }
            statek.dodano = true;
            PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", idPolecenia));
            th = Thread.CurrentThread;
            plansza.zglosGotowosc(odbierajPolecenia);
        }
        void odbierajPolecenia()
        {
            PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("START"));
            while (!statek.przegrana)
            {
                string pozycjaStatku = PoleceniaSieciowe.odbierzPolecenie(stream);
                Polecenia polecenie = null;
                try
                {
                    polecenie = Parser.parsuj(pozycjaStatku);
                }
                catch (Wyjatki.BlednePolecenie)
                {
                    PoleceniaSieciowe.wyslijPolecenie(klient.GetStream(), new Polecenia("NO", KodyBledu.blednePolecenie));
                    continue;
                }
                switch (polecenie.Polecenie)
                {
                    case "MOV":
                        plansza.mutexRuch.WaitOne();
                        List<Pozycja> stat = plansza.wykonajRuch(statek, polecenie.Kierunek);
                        if (!statek.przegrana)
                            if (stat == null)
                            {
                                PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.niemozliwyRuch));
                            }
                            else
                            {
                                if (stat.Count != 0)
                                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, NumerOdpowiedzi.mozliwyRuchStatekWpoblizu, stat));
                                else
                                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, NumerOdpowiedzi.mozliwyRuchBrakStatku, stat));
                                foreach (Pozycja p in statek.pozycje)
                                    Console.Write(" (" + p.x + "," + p.y + ")");
                                Console.WriteLine("");
                                statek.aktualizujCzasRuchu();
                            }
                        plansza.mutexRuch.ReleaseMutex();
                        break;
                    case "SHO":
                        plansza.mutexStrzal.WaitOne();
                        List<Pozycja> strzaly = plansza.wykonajStrzal(statek, polecenie.nPozycja, polecenie.IdPolecenie);
                        if (strzaly == null)
                            PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.niemozliwyStrzal));
                        else
                        {
                            if (strzaly[0] == new Pozycja(-1, -1))
                            {
                                if (strzaly.Count == 1)
                                {
                                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, polecenie.IdPolecenie, IdOdpowiedzi.chybionyStrzal));
                                }

                                else
                                {
                                    List<Pozycja> tmp = new List<Pozycja>();
                                    for (int i = 1; i < strzaly.Count; i++)
                                        tmp.Add(strzaly[i]);
                                    PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, IdOdpowiedzi.pudloBliskoStatku, tmp));
                                }
                            }
                            else
                            {
                                PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, statek.zatopiono ? IdOdpowiedzi.zatopiony : IdOdpowiedzi.trafiony, strzaly));
                            }
                            statek.aktualizujCzasStrzalu();

                        }
                        plansza.mutexStrzal.ReleaseMutex();
                        break;
                    case "PTS":
                        PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, statek.punkty, 1));
                        break;
                    case "GET":
                        statek.mutexPozycje.WaitOne();
                        PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("OK", polecenie.IdPolecenie, statek.pozycje));
                        statek.mutexPozycje.ReleaseMutex();
                        break;
                    default:
                        PoleceniaSieciowe.wyslijPolecenie(stream, new Polecenia("NO", KodyBledu.blednePolecenie));
                        break;
                }
            }
            plansza.usunStatek(statek);
            plansza.klienci.Remove(Thread.CurrentThread);
        }
    }
}
