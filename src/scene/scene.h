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

#include <io/mesh/mesh_io.h>
#include <color/color.h>
#include <geometry/transform.h>
#include <scene/light.h>
#include <scene/camera.h>
#include <scene/element.h>
#include <tree/aabb_tree.h>
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
		 * This Axis-Aligned Bounding Box (AABB) tree is used
		 * to make ray-traces through the list of elements
		 * in this scene efficient.
		 *
		 * It holds the indices of elements in the above list,
		 * and can determine which elements are hit by a given
		 * ray by performing bounding-box analysis.
		 */
		aabb_tree_t tree;

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

		/**
		 * For debugging purposes.  If this value is
		 * true, will render the normal map, instead of
		 * phong shading.
		 */
		bool render_normal_shading;

		/**
		 * If true, will perform element-searching for
		 * ray tracing in a brute-force manner.
		 *
		 * If false, will use AABB-tree to search
		 */
		bool use_brute_force_search;

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
		 * @param rd         The recursion depth to use
		 * @param debug      If true, will render normal shading
		 *
		 * @return      Returns zero on success, non-zero on failure
		 */
		int init(const std::string& filename, 
				int rd = 2, bool debug = false);

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
		 * @param transform The transform to apply to this element
		 * @param shader    The shader properties of the element
		 */
		inline void add(shape_t* shape,
				const transform_t& transform,
				const phong_shader_t& shader)
		{ 
			this->elements.resize(this->elements.size()+1);
			this->elements.back().set_shape(shape);
			this->elements.back().set_transform(transform);
			this->elements.back().set_shader(shader);
		};

		/**
		 * Adds the given mesh to the scene
		 *
		 * Will import all mesh elements as triangles, using
		 * the given transform and shader.
		 *
		 * @param mesh       The mesh to import
		 * @param transform  The transform to apply to this element
		 * @param shader     The shader properties of this element
		 */
		void add(const mesh_io::mesh_t& mesh,
				const transform_t& transform,
				const phong_shader_t& shader);

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

	/* helper functions */
	private:

		/**
		 * Brute-force search of ray intersection over all elements
		 *
		 * Will search over all elements in this scene, looking
		 * for the best intersection with the given ray.  This
		 * function performs this search using brute-force 
		 * techniques.
		 *
		 * @param i_best        Where to store the index of best 
		 *                      element.  If out-of-bounds, then 
		 *                      no element found.
		 * @param t_best        Where to store the best intersection
		 *                      point along the ray
		 * @param normal_best   Where to store normal of best 
		 *                      surface.
		 * @param ray           The ray to test
		 * @param shortcircuit  If true, will return the first
		 *                      element that intersects, even if it
		 *                      isn't the closest.
		 * @param t_min         The minimum t-value to start search
		 * @param t_max         The max t-value to end search
		 */
		void brute_force_search(size_t& i_best, float& t_best,
				Eigen::Vector3f& normal_best,
				const ray_t& ray, bool shortcircuit,
				float t_min, float t_max) const;
};

#endif
