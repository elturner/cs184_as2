#ifndef SHAPE_H
#define SHAPE_H

/**
 * @file   shape.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  A shape interface to use for raytracing
 *
 * @section DESCRIPTION
 *
 * The shape_t class is a virtual interface that allows
 * for different shapes to be used to intersect rays.
 *
 * By defining how a shape intersects a ray, we get sufficient
 * information to define the geometry of that shape.
 *
 * This class requires the Eigen framework
 */

#include <shape/ray.h>
#include <Eigen/Dense>

/* the aabb_t class is an extension of shape, but
 * is also referenced by the shape_t class, so it
 * needs to be defined here as well */
class aabb_t;

/**
 * The shape_t virtual interface
 */
class shape_t
{
	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Frees all memory and resources
		 */
		virtual ~shape_t() {};

		/*----------*/
		/* geometry */
		/*----------*/

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
		virtual bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const =0;

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
		virtual void get_bounds(aabb_t& bounds) const =0;
};

#endif
