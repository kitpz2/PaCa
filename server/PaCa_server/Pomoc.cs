using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    class Pomoc
    {
        public int modulo(int a, int m)
        {
            int tmp = a % m;
            if (tmp < 0)
                return m + tmp;
            else
                return tmp;
        }
    }
}
