/*
 * cbm_interference: main.c
 *
 *  Version 1.0
 *  Created on: 28.11.2014
 *  Last modified: 29.11.2014 14:14
 *      Author: Dennis Freise
 */

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "configfile.h"
#include "outputfile.h"
#include "render.h"

/*
 * Synopsis: Gibt kurzen Hilfstext aus
 * Parameter:
 * 1. char *exe_name: Zeiger auf einen String, der den Namen des Executables enthält
 */
void print_help( char *exe_name ) {
	printf("Syntax: %s <Config file> <Output file>\n", exe_name);
}

// Hauptfunktion des Programmes
int main( int argc, char **argv ) {
	if( argc == 2 ) {
		if( !strcmp( argv[1], "--help" ) ) {
			print_help( argv[0] );
			return( EXIT_SUCCESS );
		}
	}

	if( argc != 3 ) {
		printf("FEHLER: Falsche Anzahl von Parametern.\n");
		print_help( argv[0] );
		return( EXIT_FAILURE );
	}

	char *definitions_file = argv[1];
	char *output_file = argv[2];

	// Module configfile.c:
	struct scene *renderScene = read_configfile( definitions_file );
	if( ! renderScene ) {
		printf("FEHLER: Definitionsdatei enth\204lt einen Fehler.\n");
		return( EXIT_FAILURE );
	}

	// Module render.c:
	byte *output_picture = RenderPicture( renderScene );
	if( !output_picture ) {
		clean_scene( renderScene );
		printf("FEHLER: Beim Render ging etwas schief :-(\n");
		return( EXIT_FAILURE );
	}

	// Module outputfile.c:
	if( !write_output_file( output_file, output_picture, renderScene ) ) {
		free( output_picture );
		clean_scene( renderScene );
		printf("FEHLER: Beim Speichern der Bilddatei ging etwas schief.\n");
		return( EXIT_FAILURE );
	}

	return( EXIT_SUCCESS );
}
