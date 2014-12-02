/*
 * configfile.c
 *
 *  Created on: 02.12.2014
 *      Author: it
 */

#ifndef SOURCE_CONFIGFILE_C_
#define SOURCE_CONFIGFILE_C_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "configfile.h"

struct scene *read_configfile( const char *filename ) {
	FILE *fileHandle = fopen(filename, "rb"); // needs to be opened in binary mode, else ftell() returns a cookie

	if( !fileHandle ) {
		return NULL;
	}

	// Get size of the configfile
	fseek(fileHandle, 0L, SEEK_END);
	unsigned int fileSize = ftell(fileHandle);
	fseek(fileHandle, 0L, SEEK_SET);

	// Alloc buffer and read file
	char *input_buffer = malloc( fileSize + 1 );
	memset( input_buffer, 0, (fileSize + 1) );
	unsigned int read_bytes = fread( input_buffer, 1, fileSize, fileHandle );
	fclose( fileHandle );

	if( read_bytes != fileSize ) {
		free( input_buffer );
		return NULL;
	}

	// Parse file
	struct scene *new_scene = malloc( sizeof( struct scene ) );
	if( !new_scene )
		return NULL;

	new_scene->width		= 0;
	new_scene->height		= 0;
	new_scene->first_source = NULL;

	if( parse_scene( input_buffer, new_scene ) )
		return new_scene;
	else {
		clean_scene( new_scene );
		return NULL;
	}
}

void clean_scene( struct scene *Scene ) {
	if( Scene ) {
		struct source *tmpSource = Scene->first_source;

		while( tmpSource ) {
			struct source *tmpSource2 = tmpSource;
			tmpSource = tmpSource->next_source;
			free( tmpSource2 );
		}
		free( Scene );
	}
}

bool parse_scene( char *in_buffer, struct scene *Scene ) {
	if( !in_buffer || !Scene )
		return false;

	int count_lines = 0;
	for( int c = 0; in_buffer[c]; c++ ) {
		if( in_buffer[c] == '\n' )
			count_lines++;
	}

	char *lines[count_lines];
	lines[0] = in_buffer;
	int j = 1;

	for( int i = 0; in_buffer[i]; i++ ) {
		if( in_buffer[i] == '\r' || in_buffer[i] == '\n' ) {
			in_buffer[i] = 0;
			if(in_buffer[i+1] && in_buffer[i+1] != '\r' && in_buffer[i+1] != '\n' )
				lines[j++] = &in_buffer[i+1];
		}
	}

	int temp_integer = 0;
	int x=0, y=0, ampl=0, wvlength=0, fade=0;
	struct source *last_source = NULL;

	for( int i = 0; i < count_lines; i++ ){
		if( sscanf( lines[i], "[WIDTH:%i]", &temp_integer ) )
			Scene->width = temp_integer;
		else if( sscanf( lines[i], "[HEIGHT:%i]", &temp_integer ) )
			Scene->height = temp_integer;
		else if( sscanf( lines[i], "[SOURCE:%i,%i,%i,%i,%i]", &x, &y, &ampl, &wvlength, &fade) ) {
			struct source *new_source = malloc( sizeof( struct source ) );
			if( !new_source )
				return false;

			new_source->x = x;
			new_source->y = y;
			new_source->amplitude = ampl;
			new_source->wavelength = wvlength;
			new_source->next_source = NULL;

			if( last_source )
				last_source->next_source = new_source;

			last_source = new_source;

			if( Scene->first_source == NULL )
				Scene->first_source = new_source;
		}
	}
	return true;
}

#endif /* SOURCE_CONFIGFILE_C_ */
