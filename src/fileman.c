/*
 * fileman.c
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

#include "fileman.h"

fileman_t* fileman_new_empty (void)
{
	fileman_t* fileman = malloc (sizeof (fileman_t));
	fileman->input_path = NULL;
	fileman->output_path = NULL;
	fileman->input_content = NULL;
	return fileman;
}

fileman_t* fileman_new (char* input_path, char* output_path)
{
	fileman_t* fileman = malloc (sizeof (fileman_t));
	fileman->input_path = input_path;
	fileman->output_path = output_path;
	fileman->input_content = NULL;
	return fileman;
}

void fileman_set_input_path (fileman_t* fileman, char* input_path)
{
	fileman->input_path = input_path;
}

void fileman_set_output_path (fileman_t* fileman, char* output_path)
{
	fileman->output_path = output_path;
}

int fileman_read_input (fileman_t* fileman)
{
	/* Open file */
	if((fileman->input_file = fopen (fileman->input_path, "r")) == NULL)
	{
		return FERRNO_FILE;
	} else {

		/* Get file size */
		size_t size;
		fseek (fileman->input_file, 0L, SEEK_END);
		size = ftell(fileman->input_file);
		rewind (fileman->input_file);
		fileman->input_size = size;

		/* Allocate char array for storing file data */
		fileman->input_content = (char*) malloc (sizeof(char)*size);

		/* Read file data and store */
		fread (fileman->input_content, sizeof(char), size, fileman->input_file);

		/* Close file */
		fclose (fileman->input_file);
	}

	return 0;
}

int fileman_write_output (fileman_t* fileman, char* output, size_t size)
{
	if((fileman->output_file = fopen (fileman->output_path, "w")) == NULL)
	{
		return FERRNO_FILE;
	} else {

		/* Write output to file */
		fwrite (output, sizeof(char), size, fileman->output_file);

		/* Close file */
		fclose (fileman->output_file);
	}

	return 0;
}

void fileman_free (fileman_t* fileman)
{
	free (fileman);
}