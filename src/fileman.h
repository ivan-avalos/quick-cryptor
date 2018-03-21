/*
 * fileman.h
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

#ifndef _FILEMAN_H_
#define _FILEMAN_H_

#include <stdio.h>
#include <stdlib.h>

enum FilemanErrno
{
	 FERRNO_FILE = 1
};

typedef struct {
	char* input_path;
	char* output_path;
	FILE* input_file;
	FILE* output_file;
	/* Input */
	char* input_content;
	size_t input_size;
 } fileman_t;

fileman_t* fileman_new_empty (void);
fileman_t* fileman_new (char* input_path, char* output_path);
void fileman_set_input_path (fileman_t* fileman, char* input_path);
void fileman_set_output_path (fileman_t* fileman, char* output_path);
int fileman_read_input (fileman_t* fileman);
int fileman_write_output (fileman_t* fileman, char* output, size_t size);
void fileman_free (fileman_t* fileman);

#endif