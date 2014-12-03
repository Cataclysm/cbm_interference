/*
 * render.c
 *
 *  Created on: 02.12.2014
 *      Author: it
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#include "configfile.h"
#include "render.h"

#define M_PI		3.14159265358979323846

byte *RenderPicture( struct scene *Scene ) {
	byte *output_buffer = malloc( Scene->width * Scene->height * 3 );

	if( !output_buffer ) {
		return NULL;
	}

	int count_sources = 0;
	struct source *tmpSource = Scene->first_source;

	while( tmpSource ) {
		count_sources++;
		tmpSource = tmpSource->next_source;
	}

	struct source *Sources[count_sources];
	tmpSource = Scene->first_source;
	int i = 0;
	while( tmpSource ) {
		Sources[i++] = tmpSource;
		tmpSource = tmpSource->next_source;
	}

	for( int iHeight = 0; iHeight < Scene->height; iHeight++ ) { // main loop: all rows (y)
		for( int iWidth = 0; iWidth < Scene->width; iWidth++ ) { // main loop: all column (x)
			double sum_amplitude_r = 0.0f;
			double sum_amplitude_g = 0.0f;
			double sum_amplitude_b = 0.0f;

			for( int iSources = 0; iSources < count_sources; iSources++ ) { // main loop: all sources
				double distance = sqrt( pow(abs(Sources[iSources]->x-iWidth),2) + pow(abs(Sources[iSources]->y-iHeight),2) );

				double Fade_Factor = ((double)Sources[iSources]->fade_factor - distance) / Sources[iSources]->fade_factor;
				if( Fade_Factor < 0.0f )
					Fade_Factor = 0.0f;

				double pixelResult = (sin( distance * 2.0f * M_PI / Sources[iSources]->wavelength ) + 1) * Sources[iSources]->amplitude_r; // amplitude between 0 and 128
				sum_amplitude_r += ( pixelResult * Fade_Factor );

				pixelResult = (sin( distance * 2.0f * M_PI / Sources[iSources]->wavelength ) + 1) * Sources[iSources]->amplitude_g; // amplitude between 0 and 128
				sum_amplitude_g += ( pixelResult * Fade_Factor );

				pixelResult = (sin( distance * 2.0f * M_PI / Sources[iSources]->wavelength ) + 1) * Sources[iSources]->amplitude_b; // amplitude between 0 and 128
				sum_amplitude_b += ( pixelResult * Fade_Factor );
			}

			if( sum_amplitude_r > 255.0f )
				sum_amplitude_r = 255.0f;
			if( sum_amplitude_g > 255.0f )
				sum_amplitude_g = 255.0f;
			if( sum_amplitude_b > 255.0f )
				sum_amplitude_b = 255.0f;

			int pixel_position = ((Scene->width*iHeight)+iWidth)*3;

			output_buffer[pixel_position]   = sum_amplitude_b;	// B
			output_buffer[pixel_position+1] = sum_amplitude_g;	// G
			output_buffer[pixel_position+2] = sum_amplitude_r;	// R
		}
	}

	return output_buffer;
}
