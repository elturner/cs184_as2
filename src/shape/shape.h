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
		 * @param t  Where to store the distance along the ray
		 *           where the intersection occurs
		 * @param n  The normal vector at the point of intersection
		 *           on the surface 
		 * @param r  The ray to analyze
		 *
		 * @return   Returns true iff the shape intersects the ray
		 */
		virtual bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r) const =0;
};

#endif
