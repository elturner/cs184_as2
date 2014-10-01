#ifndef SCENE_H
#define SCENE_H

/**
 * @file   scene.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  The scene_t class represents all scene components
 *
 * @section DESCRIPTION
 *
 * This file containts the scene_t class, which is used to hold
 * all components of a scene.  These components include objects,
 * lighting, and materials.
 */

#include <color/color.h>
#include <scene/light.h>
#include <scene/camera.h>
#include <scene/element.h>
#include <Eigen/Dense>
#include <string>
#include <vector>

/**
 * The scene_t class houses all scene elements, including geometry, lights,
 * and materials.
 */
class scene_t
{
	/* properties */
	private:

		/**
		 * The geometry of the scene is represented by a set of
		 * elements.
		 *
		 * Each element has a shape and material properties.
		 */
		std::vector<element_t> elements;

		/**
		 * The lighting of the environment is represented by
		 * a set of light sources.
		 *
		 * These light sources can be either directional or
		 * point sources
		 */
		std::vector<light_t> lights;

		/**
		 * The camera represents the eye posiiton and the
		 * viewing plane.
		 *
		 * It can be oriented arbitrarily in the scene.
		 */
		camera_t camera;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Constructs default scene
		 */
		scene_t();
	
		/**
		 * Initializes a scene from the given input file
		 *
		 * @param filename   The file to parse
		 *
		 * @return      Returns zero on success, non-zero on failure
		 */
		int init(const std::string& filename);

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Traces the ray at the given viewer plane coordinates
		 *
		 * Will create the specified ray, perform ray tracing
		 * into the scene, and compute the final observed color
		 *
		 * @param u    The horizontal coordinate [0,1] of the ray
		 *             on the viewer screen
		 * @param v    The vertical coordinate [0,1] of the ray
		 *             on the viewer screen
		 *
		 * @return     Returns the final color observed by this ray
		 */
		color_t trace(float u, float v) const;

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Prints out info about this scene
		 */
		void print_info() const;
};

#endif
