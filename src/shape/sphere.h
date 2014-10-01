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
#include <Eigen/Dense>

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

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Generates default sphere
		 */
		sphere_t() : center(0.0f,0.0f,0.0f), radius(1.0f)
		{};

		/**
		 * Constructs sphere from other sphere
		 *
		 * @param other   The other sphere to copy
		 */
		sphere_t(const sphere_t& other)
			: center(other.center), radius(other.radius)
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
			: center(x,y,z), radius(r)
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
		 * Accesses the radius of the sphere
		 *
		 * @return   Reference to the radius
		 */
		inline float& get_radius()
		{ return this->radius; };

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
		inline bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r) const
		{
			Eigen::Vector3f d, c;
			float B, C, root, r2;

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
			r2 = this->radius * this->radius;
			C = c.squaredNorm() - r2;
			
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
				t = (-B - root) / 2;
				if(t < 0)
					t = (-B + root) / 2;
			}

			/* compute the normal of the sphere at this
			 * point on its surface */
			n = (r.point_at(t) - this->center);
			n.normalize();

			/* we intersected */
			return true;
		};
};

#endif
