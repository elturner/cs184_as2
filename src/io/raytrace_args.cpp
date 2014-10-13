#include "raytrace_args.h"
#include <util/cmd_args.h>
#include <util/error_codes.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * @file   raytrace_args.cpp
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Parses command-line arguments
 *
 * @section DESCRIPTION
 *
 * This file contains the raytrace_args_t class, used to parse
 * command-line arguments for the raytracer program
 */

using namespace std;

/* the following flags are used for this program */

#define SAMPLES_PER_PIXEL_FLAG "-s"
#define IMAGE_DIMS_FLAG        "-d"
#define RECURSION_DEPTH_FLAG   "-r"
#define DEBUG_FLAG             "--debug"

/* the following file types are required for this program */

#define TXT_FILE_EXT  "txt"
#define PNG_FILE_EXT  "png"

/*--------------------------*/
/* function implementations */
/*--------------------------*/

int raytrace_args_t::parse(int argc, char** argv)
{
	cmd_args_t args;
	int ret;

	/* initialze the values of this structure before
	 * we parse anything */
	this->infiles.clear();
	this->outfiles.clear();
	this->samples_per_pixel = 2;
	this->output_image_width = 1000;
	this->output_image_height = 1000;
	this->recursion_depth = 2;
	this->debug = false;

	/* prepare the command-args parser for this program */
	args.set_program_description("This program is a raytracer.  It "
			"will produce an image from a given input scene.  "
			"The scene is specified by an input .txt config "
			"file.  The output image must be a PNG.");
	args.add_required_file_type(TXT_FILE_EXT, 1, "The input config "
			"file that specifies the geometry, camera, and "
			"lighting for the scene to render.");
	args.add_required_file_type(PNG_FILE_EXT, 1, "The output image "
			"to export.  If multiple files are specified, the "
			"same image will be exported to each of them.");
	args.add(SAMPLES_PER_PIXEL_FLAG, "Specifies the number of samples "
			"generated for each pixel in the output image.  "
			"The pixel will be sampled with a NxN grid with "
			"jitter.\n\n\t" SAMPLES_PER_PIXEL_FLAG " <N>",
			true, 1);
	args.add(IMAGE_DIMS_FLAG, "Specifies the dimensions of the output "
			"image, in units of pixels.\n\n\t"
			IMAGE_DIMS_FLAG " <width> <height>", true, 2);
	args.add(RECURSION_DEPTH_FLAG, "Specifies the recusion depth for "
			"raytracing.  This determines how many times each "
			"ray can bounce off of reflective surfaces.\n\n\t"
			RECURSION_DEPTH_FLAG " <num_bounces>", true, 1);
	args.add(DEBUG_FLAG, "If seen, will render the scene using "
			"a simplified shader, via normalmap shading.  This "
			"is useful for debugging scene elements.", true, 0);

	/* parse the input values */
	ret = args.parse(argc, argv);
	if(ret)
	{
		/* report error */
		cerr << "[raytrace_args_t::parse]\tError: " << ret 
		     << "\tUnable to parse input arguments" << endl;
		return PROPEGATE_ERROR(-1, ret);
	}

	/* populate this structure with the parsed arguments */
	args.files_of_type(TXT_FILE_EXT, this->infiles);
	args.files_of_type(PNG_FILE_EXT, this->outfiles);
	if(args.tag_seen(SAMPLES_PER_PIXEL_FLAG))
		this->samples_per_pixel = args.get_val_as<size_t>(
					SAMPLES_PER_PIXEL_FLAG);
	if(args.tag_seen(IMAGE_DIMS_FLAG))
	{
		this->output_image_width = args.get_val_as<size_t>(
					IMAGE_DIMS_FLAG, 0);
		this->output_image_height = args.get_val_as<size_t>(
					IMAGE_DIMS_FLAG, 1);
	}
	if(args.tag_seen(RECURSION_DEPTH_FLAG))
		this->recursion_depth = args.get_val_as<int>(
					RECURSION_DEPTH_FLAG);
	this->debug = args.tag_seen(DEBUG_FLAG);

	/* return success */
	return 0;
}
