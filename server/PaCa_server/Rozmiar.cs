using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    public class Rozmiar
    {
        int pX;
        int pY;
        public Rozmiar(int X, int Y)
        {
            pX = X;
            pY = Y;
        }
        public Rozmiar(Rozmiar roz)
        {
            pX = roz.pX;
            pY = roz.pY;
        }
        public int x
        {
            get
            {
                return pX;
            }
        }
        public int y
        {
            get
            {
                return pY;
            }
        }
        public static bool operator ==(Rozmiar r1, Rozmiar r2)
        {
            if (object.ReferenceEquals(r1, null) && object.ReferenceEquals(r2, null))
                return true;
            if (object.ReferenceEquals(r1, null))
                return false;
            if (object.ReferenceEquals(r2, null))
                return false;
            return r1.x == r2.x && r1.y == r2.y;
        }
        public static bool operator !=(Rozmiar r1, Rozmiar r2)
        {
            return !(r1 == r2);
        }
        public override bool Equals(object obj)
        {
            return obj is Rozmiar && (Rozmiar)obj == this;
        }
        public override int GetHashCode()
        {
            return this.ToString().GetHashCode();
        }
        public override string ToString()
        {
            return " " + x.ToString() + " " + y.ToString();
        }
    }
}
