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

		/**
		 * The recursion depth for reflective surfaces
		 */
		int recursion_depth;

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
		 * Frees all memory and resources in the scene
		 */
		~scene_t();

		/**
		 * Initializes a scene from the given input file
		 *
		 * @param filename   The file to parse
		 *
		 * @return      Returns zero on success, non-zero on failure
		 */
		int init(const std::string& filename);

		/*-----------*/
		/* modifiers */
		/*-----------*/

		/**
		 * Adds a light source to the scene
		 *
		 * @param light   The light source to add
		 */
		inline void add(const light_t& light)
		{ this->lights.push_back(light); };

		/**
		 * Adds an element to the scene
		 *
		 * @param shape     The shape of the element to add
		 * @param shader    The shader properties of the element
		 */
		inline void add(shape_t* shape, 
				const phong_shader_t& shader)
		{ 
			this->elements.resize(this->elements.size()+1);
			this->elements.back().set(shape);
			this->elements.back().set_shader(shader);
		};

		/**
		 * Retrieves the camera object reference, for modification
		 *
		 * @return   Returns modifiable reference to the camera
		 */
		inline camera_t& get_camera()
		{ return this->camera; };

		/**
		 * Retrieves the camera object const reference
		 *
		 * @return   Returns a constant reference to the camera
		 */
		inline const camera_t& get_camera() const
		{ return this->camera; };

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

		/**
		 * Traces the specified ray through the scene
		 *
		 * Given a ray, will determine the color seen by
		 * that ray in the scene.  We can also specify
		 * the number of times to recurse, in case that ray
		 * bounces off of reflective elements.
		 *
		 * @param ray    The ray to trace
		 * @param r      The number of times to recurse
		 *
		 * @return       Returns the final color observed by the ray
		 */
		color_t trace(const ray_t& ray, int r) const;

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Prints out info about this scene
		 */
		void print_info() const;
};

#endif
