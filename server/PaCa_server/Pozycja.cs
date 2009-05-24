using System;
using System.Collections.Generic;
using System.Text;

namespace StatkiServ
{
    public class Pozycja:IComparable<Pozycja>
    {
        int pX;
        int pY;
        public Pozycja(int X, int Y)
        {
            x = X;
            y = Y;
        }
        public Pozycja skaluj(int X, int Y)
        {
            x %= X;
            y %= Y;
            return this;
        }
        public int x
        {
            get
            {
                return pX;
            }
            set
            {
                pX = value;
            }
        }
        public int y
        {
            get
            {
                return pY;
            }
            set
            {
                pY = value;
            }
        }
        public static bool operator==(Pozycja p1, Pozycja p2)
        {
            if (object.ReferenceEquals(p1, null) && object.ReferenceEquals(p2, null))
                return true;
            if (object.ReferenceEquals(p1, null))
                return false;
            if (object.ReferenceEquals(p2, null))
                return false;
            return p1.x == p2.x && p1.y == p2.y;
        }
        public static bool operator !=(Pozycja p1, Pozycja p2)
        {
            return !(p1 == p2);
        }
        public override bool Equals(object obj)
        {
            return obj is Pozycja && (Pozycja)obj == this;
        }
        public override int GetHashCode()
        {
            return this.ToString().GetHashCode();
        }
        public override string ToString()
        {
            return pX.ToString()+","+pY.ToString();
        }
        public static bool operator >(Pozycja p, Rozmiar r)
        {
            return p.x > r.x || p.y > r.y;
        }
        public static bool operator <(Pozycja p, Rozmiar r)
        {
            return p.x < r.x || p.y < r.y;
        }
        public static Pozycja operator -(Pozycja p, int i)
        {
            return new Pozycja(p.x - i, p.y - i);
        }
        public static Pozycja operator +(Pozycja p, int i)
        {
            return new Pozycja(p.x + i, p.y + i);
        }
        public static bool operator <(Pozycja p, int i)
        {
            return p.x<i&&p.y<i;
        }
        public static bool operator >(Pozycja p, int i)
        {
            return p.x > i || p.y > i;
        }
        public static Pozycja operator %(Pozycja p, int i)
        {
            int t1 = p.x % i;
            int t2 = p.y % i;
            if (t1 < 0) t1 = i + t1;
            if (t2 < 0) t2 = i + t2;
            return new Pozycja(t1, t2);
        }
        public int CompareTo(Pozycja p)
        {
            if (this.x.CompareTo(p.x)==0)
                return this.y.CompareTo(p.y);
            return this.x.CompareTo(p.x);
        }
    }
}
