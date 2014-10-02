#include <iostream>
#include <gui/canvas.h>
#include <scene/scene.h>

/**
 * @file    main.cpp
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   Main file for CS-184 Assignment #2, Fall 2014
 *
 * @section DESCRIPTION
 *
 * This program is written for berkeley CS-184, Assignment #2,
 * which performs ray tracing on a scene.
 */

using namespace std;

/* function implementations */

/**
 * The main function for this program
 */
int main(int argc, char** argv)
{
	canvas_t canvas;
	scene_t scene;
	int ret;

	// TODO do a basic test
	size_t r, c, width, height;
	float u, v;
	width = height = 2000;
	canvas.set_size(width, height);
	scene.init("");
	for(r = 0; r < height; r++)
		for(c = 0; c < width; c++)
		{
			/* sample a coordinate from the pixel */
			u = ((1.0f * (c + 0.5f)) / width);
			v = ((1.0f * (r + 0.5f)) / height);

			/* raytrace for this pixel */
			canvas.add_pixel(c, r, scene.trace(u, v));
		}
	canvas.writepng("unittest2.png");
	
	/* success */
	return 0;
}
