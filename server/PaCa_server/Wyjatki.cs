using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    public class Wyjatki
    {
        public class BlednePolecenie : ApplicationException
        {
            public BlednePolecenie()
                : base("Bledne polecenie")
            { }

        }
    }
}
