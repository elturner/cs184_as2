#ifndef RAY_H
#define RAY_H

/**
 * @file    ray.h
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   The ray_t represents a ray in 3D space
 *
 * @section DESCRIPTION
 *
 * This file contains the ray_t class, which is used to represent
 * a ray in 3D space.  This ray starts at some position, and goes
 * in some direction.  The ray can be specified to have finite duration
 * or infinite duration.
 */

#include <Eigen/Dense>
#include <iostream>

/**
 * The ray_t class represents a ray in 3D space
 */
class ray_t
{
	/* parameters */
	private:

		/**
		 * The start position for this ray 
		 */
		Eigen::Vector3f origin;

		/**
		 * A point that the ray passes through
		 *
		 * Note that (target - origin) is an unnormalized
		 * representation for the direction vector of this
		 * ray.
		 */
		Eigen::Vector3f target;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Constructs a default ray
		 *
		 * This call will create a ray that points along the x-axis
		 */
		ray_t() : origin(0.0f,0.0f,0.0f), target(1.0f,0.0f,0.0f)
		{};

		/**
		 * Constructs a ray given origin and target
		 *
		 * Given the position of the ray's origin and the position
		 * of a target in 3D space that the ray passes through,
		 * will construct this ray
		 *
		 * @param oo   The origin position to use
		 * @param tt   The target position to use
		 */
		ray_t(const Eigen::Vector3f& oo, const Eigen::Vector3f& tt)
			: origin(oo), target(tt)
		{};

		/**
		 * Constructs a ray from the other given ray
		 *
		 * @param other   The ray to copy
		 */
		ray_t(const ray_t& other) 
			: origin(other.origin), target(other.target)
		{};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Sets the values of this ray
		 *
		 * @param oo   The origin position to use
		 * @param tt   The target position to use
		 */
		inline void set(const Eigen::Vector3f& oo,
		                const Eigen::Vector3f& tt)
		{
			this->origin = oo;
			this->target = tt;
		};

		/**
		 * Retrieves the origin of this ray
		 *
		 * @return   Returns a reference to the origin position
		 */
		inline const Eigen::Vector3f& get_origin() const
		{ return this->origin; };

		/**
		 * Retrieves the target of this ray
		 *
		 * @return   Returns a reference to the target position
		 */
		inline const Eigen::Vector3f& get_target() const
		{ return this->target; };

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Generates specified point along the ray
		 *
		 * Given the parameterized value along the ray specified
		 * by this object, will generate the point along this
		 * ray at that value.
		 *
		 * @param t   The value at which to sample ray
		 *
		 * @return    Returns position of ray at this value
		 */
		inline Eigen::Vector3f point_at(float t) const
		{
			Eigen::Vector3f p;

			/* compute position */
			p = this->origin + t*(this->target - this->origin);

			/* return computation */
			return p;
		};

		/**
		 * Retrieves the direction of this ray
		 *
		 * Will compute the normalized direction vector
		 * of this ray, from the origin to the target.
		 *
		 * @return    Returns the direction vector
		 */
		inline Eigen::Vector3f dir() const
		{
			Eigen::Vector3f d;

			/* compute direction */
			d = (this->target - this->origin);
			d.normalize();

			/* return the result */
			return d;
		};

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Prints out this ray to the given stream
		 *
		 * Will export debugging information about this
		 * ray to the given stream.
		 *
		 * @param os   The output stream to write to
		 */
		inline void print(std::ostream& os) const
		{
			os << "<ray_t: origin=["
			   << this->origin.transpose() << "], target=["
			   << this->target.transpose() << "] >"
			   << std::endl;
		};
};

#endif
