/*
 * configfile.h
 *
 *  Created on: 02.12.2014
 *      Author: it
 */

#ifndef SOURCE_CONFIGFILE_H_
#define SOURCE_CONFIGFILE_H_

#include <stdbool.h>

// Structures
struct source {
	unsigned int	x;
	unsigned int	y;

	unsigned int	amplitude;
	unsigned int	wavelength;
	unsigned int	fade_factor;

	struct source	*next_source;
};

struct scene {
	unsigned int	width;
	unsigned int	height;

	struct source	*first_source;
};

// Prototypes
struct scene *read_configfile( const char *filename );
void clean_scene( struct scene *Scene );
bool parse_scene( char *in_buffer, struct scene *Scene );

#endif /* SOURCE_CONFIGFILE_H_ */
