#ifndef TRANSFORM_H
#define TRANSFORM_H

/**
 * @file   transform.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  THis class represents a transform in homogenous coordinates
 *
 * @section DESCRIPTION
 *
 * This file defines the transform_t class, which is used
 * to represent a linear transform between coordinate
 * systems.
 */

#include <shape/ray.h>
#include <Eigen/Dense>

/**
 * the transform_t class represents rigid-body transformations
 */
class transform_t
{
	/* parameters */
	public:

		/* The following specify the orientation of the scanner
		 * at this pose as a rotation from system coordinates
		 * to world coordinates */
		Eigen::Matrix4f H; /* homogenous matrix */

		/**
		 * The following is a cached version of the inverse
		 * of H
		 */
		Eigen::Matrix4f H_inv;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/* contains eigen constructions, so it must be 
		 * properly aligned */
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * Initializes identity transformation
		 */
		transform_t();
		
		/**
		 * Initializes transform from given other transform
		 *
		 * @param other   The other transform to copy
		 */
		transform_t(const transform_t& other)
			: H(other.H), H_inv(other.H_inv)
		{};

		/**
		 * Frees all memory and resources.
		 */
		~transform_t();

		/*-----------*/
		/* modifiers */
		/*-----------*/

		/**
		 * Resets rotation to identity
		 */
		void reset();

		/**
		 * Sets this transform to the given translation
		 *
		 * Any existing data in this transform will be deleted.
		 *
		 * @param tx   The x-component of the translation to set
		 * @param ty   The y-component of the translation to set
		 * @param tz   The z-component of the translation to set
		 */
		void set_translation(float tx, float ty, float tz);

		/**
		 * Sets this transform to the given scale
		 *
		 * Any existing data in this transform will be deleted.
		 *
		 * @param sx   The x-component of the scale to set
		 * @param sy   The y-component of the scale to set
		 * @param sz   The z-component of the scale to set
		 */
		void set_scale(float sx, float sy, float sz);

		/**
		 * Sets this transform to the given rotation
		 *
		 * Any existing data in this transform will be deleted.
		 * The input values should be in units of degrees, and
		 * represented as the components of an exponential map.
		 *
		 * @param rx   The x-component of the exponential map
		 * @param ry   The y-component of the exponential map
		 * @param rz   The z-component of the exponential map
		 */
		void set_rotation(float rx, float ry, float rz);
		
		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Concatenates two transforms
		 *
		 * Will concatenate the specified transform to this one,
		 * and store the result in-place.  Applying the resulting
		 * transform is equivalent to applying the original, and
		 * then the argument transform.
		 *
		 * Consider the following example:
		 *
		 * A2B.cat(B2C)
		 * ("A2B" is now the transform from A to C)
		 *
		 * @param t    The transform to post-apply to this one
		 */
		void cat(const transform_t& t);

		/**
		 * Applies this transform to the given point
		 *
		 * Given a point in 3D space, will apply the transform
		 * to the point and return the modified version of the 
		 * point.
		 *
		 * @param p    The point to modify
		 *
		 * @return     The modified version of the point.
		 */
		Eigen::Vector3f apply(const Eigen::Vector3f& p) const;

		/**
		 * Applies the inverse of this transform to the given point
		 *
		 * Given a point in 3D space, will apply the inverse 
		 * of this transform to the point and return the modified 
		 * version of the point.
		 *
		 * @param p    The point to modify
		 *
		 * @return     The modified version of the point.
		 */
		Eigen::Vector3f apply_inverse(
					const Eigen::Vector3f& p) const;

		/**
		 * Applies this transform to the given normal vector
		 *
		 * Given a normal vector, will apply the transform
		 * and return the modified version of the normal
		 * vector.  The output normal vector will be renormalized.
		 *
		 * It is assumed the input vector is already normalized.
		 *
		 * @param n   The normal vector to modify
		 *
		 * @return    The modified normal vector
		 */
		Eigen::Vector3f apply_normal(
					const Eigen::Vector3f& n) const;

		/**
		 * Applies the inverse of this transformation to the
		 * given normal vector.
		 *
		 * Given a normal vector, will apply the inverse of
		 * this transform and return the modified normal vector.
		 * The output normal vector will be renormalized.
		 *
		 * It is assumed the input vector is already normalized
		 *
		 * @param n   The normal vector to modify
		 *
		 * @return    The modified normal vector
		 */
		Eigen::Vector3f apply_normal_inverse(
					const Eigen::Vector3f& n) const;

		/**
		 * Applies this transform to the given ray
		 *
		 * Given a ray, this transform will be applied, and the 
		 * a modified version of the ray will be returned.
		 *
		 * @param ray   The ray to transform.
		 * @param s     The distance scaling from original to new
		 *
		 * @return      The transformed ray
		 */
		ray_t apply(const ray_t& ray) const;
		ray_t apply(const ray_t& ray, float& s) const;

		/**
		 * Applies the inverse of this transform to the given ray
		 *
		 * Will apply the inverse of this transform and returns the
		 * modified version of the ray.
		 *
		 * @param ray   The ray to transform.
		 * @param s     The distance scaling from original to new
		 *
		 * @return      The transformed ray
		 */
		ray_t apply_inverse(const ray_t& ray) const;
		ray_t apply_inverse(const ray_t& ray, float& s) const;

		/*-----------*/
		/* operators */
		/*-----------*/

		/**
		 * Sets value of this transform to argument
		 *
		 * Sets the value of this object to rhs
		 *
		 * @param  rhs   The transform to copy
		 *
		 * @return  Returns this tranform after modifications
		 */
		inline transform_t operator = (const transform_t& rhs)
		{
			/* copy params */
			this->H = rhs.H;
			this->H_inv = rhs.H_inv;

			/* return the value of this point */
			return (*this);
		};
};

#endif
