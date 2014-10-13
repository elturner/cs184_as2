#include <iostream>
#include <io/raytrace_args.h>
#include <gui/canvas.h>
#include <gui/sampler.h>
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
	raytrace_args_t args;
	canvas_t canvas;
	sampler_t sampler;
	scene_t scene;
	size_t i, n, r, c;
	float u, v;
	int ret;

	/* parse the args */
	ret = args.parse(argc, argv);
	if(ret)
		return 1;

	/* initialize the classes */
	canvas.set_size(args.output_image_width, args.output_image_height);
	sampler.init(args.output_image_width, args.output_image_height, 
			args.samples_per_pixel);

	/* initialize the scene */
	n = args.infiles.size();
	for(i = 0; i < n; i++)
	{
		/* import each given file */
		ret = scene.init(args.infiles[i], args.recursion_depth,
				args.debug);
		if(ret)
		{
			/* error occurred while initializing */
			cerr << "[main]\tUnable to initialize scene from "
			     << "input file: " << args.infiles[i] << endl;
			return 2;
		}
	}

	/* render the scene by generating rays using the sampler */
	while(!(sampler.is_done()))
	{
		/* sample a coordinate from the pixel */
		sampler.next(c, r, u, v);

		/* raytrace for this pixel */
		canvas.add_pixel(c, r, scene.trace(u, v));
	}

	/* export the canvas to the output image(s) */
	n = args.outfiles.size();
	for(i = 0; i < n; i++)
		canvas.writepng(args.outfiles[i]);
	
	/* success */
	return 0;
}
