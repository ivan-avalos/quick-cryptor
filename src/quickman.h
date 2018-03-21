/*
 * quickman.h
 *
 * Copyright (C) 2018 - Ivan Avalos
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QUICKMAN_H_
#define _QUICKMAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum QuickmanErrno
{
	 QERRNO_FILESIZE = 3
};

char* quickman_key_generate (size_t length);
char* quickman_xor_cipher (char* a, size_t za, char* b, size_t zb);
char* quickman_xor_decipher (char* a, size_t za, char* b, size_t zb);
char* quickman_hex_encode (char* a, size_t size);
char* quickman_hex_decode (char* a, size_t size);
char* quickman_hex_single_encode (unsigned char a);
unsigned char quickman_hex_single_decode (char* a);

#endif