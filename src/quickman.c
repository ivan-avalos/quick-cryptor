/*
 * quickman.c
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

#include "quickman.h"

const char q_hex_chars[] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'a', 'b',
			'c', 'd', 'e', 'f' };

char* quickman_key_generate(size_t length)
{
	char *semi_output = NULL, *output = NULL;

	/* Initialize random generator with time as seed */
	srand ((unsigned) time(NULL));

	semi_output = (char*) malloc (sizeof(char)*length);

	int i;
	for (i=0;i<length;i++)
	{
		semi_output[i] = rand ();
	}

	/* Encode as hexadecimal and return */
	output = quickman_hex_encode (semi_output, length);

	/* Free memory */
	free (semi_output);

	return output;
}
char* quickman_xor_cipher(char* a, size_t za, char* b, size_t zb)
{
	char *semi_output = NULL, *output = NULL;
	
	/* Compare sizes (b should be equal or more than a) */
	if(zb >= za) {
		
		/* Cipher and store partial result */
		semi_output = (char*) malloc(sizeof(char)*za);
		int i;
		for(i=0;i<za;i++) 
		{
			semi_output[i] = a[i] ^ b[i];
		}

		/* Encode as hexadecimal and store final result */
		output = quickman_hex_encode (semi_output, za);

		/* Free memory */
		free (semi_output);
		
		return output;
	} else {
		return NULL;
	}
}

char* quickman_xor_decipher(char* a, size_t za, char* b, size_t zb)
{
	char* semi_output;
	char* output;
	
	/* Compare sizes (b should be equal or more than a/2) */
	if(zb >= za / 2) {

		/* Decode hexadecimal and store partial result */
		semi_output = quickman_hex_decode (a, za);

		/* Decipher and store final result */
		output = (char*) malloc((sizeof(char)*za)/2);
		int j;
		for (j=0;j<(za/2);j++)
		{
			output[j] = semi_output[j] ^ b[j];
		}

		/* Free memory */
		free (semi_output);

		return output;
	} else {
		return NULL;
	}
}

char* quickman_hex_encode(char* a, size_t size)
{
	char *output = NULL;
	
	output = (char*) malloc(sizeof(char)*(size*2));

	int i;
	for (i=0;i<(size*2);i+=2)
	{
		char* buffer = quickman_hex_single_encode (a[i/2]);
		output[i] = buffer [0];
		output[i+1] = buffer [1];
	}
	output[size*2] = '\0';

	return output;
}

char* quickman_hex_decode(char* a, size_t size)
{
	char* output;

	/* Error check */
	if(size % 2 != 0) return NULL;
	
	output = (char*) malloc(sizeof(char)*(size/2));
	output[size/2] = '\0';

	int i;
	for (i=0;i<size;i+=2)
	{
		
		char buffer[2];
		buffer[0] = a[i];
		buffer[1] = a[i+1];

		int c = i/2;
		output[c] = quickman_hex_single_decode (buffer);
	}

	return output;
}

char* quickman_hex_single_encode (unsigned char a)
{
	char* output = (char*) malloc (sizeof(char)*2);

	if (a == 0) {
		output[0] = '0';
		output[1] = '0';
	} else {
		/* Low nibble */
		output[1] = q_hex_chars [ a % 16 ];
		/* High nibble */
		output[0] = q_hex_chars [ (a / 16) % 16 ];
	}

	return output;
}

unsigned char quickman_hex_single_decode (char* a)
{
	char output = 0;
	
	/* 0 - 9 */
	if (a[0] >= 48 && a[0] <= 57)
		output += (a[0] - 48) * 16;
	/* a - f */
	else if (a[0] >= 97 && a[0] <= 102)
		output += (10 + (a[0] - 97)) * 16;
	else return 0;

	/* 0 - 9 */
	if (a[1] >= 48 && a[1] <= 57)
		output += (a[1] - 48);
	/* a - f */
	else if (a[1] >= 97 && a[1] <= 102)
		output += (10 + (a[1] - 97));
	else return 0;
	
	
	return output;
}
