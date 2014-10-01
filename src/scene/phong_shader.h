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
#include <vector>

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

	/* functions */
	public:

		/**
		 * Default constructor
		 */
		phong_shader_t();

		/**
		 * Computes the phong illumination model for the
		 * specified point on a surface.
		 *
		 * NOTE:  all directions are assumed to be normalized
		 * vectors.
		 *
		 * @param P        The point on surface to analyze
		 * @param N        The normal of the surface, in 
		 *                 world coordinates.
		 * @param V        The direction from the surface 
		 *                 to the viewer.
		 * @param lights   The list of light sources in the scene.
		 * 
		 * @return   Returns the observed surface color
		 */
		color_t compute_phong(
				const Eigen::Vector3f& P,
				const Eigen::Vector3f& N,
				const Eigen::Vector3f& V,
				const std::vector<light_t>& lights) const;

};

#endif
