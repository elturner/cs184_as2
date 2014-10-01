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

#include <scene/light.h>
#include <scene/phong_shader.h>
#include <scene/sphere.h>
#include <scene/camera.h>
#include <Eigen/Dense>
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
		 * the geometry of the scene is represented by a set of
		 * spheres 
		 */
		std::vector<sphere_t> spheres;

		/**
		 * The lighting of the environment is represented by
		 * a set of light sources.
		 *
		 * These light sources can be either directional or
		 * point sources
		 */
		std::vector<light_t> lights;

		/**
		 * All geometry has the same material, which is represented
		 * by this phong shader object.
		 */
		phong_shader_t shader;

		/**
		 * The camera is positioned in the scene along the z-axis,
		 * looking downward.  So the camera direction is always
		 * constant, but the position can vary
		 */
		camera_t camera;

		/**
		 * If true, will rotate the lights
		 */
		bool rotate_lights;

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
		 * Initializes a scene from command-line arguments
		 *
		 * @param argc  Number of command-line arguments
		 * @param argv  The command-line arguments
		 *
		 * @return      Returns zero on success, non-zero on failure
		 */
		int init(int argc, char** argv);

		/**
		 * Set the display size
		 *
		 * This will modify the scene's camera, in order to
		 * keep everything in view
		 *
		 * @param w     Width of the camera display
		 * @param h     Height of the camera display
		 */
		void set_display_size(int w, int h);

		/*----------*/
		/* graphics */
		/*----------*/

		/**
		 * Will toggle light rotation in the scene
		 */
		inline void toggle_rotation()
		{ this->rotate_lights = !(this->rotate_lights); };

		/**
		 * Renders this scene
		 */
		void render();

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Prints out info about this scene
		 */
		void print_info() const;
};

#endif
