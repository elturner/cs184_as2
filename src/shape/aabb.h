#ifndef AABB_H
#define AABB_H

/**
 * @file   aabb.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Defines axis-aligned bounding box geometry for ray tracing
 *
 * @section DESCRIPTION
 *
 * This file defines the aabb_t class, which is used to represent
 * axis-aligned bounding boxes, for the purposes of ray tracing.
 */

#include <shape/shape.h>
#include <shape/ray.h>
#include <geometry/transform.h>
#include <Eigen/Dense>

/**
 * The aabb_t class represents the geometry of an axis-aligned bounding box
 */
class aabb_t : public shape_t
{
	/* parameters */
	private:

		/**
		 * The extrema corners of this bounding box
		 *
		 * [ 	min_x, max_x,
		 *	min_y, max_y,
		 *	min_z, max_z	]
		 */
		Eigen::Matrix<float,3,2> bounds;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Constructs default (invalid) bounding box
		 *
		 * The default box is invalid since all the min coordinates
		 * are greater than the max coordinates.
		 */
		aabb_t() 
		{ this->bounds << 1.0f,-1.0f, 1.0f,-1.0f, 1.0f,-1.0f; };
		
		/**
		 * Constructs bounding box from given other bounding box
		 *
		 * @param other   The other aabb to copy
		 */
		aabb_t(const aabb_t& other) : bounds(other.bounds)
		{};

		/**
		 * Constructs bounding box from given values
		 *
		 * [ 	min_x, max_x,
		 *	min_y, max_y,
		 *	min_z, max_z	]
		 *
		 * @param b    The extrema corners of the box to use
		 */
		aabb_t(const Eigen::Matrix<float,3,2>& b) : bounds(b)
		{};

		/**
		 * Constructs bounding box from arguments
		 *
		 * @param min_x   The min x-coord
		 * @param max_x   The max x-coord
		 * @param min_y   The min y-coord
		 * @param max_y   The max y-coord
		 * @param min_z   The min z-coord
		 * @param max_z   The max z-coord
		 */
		aabb_t(float min_x, float max_x, 
				float min_y, float max_y,
				float min_z, float max_z)
		{ 
			this->bounds << 
				min_x, max_x, 
				min_y, max_y, 
				min_z, max_z;
		};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Retrieve the min corner of this bounding box
		 *
		 * @param i  The coordinate to find the min for
		 *           (x = 0, y = 1, z = 2)
		 *
		 * @return   The minimum coordinate value
		 */
		inline float min(size_t i) const
		{ return this->bounds(i,0); };

		/**
		 * Retrieve the max corner of this bounding box
		 *
		 * @param i  The coordinate to find the max for
		 *           (x = 0, y = 1, z = 2)
		 *
		 * @return   The maximum coordinate value
		 */
		inline float max(size_t i) const
		{ return this->bounds(i,1); };

		/**
		 * Resets this bounding box to an invalid state
		 */
		inline void reset()
		{ this->set(1.0f,-1.0f,1.0f,-1.0f,1.0f,-1.0f); };

		/**
		 * Sets this bounding box to the given values
		 *
		 * [ 	min_x, max_x,
		 *	min_y, max_y,
		 *	min_z, max_z	]
		 *
		 * @param b    The extrema corners of the box to use
		 */
		inline void set(const Eigen::Matrix<float,3,2>& b)
		{ this->bounds = b; };
		
		/**
		 * Sets bounding box from arguments
		 *
		 * @param min_x   The min x-coord
		 * @param max_x   The max x-coord
		 * @param min_y   The min y-coord
		 * @param max_y   The max y-coord
		 * @param min_z   The min z-coord
		 * @param max_z   The max z-coord
		 */
		inline void set(float min_x, float max_x, 
				float min_y, float max_y,
				float min_z, float max_z)
		{
			this->bounds <<
				min_x, max_x,
				min_y, max_y,
				min_z, max_z;
		};

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Grows this bounding box to include the given point
		 *
		 * If this box is initially invalid, then it will be
		 * exactly the given point
		 *
		 * @param p  The point to include in this box
		 */
		void expand_to(const Eigen::Vector3f& p);

		/**
		 * Grows this bounding box to include the given bounding box
		 *
		 * If this box is initially invalid, then it will be
		 * exactly the input bounding box after this call.
		 *
		 * @param b   The bounding box to include in this object's
		 *            domain
		 */
		void expand_to(const aabb_t& b);

		/**
		 * Applies the given transform to this bounding box
		 *
		 * Note that this is not necessarily an invertable
		 * operation, since it may add 'slop' to the box.
		 *
		 * @param t   The transform to apply to this box
		 */
		void apply(const transform_t& t);

		/*----------------------*/
		/* overloaded functions */	
		/*----------------------*/

		/**
		 * Checks if this shape intersects a ray
		 *
		 * By checking this shape against the parameters
		 * of a ray, should determine if the 3D shape intersects 
		 * the given ray.
		 *
		 * @param t      Where to store the distance along the ray
		 *               where the intersection occurs
		 * @param n      The normal vector at the point of 
		 *               intersection on the surface 
		 * @param r      The ray to analyze
		 * @param t_min  The minimum valid t value to cause x-tion
		 * @param t_max  The maximum valid t value to cause x-tion
		 *
		 *
		 * @return   Returns true iff the shape intersects the ray
		 */
		bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const;
		
		/**
		 * Populates the axis-aligned bounding box for this shape
		 *
		 * Will generate the bounding box for this shape, and modify
		 * the given bounding box to represent the tightest bounds
		 * to this shape, restricted to being axis-aligned.
		 *
		 * @param bounds   Where to store the axis-aligned bounding
		 *                 box for this shape.
		 */
		inline void get_bounds(aabb_t& bounds) const
		{ bounds.set(this->bounds); };

		/*-----------*/
		/* operators */
		/*-----------*/

		/**
		 * Given another bounding box, will set this object to
		 * a copy of the argument.
		 *
		 * @param other   The bounding box to copy
		 *
		 * @return     Returns reference to modified box
		 */
		inline aabb_t& operator = (const aabb_t& other)
		{
			/* copy values */
			this->set(other.bounds);

			/* return the result */
			return (*this);
		};
};

#endif
