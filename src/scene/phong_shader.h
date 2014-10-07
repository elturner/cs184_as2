#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

/**
 * @file   phong_shader.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  This file implements phong shading operations
 *
 * @section DESCRIPTION
 *
 * The phong_shader_t class is used to represent
 * the phong shader for a scene, and can apply this
 * shading appoach to a given scene element.
 */

#include <color/color.h>
#include <scene/light.h>
#include <Eigen/Dense>

/**
 * This class is used to represent a material, and
 * can perform phong shading for that material in a scene
 */
class phong_shader_t
{
	/* parameters */
	public:

		/**
		 * The ambient color coefficients 
		 */
		color_t ka;

		/**
		 * The diffuse color coefficients
		 */
		color_t kd;

		/**
		 * The specular color coefficients
		 */
		color_t ks;

		/**
		 * The power coefficient for specular color
		 */
		float p;

		/**
		 * The reflective color coefficients
		 */
		color_t kr;

	/* functions */
	public:

		/**
		 * Default constructor
		 */
		phong_shader_t();

		/**
		 * Construct from another shader
		 *
		 * @param other   The other shader
		 */
		phong_shader_t(const phong_shader_t& other)
			: ka(other.ka), kd(other.kd),
			  ks(other.ks), p(other.p), kr(other.kr)
		{};

		/**
		 * Computes the phong illumination model for the
		 * specified point on a surface.
		 *
		 * NOTE:  all directions are assumed to be normalized
		 * vectors.
		 *
		 * @param N        The normal of the surface, in 
		 *                 world coordinates.
		 * @param V        The direction from the surface 
		 *                 to the viewer.
		 * @param L        The direction from light source to
		 *                 surface
		 * @param I        The color of the light source
		 * 
		 * @return   Returns the observed surface color
		 */
		color_t compute_phong(
				const Eigen::Vector3f& N,
				const Eigen::Vector3f& V,
				const Eigen::Vector3f& L,
				const color_t& I) const;

		/**
		 * Copies value of given shader
		 *
		 * Will copy the parameters from the other shader
		 *
		 * @param other  The other shader to copy
		 *
		 * @return    Returns the modified shader
		 */
		inline phong_shader_t& operator = (
				const phong_shader_t& other)
		{
			/* copy parameters */
			this->ka = other.ka;
			this->kd = other.kd;
			this->ks = other.ks;
			this->p  = other.p;
			this->kr = other.kr;

			/* return the modified object */
			return (*this);
		};
};

#endif
