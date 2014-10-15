#ifndef SPHERE_H
#define SPHERE_H

/**
 * @file   sphere.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  The sphere_t class represents a sphere in 3D space
 *
 * @section DESCRIPTION
 *
 * This file contains the sphere_t class, which is used to represent
 * the geometry of a sphere in 3D space.
 */

#include <shape/shape.h>
#include <shape/aabb.h>
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>

/**
 * The sphere_t class is used to represent the geometry of a sphere
 * in 3D space
 */
class sphere_t : public shape_t
{
	/* parameters */
	private:

		/**
		 * The position of the center of the sphere
		 * in 3D space
		 */
		Eigen::Vector3f center;

		/**
		 * The radius of this sphere
		 */
		float radius;

		/**
		 * Cached value of the square of the radius
		 */
		float radius_squared;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Generates default sphere
		 */
		sphere_t() : center(0.0f,0.0f,0.0f), 
		             radius(1.0f), radius_squared(1.0f)
		{};

		/**
		 * Constructs sphere from other sphere
		 *
		 * @param other   The other sphere to copy
		 */
		sphere_t(const sphere_t& other)
			: center(other.center), 
			  radius(other.radius), 
			  radius_squared(other.radius_squared)
		{};

		/**
		 * Constructs sphere from given values
		 *
		 * @param x   The center x-position
		 * @param y   The center y-position
		 * @param z   The center z-position
		 * @param r   The radius
		 */
		sphere_t(float x, float y, float z, float r)
			: center(x,y,z), radius(r), radius_squared(r*r)
		{};

		/**
		 * Frees all memory and resources
		 */
		~sphere_t()
		{ /* don't need to do anything */ };

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Accesses the center position of the sphere
		 *
		 * @return   Reference to the center position
		 */
		inline const Eigen::Vector3f& get_center() const
		{ return this->center; };
		
		/**
		 * Accesses the center position of the sphere
		 *
		 * @return   Reference to the center position
		 */
		inline Eigen::Vector3f& get_center()
		{ return this->center; };

		/**
		 * Accesses the radius of the sphere
		 *
		 * @return   Reference to the radius
		 */
		inline const float& get_radius() const
		{ return this->radius; };

		/**
		 * Sets the radius of this sphere
		 *
		 * @param r  The value to use as the radius
		 */
		inline void set_radius(float r)
		{ 
			this->radius = r;
			this->radius_squared = r*r;
		};

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
		 * @param t_min  The minimum valid t value to cause x-tion
		 * @param t_max  The maximum valid t value to cause x-tion
		 *
		 * @return   Returns true iff the shape intersects the ray
		 */
		inline bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const
		{
			Eigen::Vector3f d, c;
			float B, C, root, t1, t2;

			/* characterize ray via tangent direction */
			d = r.dir();

			/* get displacement of center along line */
			c = this->center - r.get_origin();
			
			/* the intersection of the ray with the sphere
			 * can be represented as the following quadratic
			 * equation:
			 *
			 *	0 = ||d||^2 * t^2 + 2*d.dot(c)*t 
			 *			+ (||c||^2 - radius^2)
			 *
			 * If there are no real roots, then no intersection
			 * occurs.  Otherwise, we can use the value of t
			 * at the roots to find the intersection position.
			 */

			/* compute terms of quadratic.  Note d is unit */
			B = -2*d.dot(c);
			C = c.squaredNorm() - this->radius_squared;
			
			/* compute b^2 - 4*a*c for the quadratic */
			root = B*B - 4*C;
			if(root < 0)
				return false; /* no real roots */
			if(root == 0)
			{
				/* one real root */
				t = -B/2;
			}
			else
			{
				/* two real roots, choose the one that
				 * is positive, and if both are positive,
				 * choose smaller */
				root = sqrt(root);
				t1 = (-B - root) / 2;
				t2 = (-B + root) / 2;

				if(t2 < t_min)
					return false; /* both roots bad */
				if(t1 > t_max)
					return false; /* both roots bad */
				if(t1 < t_min && t2 > t_max)
					return false; /* out of bounds */

				/* determine best of both roots */
				if(t1 < t_min)
					t = t2; /* use higher one */
				else
					t = t1; /* use lower one */
			}

			/* compute the normal of the sphere at this
			 * point on its surface */
			n = (r.point_at(t) - this->center) / this->radius;
			n.normalize();

			/* we intersected */
			return true;
		};
		
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
		{
			/* fit cube around sphere */
			bounds.set(
				this->center(0) - this->radius,
				this->center(0) + this->radius,
				this->center(1) - this->radius,
				this->center(1) + this->radius,
				this->center(2) - this->radius,
				this->center(2) + this->radius);
		};
};

#endif
