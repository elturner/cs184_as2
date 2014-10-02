#include "sampler.h"
#include <stdlib.h>

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
		
/*--------------------------*/
/* function implementations */
/*--------------------------*/

void sampler_t::init(size_t w, size_t h, size_t n)
{
	size_t i;

	/* populate random table */
	for(i = 0; i < sampler_t::TABLE_SIZE; i++)
		this->rand_table[i] = rand();

	/* store the given values */
	this->image_width           = w;
	this->image_height          = h;
	this->num_pixels            = w*h;
	this->pixel_width           = 1.0f / w;
	this->pixel_height          = 1.0f / h;
	this->samples_per_pixel_dir = n;
	this->samples_per_pixel     = n*n;
	this->subpixel_width        = 1.0f / (w*n);
	this->subpixel_height       = 1.0f / (h*n);

	/* initialize all indices to start at first pixel */
	this->next_rand             = 0;
	this->curr_pixel            = 0;
	this->curr_pixel_sample     = 0;
}

void sampler_t::next(size_t& c, size_t& r, float& u, float& v)
{
	size_t sr, sc;
	float ju, jv;

	/* compute the next pixel (r,c) coordinates */
	r  = this->curr_pixel / this->image_width;
	c  = this->curr_pixel % this->image_width;

	/* get the sub-pixel index location (sr, sc) */
	sr = this->curr_pixel_sample / this->samples_per_pixel_dir;
	sc = this->curr_pixel_sample % this->samples_per_pixel_dir;

	/* get the image (u,v) coordinates without any randomness */
	u = (c * this->pixel_width)  + ((sc+0.5f) * this->subpixel_width);
	v = (r * this->pixel_height) + ((sr+0.5f) * this->subpixel_height);

	/* compute random jitter for sample of size subpixel*[-0.5,0.5] */
	ju = this->subpixel_width 
		* (this->rand_table[this->next_rand] % 100 - 50) / 100.0f;
	jv = this->subpixel_height 
		* (this->rand_table[1+this->next_rand] % 100 - 50) / 100.0f;

	/* add jitter to sample coordinates */
	u += ju;
	v += jv;

	/* update indices for next time */
	this->curr_pixel_sample++;
	if(this->curr_pixel_sample >= this->samples_per_pixel)
	{
		/* move to next pixel */
		this->curr_pixel_sample = 0;
		this->curr_pixel++;
	}

	/* update random number to use next time */
	this->next_rand += 2;
	if(this->next_rand >= sampler_t::TABLE_SIZE)
		this->next_rand = 0;
}
