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
		 * The normalized direction vector of this ray
		 */
		Eigen::Vector3f direction;

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
		ray_t() : origin(0.0f,0.0f,0.0f), direction(1.0f,0.0f,0.0f)
		{};

		/**
		 * Constructs a ray given origin and target
		 *
		 * Given the position of the ray's origin and the
		 * direction in 3D space of the ray,
		 * will construct this ray
		 *
		 * @param oo   The origin position to use
		 * @param dd   The direction to use
		 */
		ray_t(const Eigen::Vector3f& oo, const Eigen::Vector3f& dd)
			: origin(oo), direction(dd)
		{
			/* make sure direction is normalized */
			this->direction.normalize();
		};

		/**
		 * Constructs a ray from the other given ray
		 *
		 * @param other   The ray to copy
		 */
		ray_t(const ray_t& other) 
			: origin(other.origin), direction(other.direction)
		{};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Sets the values of this ray
		 *
		 * @param oo   The origin position to use
		 * @param dd   The direction to use
		 */
		inline void set(const Eigen::Vector3f& oo,
		                const Eigen::Vector3f& dd)
		{
			/* set values */
			this->origin    = oo;
			this->direction = dd;

			/* make sure direction is normalized */
			this->direction.normalize();
		};

		/**
		 * Retrieves the origin of this ray
		 *
		 * @return   Returns a reference to the origin position
		 */
		inline const Eigen::Vector3f& get_origin() const
		{ return this->origin; };

		/**
		 * Retrieves the direction of this ray
		 *
		 * Will compute the normalized direction vector
		 * of this ray, from the origin to the target.
		 *
		 * @return    Returns the direction vector
		 */
		inline const Eigen::Vector3f& dir() const
		{ return this->direction; };

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
			p = this->origin + t*(this->direction);
			
			/* return computation */
			return p;
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
			   << this->origin.transpose() << "], dir=["
			   << this->direction.transpose() << "] >"
			   << std::endl;
		};
};

#endif
