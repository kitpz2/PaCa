using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    class Parser
    {
        public static Polecenia parsuj(string polecenie)
        {
            if(polecenie[polecenie.Length-1]!='#')
                return null;
            string[] skladowePolecenia=polecenie.Split();

            skladowePolecenia[skladowePolecenia.Length - 1] = skladowePolecenia[skladowePolecenia.Length - 1].Substring(0, skladowePolecenia[skladowePolecenia.Length-1].Length - 1);
            if(skladowePolecenia.Length==0) throw new Wyjatki.BlednePolecenie();
            Polecenia sparsowanePolecenia = new Polecenia();
            switch (skladowePolecenia[0])
            {
                case "CRE":
                    try
                    {
                        if (skladowePolecenia.Length != 8 && skladowePolecenia.Length != 7)
                            throw new Wyjatki.BlednePolecenie();
                        int tryb=Int32.Parse(skladowePolecenia[4]);
                        if (tryb != Tryby.deathmatch && tryb != Tryby.normalny)
                        {
                            throw new Wyjatki.BlednePolecenie();
                        }
                        if (tryb == Tryby.deathmatch)
                            if (skladowePolecenia.Length != 8)
                                throw new Wyjatki.BlednePolecenie();
                            else
                                sparsowanePolecenia.ustawCRE(skladowePolecenia[0], skladowePolecenia[1], Int32.Parse(skladowePolecenia[2]), Int32.Parse(skladowePolecenia[3]), Int32.Parse(skladowePolecenia[4]),Int32.Parse(skladowePolecenia[5]),skladowePolecenia[6],Int32.Parse(skladowePolecenia[7]));
                        if (tryb == Tryby.normalny)
                            if (skladowePolecenia.Length != 7)
                                throw new Wyjatki.BlednePolecenie();
                            else
                            {
                                sparsowanePolecenia.ustawCRE(skladowePolecenia[0], skladowePolecenia[1], Int32.Parse(skladowePolecenia[2]), Int32.Parse(skladowePolecenia[3]), Int32.Parse(skladowePolecenia[4]), Int32.Parse(skladowePolecenia[5]), skladowePolecenia[6]);
                            }
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "CON":
                    if (skladowePolecenia.Length != 3) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawCON(skladowePolecenia[0], skladowePolecenia[1], skladowePolecenia[2]);
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "SET":
                    if (skladowePolecenia.Length != 11) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawSET(skladowePolecenia[0], Int32.Parse(skladowePolecenia[1]), Int32.Parse(skladowePolecenia[2]), Int32.Parse(skladowePolecenia[3]), Int32.Parse(skladowePolecenia[4]), Int32.Parse(skladowePolecenia[5]), Int32.Parse(skladowePolecenia[6]), Int32.Parse(skladowePolecenia[7]), Int32.Parse(skladowePolecenia[8]), Int32.Parse(skladowePolecenia[9]), Int32.Parse(skladowePolecenia[10]));
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "MOV":
                    if (skladowePolecenia.Length != 4) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawMOV(skladowePolecenia[0], Int32.Parse(skladowePolecenia[1]), Int32.Parse(skladowePolecenia[2]), Int32.Parse(skladowePolecenia[3]));
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "SHO":
                    if (skladowePolecenia.Length != 5) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawSHO(skladowePolecenia[0], Int32.Parse(skladowePolecenia[1]), Int32.Parse(skladowePolecenia[2]), Int32.Parse(skladowePolecenia[3]), Int32.Parse(skladowePolecenia[4]));
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "PTS":
                    if (skladowePolecenia.Length != 3) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawPTSGET(skladowePolecenia[0], Int32.Parse(skladowePolecenia[1]), Int32.Parse(skladowePolecenia[2]));
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "GET":
                    if (skladowePolecenia.Length != 3) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                       sparsowanePolecenia.ustawPTSGET(skladowePolecenia[0], Int32.Parse(skladowePolecenia[1]), Int32.Parse(skladowePolecenia[2]));
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                case "LST":
                    if (skladowePolecenia.Length != 1) throw new Wyjatki.BlednePolecenie();
                    try
                    {
                        sparsowanePolecenia.ustawLST(skladowePolecenia[0]);
                    }
                    catch (Exception)
                    {
                        throw new Wyjatki.BlednePolecenie();
                    }
                    break;
                default:
                    throw new Wyjatki.BlednePolecenie();
            }
            return sparsowanePolecenia;
        }
    }
}
