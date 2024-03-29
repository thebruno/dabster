// Sigma Dabster 5
// Copyright (C) 2007  Zesp� Dabstera

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

// This file contains macros useful when debugging the AES-related
// parts of Sigma Dabster 5.

///
/// @file   aes_macros.H
/// @author Jan Swierzawski
/// @date   Sat May 12 15:05:43 2007
/// 
/// @brief  makra do debugowania poszczeg�lnych element�w szyfrowania AES.
/// 
/// Makra w tym pliku s�u�� wy��cznie do debugowania, i przede
/// wszystkim w��czaj� funkcje main() w odpowiednich plikach.
///
/// Modu�, kt�ry ma by� debugowany jest wybierany w pliku Makefile
///

#ifndef AES_MACROS
#define AES_MACROS

#define DDABSTER 0
#define DAES 1
#define DSTATE 2
#define DSBOX 3
#define DBYTES 4
#define DPOLYNOMIAL 5
#define DKEY 6


// Default key length in bits:
#define AES_DEFAULT_KEYLEN 256
// uint by�o zachowane dla zgodno�ci ze starymi kompilatorami C, i
// s�dzi�em �e jest w standardzie.
typedef unsigned int uint ;
#endif	 // AES_MACROS
