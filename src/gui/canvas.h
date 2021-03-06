#ifndef CANVAS_H
#define CANVAS_H

/**
 * @file    canvas.h
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   The canvas repesents the output image that was generated
 *
 * @section DESCRIPTION
 *
 * This file contains the canvas_t class, which is used to represent
 * the output image that was generated by processing, as observed from
 * the scene's camera.
 *
 * The canvas stores the aggregate colors and has the ability to export
 * to file.
 */

#include <color/color.h>
#include <string>
#include <vector>

/**
 * The canvas_t class is used to represent the output image
 */
class canvas_t
{
	/* parameters */
	private:

		/**
		 * The width of the output image
		 */
		size_t width;

		/**
		 * The height of the output image
		 */
		size_t height;

		/**
		 * The rgb components of the image
		 *
		 * Represented as a vector of length width*height,
		 * the rgb components of each pixel (in row-major order)
		 */
		std::vector<color_t> pixels;

		/**
		 * The count of number of rays that have contributed to
		 * each pixel.
		 */
		std::vector<size_t> counts;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Initializes empty canvas
		 */
		canvas_t() : width(0), height(0), pixels(), counts()
		{};

		/**
		 * Initializes the canvas to be blank at given resolution
		 *
		 * @param w    The width of the canvas to use
		 * @param h    The height of the canvas to use
		 */
		canvas_t(size_t w, size_t h);

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Clears all values from this canvas
		 */
		void clear();

		/**
		 * Sets the size of the canvas, which will clear contents
		 *
		 * @param w     The width of the canvas to use
		 * @param h     The height of the canvas to use
		 */
		void set_size(size_t w, size_t h);

		/**
		 * Retrieves the width of this canvas
		 */
		inline size_t get_width() const
		{ return this->width; };

		/**
		 * Retrieves the height of this canvas
		 */
		inline size_t get_height() const
		{ return this->height; };

		/**
		 * Adds a color sample to the specified pixel of this canvas
		 * 
		 * @param i   The horizontal index of the pixel
		 * @param j   The vertical index of the pixel
		 * @param c   The color sample to add
		 */
		void add_pixel(size_t i, size_t j, const color_t& c);

		/**
		 * Adds a color sample to this canvas
		 * 
		 * @param u   The horizontal coordinate [0,1] of the sample
		 * @param v   The vertical coordinate [0,1] of the sample
		 * @param c   The color sample to add
		 */
		void add_coord(float u, float v, const color_t& c);
		
		/*-----*/
		/* i/o */
		/*-----*/

		/**
		 * Exports canvas to the given PNG image
		 *
		 * Will export the contents of the canvas to the
		 * given png file location.
		 *
		 * @param filename    Where to write the PNG image
		 *
		 * @return     Returns zero on success, non-zero on failure.
		 */
		int writepng(const std::string& filename) const;

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * The following function is a unit-test for the canvas
		 *
		 * It will populate the canvas image with a pattern and
		 * export the image.
		 */
		static void unittest();
};

#endif
