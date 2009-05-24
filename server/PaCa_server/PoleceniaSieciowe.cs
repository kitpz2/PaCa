using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace StatkiServ
{
    class PoleceniaSieciowe
    {
        public static string odbierzPolecenie(NetworkStream stream)
        {
            Byte[] bajty = new Byte[256];
            int dlugosc = 0;
            StringBuilder polecenie = new StringBuilder("");
            while (true)
            {
                int i = stream.Read(bajty, 0, 256);
                dlugosc += i;
                polecenie.Append(Encoding.ASCII.GetString(bajty,0,i));
                if (polecenie[dlugosc - 1] == '#') break;
            }
            return polecenie.ToString();
        }
        public static void wyslijPolecenie(NetworkStream stream, Polecenia polecenie)
        {
            stream.Write(Encoding.ASCII.GetBytes(polecenie.ToString().ToCharArray()), 0, polecenie.ToString().Length);
        }
    }
}
