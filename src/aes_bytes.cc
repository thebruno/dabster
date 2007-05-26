/*
  Sigma Dabster 5
  Copyright (C) 2007  Zespó³ Dabstera

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
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

///
/// @file   aes_bytes.cc
/// @author Jan Swierzawski
///

#include "aes_macros.H"
#include "aes_bytes.H"

#include <bitset>
#include <vector>

#if ( (DBYTES == CURRENT) && DEBUG )
#include <iostream>
#endif
using namespace aes;

std::bitset<8> byte::m (0x1b);


void
byte::operator *= (const byte op)
{
     byte result (this->operator* (op));
     for (register int i = 0; i < 8; i++)
 	  this->byte_.set (i, result[i]);
}

unsigned int
byte::to_int()
// to samo, co to_ulong(), bo mamy 8 bitów
{ return byte_.to_ulong (); };

byte
byte::operator + (const byte op)
{ return op.byte_ ^ byte_; }

void
byte::operator += (const byte op ) 
{ byte_ ^= op.get_byte (); }

byte
byte::operator + (const std::bitset <8> op)
{ return op ^ byte_; }

void
byte::operator += (const std::bitset <8> op)
{ byte_ ^= op; }

byte
byte::operator - (const byte op)
{ return op.byte_ ^ byte_; }

bool
byte::operator [] (const short int pos)
{ return byte_[pos]; }

bool
byte::operator == (byte const  other)
{ return byte_ == other.get_byte (); };

inline bool
byte::operator != (byte const  other)
{return (! operator==( other));}


const std::bitset <8>
byte::get_byte () const
{ return byte_; };

const bool
byte::get_bit (int i) const
{ return get_byte()[i]; }

// do upiêkszenia - ta wersja po prostu dzia³a :)
byte
byte::operator * (const byte op)
{
     std::bitset <8> result ( ( true == (op.get_byte ())[0] )
			      ? byte_ : 0 );
     std::bitset <8> temp (byte_);
     
     for (register int i = 1; i < 8; i++) {
	  bool needs_correction = temp[7];
	  temp <<= 1;
	  if (needs_correction) temp ^= m;

	  if ( (op.get_byte ())[i] ) {
	       result ^= temp;
	  }
     }

     return result;
}

void
byte::operator = (const char other)
{
     if (other != to_char())
	  this->operator= (byte (other));
     
}


void
byte::operator = (const byte & other)
{
     byte tmp (const_cast <byte &> (other));
     for (int i = 0; i < 8; i++)
	  byte_.set (i, other.get_bit (i));
     
}


#if ( (CURRENT == DBYTES) &&  DEBUG )
int
main ()
{
     aes::byte c (0x57);
     aes::byte d (0x13);

     // operatory przypisania dzia³aj±:
//      c = d;
//      d = '1';
     

     std::cout << " c: " << c.to_int() << std::endl;
     std::cout << " d: " << d.to_int()<< std::endl;


     std::cout << c.get_byte () << " * " << d.get_byte () << " = "
	       << (c * d).get_byte () << std::endl;

     std::cout << " c: " << c.get_byte () << std::endl;
     std::cout << " d: " << d.get_byte () << std::endl;

     std::bitset <8> e (0x11);
     std::cout << " e: " << e << std::endl;
     c *= d;
     std::cout << "c *= d; // c = " << c.get_byte () << std::endl;

     c += e;
     std::cout << "     c += e; // " << c.get_byte () << std::endl;
     
     return 0;
}
#endif

