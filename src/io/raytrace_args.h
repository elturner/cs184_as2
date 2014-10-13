#ifndef RAYTRACE_ARGS_H
#define RAYTRACE_ARGS_H

/**
 * @file   raytrace_args.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Parses command-line arguments
 *
 * @section DESCRIPTION
 *
 * This file contains the raytrace_args_t class, used to parse
 * command-line arguments for the raytracer program
 */

#include <string>
#include <vector>

/**
 * The raytrace_args_t represents the input arguments to this program
 */
class raytrace_args_t
{
	/* parameters */
	public:

		/**
		 * The input .txt file that contains the scene information
		 *
		 * If more than one file is given, then the scene will be
		 * populated with all the files, in order, by concatenating
		 * their contents.
		 */
		std::vector<std::string> infiles;

		/**
		 * The output image files
		 * 
		 * The generated image will be written to each output
		 * file given.  The files must be of format .png
		 */
		std::vector<std::string> outfiles;

		/**
		 * The following value indicates how many samples to use
		 * per pixel.  If this value is N, then a NxN grid will
		 * be sampled at each pixel.
		 */
		size_t samples_per_pixel;

		/**
		 * The following specifies the output dimensions
		 *
		 * This specifies the output image width, in pixels
		 */
		size_t output_image_width;

		/**
		 * This specifies the output image height, in pixels
		 */
		size_t output_image_height;

		/**
		 * The recursion depth to use for raytracing
		 */
		int recursion_depth;

		/**
		 * Whether or not to use normalmap shading (for
		 * debugging purposes)
		 */
		bool debug;

	/* functions */
	public:

		/**
		 * Parses the command-line arguments of this program
		 *
		 * Given the input command-line arguments, will
		 * parse them and populate the values of this structure.
		 *
		 * @param argc  Number of command-line args (including
		 *              program name)
		 * @param argv  The command-line arguments
		 *
		 * @return      Returns zero on success, non-zero on failure
		 */
		int parse(int argc, char** argv);
};

#endif
