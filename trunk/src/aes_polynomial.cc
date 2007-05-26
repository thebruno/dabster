/*
  Sigma Dabster 5
  Copyright (C) 2007  Zespół Dabstera

  This program is free software; you can redistribute it and/or modify
  it under the terms_ of the GNU General Public License as published by
  the Free Software Foundation; version 2 dated June, 1991.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301 USA.
*/

#include <vector>
#include <iostream>

#include "aes_macros.H"
#include "aes_bytes.H"
#include "aes_polynomial.H"

using namespace aes;

const size_t
polynomial::terms() const
{return terms_;}


polynomial
polynomial::operator + (const polynomial op)
{
     std::vector <byte> result (coefficients_);
     for (int i = 0; i < 4; i++)
	  result[i] += op.get_coefficient (i);
     return polynomial (result);
}

void
polynomial::operator += (const polynomial op)
{
     for (int i = 0; i < 4; i++)
	  coefficients_[i] += op.get_coefficient (i);
}


byte &
polynomial::operator [] (const int i)
{return coefficients_[i];}


polynomial
polynomial::operator<< (const unsigned int i)
{
     polynomial result;
     for (unsigned int cnt = 0; cnt < i; cnt++) {
	  byte temp (result[0]);
	  for (uint j = 0; j < terms_ - 1; j++)
	       result[j] = result[j+1];
	  result[terms_ - 1] = temp;
     }
     return result;
}


polynomial
polynomial::operator>> (const unsigned int i)
{
     polynomial result;
     for (unsigned int cnt = 0; cnt < i; cnt++) {
	  byte temp (result [terms_ - 1]);
	  for (int k = terms_ -1 ; k >= 0; k--)
	       result[k] = result[k - 1];
	  result [0] = temp;
     }
     return result;
}



void
polynomial::operator<<= (const unsigned int i)
{
     for (unsigned int cnt = 0; cnt < i; cnt++) {
	  byte temp (coefficients_[0]);
	  for (uint j = 0; j < terms_ - 1; j++)
	       coefficients_[j] = coefficients_[j+1];
	  coefficients_[terms_ - 1] = temp;
     }

}

///
/// @param a3 najstarszy bajt
/// @param a0 najmłodszy bajt
///
polynomial::polynomial (const byte a3, const byte a2,
			const byte a1, const byte a0)
     : coefficients_ (terms_)
{
     coefficients_[3] = a3;
     coefficients_[2] = a2;
     coefficients_[1] = a1;
     coefficients_[0] = a0;
}

///
/// @param a3 najstarszy bajt
/// @param a0 najmłodszy bajt
///
polynomial::polynomial (const char a3, const char a2,
			const char a1, const char a0)
     : coefficients_ (terms_)
{
     coefficients_[3] = a3;
     coefficients_[2] = a2;
     coefficients_[1] = a1;
     coefficients_[0] = a0;
}


void
polynomial::operator>>= (const unsigned int i)
{
     for (unsigned int cnt = 0; cnt < i; cnt++) {
	  byte temp (coefficients_ [terms_ - 1]);
	  for (int k = terms_ -1 ; k >= 0; k--)
	       coefficients_[k] = coefficients_[k - 1];
	  coefficients_ [0] = temp;
     }
}



// Nadgorliwość jest gorsza od faszyzmu - to jest wycinek z FIPS-197:
// d0 = (a0 • b0 ) ⊕ (a3 • b1 ) ⊕ (a2 • b2 ) ⊕ (a1 • b3 )
// d1 = (a1 • b0 ) ⊕ (a0 • b1 ) ⊕ (a3 • b2 ) ⊕ (a2 • b3 )  (4.12)
// d2 = (a2 • b0 ) ⊕ (a1 • b1 ) ⊕ (a0 • b2 ) ⊕ (a3 • b3 )
// d3 = (a3 • b0 ) ⊕ (a2 • b1 ) ⊕ (a1 • b2 ) ⊕ (a0 • b3 )

void
polynomial::operator= (const polynomial & other)
{
     polynomial tmp (other);
     for (uint i = 0; i < terms_; i++)
	  coefficients_[i] = tmp.get_coefficient (i);
}



polynomial
polynomial::operator* (polynomial op)
{
     polynomial d;
     std::vector <byte> &a =  coefficients_;
     std::vector <byte> b(op.get_coefficients());
     d[0]= ((a[0] * b[0]) + (a[3] * b[1]) + (a[2] * b[2]) + (a[1] * b[3]));
     d[1]= ((a[1] * b[0]) + (a[0] * b[1]) + (a[3] * b[2]) + (a[2] * b[3]));
     d[2]= ((a[2] * b[0]) + (a[1] * b[1]) + (a[0] * b[2]) + (a[3] * b[3]));
     d[3]= ((a[3] * b[0]) + (a[2] * b[1]) + (a[1] * b[2]) + (a[0] * b[3]));

     return d;
}

void
polynomial::operator*=(const polynomial op)
{
     std::vector <byte> &a = coefficients_;
     std::vector <byte> b (op.get_coefficients()) ;
     polynomial d;

     d[0]= (a[0] * b[0]) + (a[3] * b[1]) + (a[2] * b[2]) + (a[1] * b[3]);
     d[1]= (a[1] * b[0]) + (a[0] * b[1]) + (a[3] * b[2]) + (a[2] * b[3]);
     d[2]= (a[2] * b[0]) + (a[1] * b[1]) + (a[0] * b[2]) + (a[3] * b[3]);
     d[3]= (a[3] * b[0]) + (a[2] * b[1]) + (a[1] * b[2]) + (a[0] * b[3]);

     for (uint i = 0; i < terms_; i++)
	  a[i] = d[i];

}


const byte
polynomial::get_coefficient (const int i) const
{ return coefficients_[i]; }

const std::vector<byte>
polynomial::get_coefficients () const
{ return coefficients_; }


#if ( (CURRENT == DPOLYNOMIAL) && DEBUG)
using namespace std;
int
main()
{
     std::vector<byte> t (4);
     std::vector<byte> d (4);

     t[3] = 0x03;
     t[2] = 0x01;
     t[1] = 0x01;
     t[0] = 0x02;

     d[3] = 0x0b;
     d[2] = 0x0d;
     d[1] = 0x09;
     d[0] = 0x0e;


     polynomial test_1 (t);
     polynomial test_2 (d);

     test_1 *= test_2;

     cout << test_1.get_coefficient(0).get_byte() << endl
	  << test_1.get_coefficient(1).get_byte()<< endl
	  << test_1.get_coefficient(2).get_byte()<< endl
	  << test_1.get_coefficient(3).get_byte()<< endl
	  << endl;

     return 0;

}
#endif


