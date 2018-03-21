/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2018 Ivan Avalos <ivan.avalos.diaz@hotmail.com>
 * 
 * quick-cryptor is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * quick-cryptor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

#include "fileman.h"
#include "quickman.h"

char* help_message =
"QuickCryptor 0.1 (alpha)\n\n"
"Usage: %s -h\n"
"Or   : %s [-c|-d] -i <file> -k <file> -o <file>\n"
"Or   : %s -g <file> -s <value>\n"
"Options:\n"
"\t-h         Display this help menu\n"
"\t-c         Cipher file with a key\n"
"\t-i <file>  Input file\n"
"\t-k <file>  Key file\n"
"\t-o <file>  Output file\n"
"\t-g <file>  Generate a key on <file> of '-s' <size>\n"
"\t-s <value> Size of the key (output: size x2)\n\n";

void print_help (char* argv0);

int main(int argc, char* argv[])
{

	/* Get user parameters */
	int c, gflag = 0, cflag = 0, dflag = 0;
	size_t s;
	char *g, *i, *k, *o;
	
	while((c = getopt(argc, argv, "hg:s:cdi:k:o:")) != -1)
	{
		switch (c)
		{
			/* Help */
			case 'h':
				print_help (argv[0]);
				return 0;
				
			/* Operations */
			case 'g':
				// Generate key
				if (cflag || dflag)
				{
					print_help (argv[0]);
					return 2;
				}
				gflag = 1;
				g = optarg;
				break;
			case 's':
				// Set size of key
				if (!gflag)
				{
					print_help (argv[0]);
					return 2;
				}
				sscanf (optarg, "%zu\n", &s);
				break;
			case 'c':
				// Cipher operation selected
				if (gflag || dflag)
				{
					print_help (argv[0]);
					return 2;
				}
				cflag = 1;
				break;
			case 'd':
				// Decipher operation selected
				if (gflag || cflag)
				{
					print_help (argv[0]);
					return 2;
				}
				dflag = 1;
				break;
				
			/* Input / Key / Output */
			case 'i':
				// Input file path
				i = optarg;
				break;
			case 'k':
				// Key file path
				k = optarg;
				break;
			case 'o':
				// Output file path
				o = optarg;
				break;

			/* Error handling */
			case '?':
				// Invalid arguments
				if(!gflag) {
					if (optopt == 'i' || optopt == 'k' || optopt == 'o') {
						fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
					}
				} else {
					if (optopt == 'g' || optopt == 's') {
						fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
					}
				}

				print_help (argv[0]);

				return (2);
				
			default:
				abort ();
		}
	}

	/* Do correspondent operation */
	if (gflag)
	{
		fileman_t* fileman;
		char* key;

		/* Create fileman instance */
		fileman = fileman_new_empty ();
		fileman_set_output_path (fileman, g);

		/* Generate key */
		key = quickman_key_generate (s);

		/* Write to output */
		if (fileman_write_output (fileman, key, s*2) == FERRNO_FILE) {
			fprintf(stderr, "ERROR: Could not write key to file\n");
			return FERRNO_FILE;
		}
		
		/* Free memory */
		fileman_free (fileman);
		free (key);
	}
	else if (cflag || dflag) {
		
		fileman_t *io_fileman, *key_fileman;
		char* output = NULL;
		size_t output_size = 0;

		/* Create fileman instances */
		io_fileman = fileman_new (i, o);
		key_fileman = fileman_new_empty ();
		fileman_set_input_path (key_fileman, k);

		/* Read inputs (input & key) */
		if (fileman_read_input (io_fileman) == FERRNO_FILE)
		{
			fprintf(stderr, "ERROR: Could not read input file\n");
			return FERRNO_FILE;
		}

		if (fileman_read_input (key_fileman) == FERRNO_FILE)
		{
			fprintf(stderr, "ERROR: Could not read key file\n");
			return FERRNO_FILE;
		}

		if(cflag)
		{
			/* Cipher input with key */
			output = quickman_xor_cipher (io_fileman->input_content,
				                          io_fileman->input_size,
				                          key_fileman->input_content,
				                          key_fileman->input_size);
			
			output_size = io_fileman->input_size * 2;
		}
		else if (dflag)
		{
			/* Decipher input with key */
			output = quickman_xor_decipher (io_fileman->input_content,
				                            io_fileman->input_size,
				                            key_fileman->input_content,
				                            key_fileman->input_size);

			output_size = io_fileman->input_size / 2;
		}

		/* Check error */
		if (output == NULL) 
		{
			fprintf (stderr, "ERROR: Key needs to be larger or equal to input file.\n");
			return QERRNO_FILESIZE;
		}
		
		/* Write to output */
		if (fileman_write_output (io_fileman, output,
		                          output_size) == FERRNO_FILE) {
			fprintf(stderr, "ERROR: Could not write cipher to file\n");
			return FERRNO_FILE;
		}

		/* Free memory */
		fileman_free (io_fileman);
		fileman_free (key_fileman);
	} else {
		print_help (argv[0]);
	}
	
	return (0);
}

void print_help(char* argv0)
{
	printf (help_message, argv0, argv0, argv0);
}
