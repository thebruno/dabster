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
/// @file   aes_base.H
/// @author Jan Swierzawski
/// @date   Sat May 19 03:43:14 2007
///
/// @brief  w³a¶ciwy algorytm AES
///
/// Klasa aes::aes_base scala wszystkie podklasy algorytmu AES
/// (aes::subst_box, aes::key i aes::state) w jedn± ca³o¶æ i
/// udostêpnia na zewn±trz dwie metody - szyfrowanie i deszyfrowanie
/// jednego bloku. Ca³a reszta nie mo¿e odbywaæ siê bez udzia³u reszty
/// Dabstera, i w  zwi±zku z tym odbywa siê w klasie aes::aes, która
/// jest przewidziana w projekcie.
///

#ifndef AES_BASE
#define AES_BASE

#include "aes_bytes.H"
#include "aes_polynomial.H"
#include "aes_key.H"
#include "aes_sbox.H"
#include "aes_state.H"
#include "aes_macros.H"

#include <iosfwd>

namespace aes
{
     // klasa s³u¿y do synchronizacji informacji pomiêdzy klasami
     // aes::state, aes::subst_box oraz aes::key.
     class aes_base
     {
	  bool ciphered;
	  subst_box The_Sbox;
	  state The_State;
	  key The_Key;
	  uint current_round_;
	  const uint current_round() const;
	  uint update_round(uint i);
	  const uint key_length() const;
	  uint key_length (uint keylen);
	  const uint Nb() const;
	  const uint Nk() const;
	  const uint Nr() const;

	  void __cipher();
	  void __inv_cipher();
	  // szyfrowanie w któr±¶ ze stron
	  void _cph (std::vector <byte> & input, const std::vector <byte> key,
		     void (aes::aes_base::* ) ());
     public:
	  aes_base (std::string key) : The_Key (key) {};
	  aes_base (char * key, size_t length) : The_Key (key, length) {};
	  aes_base (std::vector <byte> key) : The_Key (key) {};
	  aes_base (std::vector <byte> key, std::vector <byte> & data);
	  aes_base (std::string key, std::vector <byte> & data);
	  
	  void set_key (const std::string key);
	  void set_key (const std::vector <byte> key);
	  void set_key (const std::vector <char> key);
	  void set_key (const char * key, size_t length);

	  state get_block() { return The_State; };
	  void cipher();
	  void cipher(std::ifstream & in, std::ofstream & out);
	  void cipher (std::vector <byte> & input, const std::vector <byte> key);

	  void decipher();
	  void decipher (std::ifstream & in, std::ofstream & out);
	  void decipher (std::vector <byte> & input,
			 const std::vector <byte> key);
	  bool is_ciphered() { return ciphered; };
     };
}
#endif	 // AES_BASE
