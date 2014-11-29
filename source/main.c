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
			return 0;
		}
	}

	if( argc != 3 ) {
		printf("FEHLER: Falsche Anzahl von Parametern.\n");
		print_help( argv[0] );
		return 1;
	}

	char *definitions_file = argv[1];
	char *output_file = argv[2];

	// Modul file_input:
	//read_configfile(definitions_file);

	// Modul calc:
	// Render(config_array);

	// Modul file_output:
	//write_file(...);

	return 0;
}
