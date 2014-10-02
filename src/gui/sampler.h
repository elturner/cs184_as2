#ifndef SAMPLER_H
#define SAMPLER_H

/**
 * @file   sampler.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Will compute randomly-sampled image coordinates on a canvas
 *
 * @section DESCRIPTION
 *
 * Will perform random sampling 'jitter' for each pixel in an image,
 * to allow a raytracer to get a monte carlo sampling of each pixel.
 */

#include <stdlib.h>

/**
 * The sampler_t class stores a random-number table and applies it to
 * subpixel coordinates
 */
class sampler_t
{
	/* parameters */
	private:

		/**
		 * Two times a (relatively) large prime number
		 *
		 * 10006 = 2 * 5003
		 */
		static const size_t TABLE_SIZE = 10006;

		/**
		 * A table of random numbers
		 */
		int rand_table[TABLE_SIZE];

		/**
		 * The index of the next random number to use
		 */
		size_t next_rand;
		
		/**
		 * The width of the image to sample
		 */
		size_t image_width;

		/**
		 * The height of the image to sample
		 */
		size_t image_height;

		/**
		 * Total number of pixels in the image
		 *
		 * This value just caches image_width * image_height
		 */
		size_t num_pixels;

		/**
		 * Cached value for 1.0f / image_width
		 */
		float pixel_width;

		/**
		 * Cached value for 1.0f / image_height
		 */
		float pixel_height;

		/**
		 * How many sample to use per pixel
		 *
		 * Each pixel will be sampled in a NxN grid,
		 * where this value specifies N.
		 */
		size_t samples_per_pixel_dir;

		/**
		 * Cached value for samples_per_pixel_dir^2
		 *
		 * This represents the total number of samples in a pixel
		 */
		size_t samples_per_pixel;

		/**
		 * Cached value for 1.0f/(image_width * samples_per_pixel)
		 */
		float subpixel_width;

		/**
		 * Cached value for 1.0f/(image_height * samples_per_pixel)
		 */
		float subpixel_height;

		/**
		 * The next pixel index to sample
		 */
		size_t curr_pixel;

		/**
		 * How many samples have been generated
		 * for the current pixel so far
		 */
		size_t curr_pixel_sample;

	/* functions */
	public:

		/**
		 * Resets this sampler's parameters, specific
		 * for the given canvas.
		 *
		 * This call will populate the table of random numbers
		 *
		 * This function should be called before retrieving
		 * samples from this structure.
		 *
		 * @param w   The image width to use
		 * @param h   The image height to use
		 * @param n   Specifies number of samples per pixel.
		 *            Each pixel will be sampled in a nxn grid.
		 */
		void init(size_t w, size_t h, size_t n);

		/**
		 * Retrieves the next sample as a image coordinate
		 *
		 * The output coordinate (u,v) will be a normalized
		 * image coordinate (so in the domain [0,1]^2).
		 *
		 * @param c   where to store the column index for the
		 *            pixel being sampled
		 * @param r   Where to store the row index for the pixel
		 *            being sampled
		 * @param u   Where to store the next u-value, 
		 *            horizontal [0,1]
		 * @param v   Where to store the next v-value, 
		 *            vertical [0,1]
		 */
		void next(size_t& c, size_t& r, float& u, float& v);

		/**
		 * Will return true after all samples have been generated
		 *
		 * This function will only return true after every
		 * pixel has been sampled n^2 times.
		 *
		 * @return    Returns true iff sampling is finished.
		 */
		inline bool is_done() const
		{
			/* check if we've reached the final pixel yet */
			return (this->curr_pixel >= this->num_pixels);
		};
};

#endif
