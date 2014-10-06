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

	/* functions */
	public:

		/* the pose list is an array that contains eigen 
		 * constructions, so it must be properly aligned */
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * Initializes identity transformation
		 */
		transform_t();

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
		 * A2C = A2B
		 *
		 * @param t    The transform to post-apply to this one
		 */
		void cat(const transform_t& t);

		/**
		 * Applies this transform to the given ray
		 *
		 * Given a ray, this transform will be applied in-place.
		 *
		 * @param ray   The ray to transform.
		 */
		void apply(ray_t& ray) const;

		/**
		 * Applies the inverse of this transform to the given ray
		 *
		 * Will apply the inverse of this transform in-place on the
		 * specified ray.
		 *
		 * @param ray   The ray to modify.
		 */
		void apply_inverse(ray_t& ray) const;

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

			/* return the value of this point */
			return (*this);
		};
};

#endif
