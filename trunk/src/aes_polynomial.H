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
/// @file   aes_polynomial.H
/// @author Jan Swierzawski
/// @date   Sat May 12 14:55:23 2007
/// 
/// @brief  Obs³uga wielomianów w GF(2^8)
/// 
/// Implementacja sekcji 4.3 FIPS-197. Wielomiany s± u³o¿one w
/// wektorze od najm³odszego do najstarszego
/// 
///
#ifndef _H_AES_POLYNOMIAL
#define _H_AES_POLYNOMIAL
#include <vector>
#include <iostream>

#include "aes_bytes.H"

namespace aes
{
     class polynomial 
     {
	  std::vector <byte> coefficients_;
	  static const size_t terms_ = 4;
     public:
	  polynomial()
	       : coefficients_ (terms_, byte ()) {};
	  polynomial (const std::vector<byte> coeff)
	       : coefficients_ (coeff) {};
	  polynomial (const byte a3, const byte a2,
		      const byte a1, const byte a0);
	  polynomial (const char a3, const char a2,
		      const char a1, const char a0);
	  
	  polynomial operator + (const polynomial op);
	  void  operator += (const polynomial op);
	  polynomial operator * ( polynomial op);
	  void operator *= (const polynomial op);
	  void operator = (const polynomial & other);
	  byte & operator[] (const int i);
	  const byte get_coefficient (const int i) const;
	  const std::vector<byte>  get_coefficients () const;
	  polynomial operator<< (const unsigned int i);
	  void operator<<= (const unsigned int i);
	  polynomial operator>> (const unsigned int i);
	  void operator>>= (const unsigned int i);
	  const size_t terms() const;
     };
}
#endif

