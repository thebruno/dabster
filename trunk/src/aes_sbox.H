// Sigma Dabster 5
// Copyright (C) 2007  Zespó³ Dabstera

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 dated June, 1991. 

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details. 

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA. 

#ifndef _H_SBOX
#define _H_SBOX

#include "aes_bytes.H"
#include "aes_polynomial.H"
#include "aes_state.H"

//#include <map>

#include <vector>

namespace aes
{
     
     class subst_box
     {
	  static byte s_box[256];
	  static byte inv_s_box[256];
	  
     public:
	  void sub_word (polynomial & word);
	  void inv_sub_word (polynomial & word);

	  void sub_bytes (state & state);
	  byte sub_byte (byte c);

	  byte inv_sub_byte (byte c);
	  void inv_sub_bytes (state & state);
     };

} // namespace aes;

#endif
