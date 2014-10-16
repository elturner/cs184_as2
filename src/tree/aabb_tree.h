#ifndef AABB_TREE_H
#define AABB_TREE_H

/**
 * @file    aabb_tree.h
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   This file defines the aabb_tree_t class, used to 
 *          efficiently index elements for a scene
 *
 * @section DESCRIPTION
 *
 * This file contains the aabb_tree_t class.  This tree contains
 * index values and bounding boxes for elements in the originating
 * scene.  These are used to do efficient ray-tracing into the scene,
 * so that the ray-intersection function only needs to be called on
 * the bare minimum elements.
 */

#include <tree/aabb_node.h>
#include <shape/aabb.h>
#include <shape/ray.h>
#include <scene/element.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>

/**
 * The aabb_tree_t class represents all scene elements in a tree,
 * determines efficiently the intersection tests for rays against
 * these elements.
 */
class aabb_tree_t
{
	/* parameters */
	private:

		/**
		 * The root node of this tree
		 */
		aabb_node_t* root;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Initializes empty tree
		 */
		aabb_tree_t() : root(NULL)
		{};

		/**
		 * Frees all memory and resources
		 */
		~aabb_tree_t();

		/*----------------*/
		/* initialization */
		/*----------------*/

		/**
		 * Initializes this tree with the given list of elements
		 *
		 * Any existing values in the tree will be destroyed.
		 *
		 * @param elements    The elements to insert in this tree
		 */
		void init(const std::vector<element_t>& elements);

		/**
		 * Frees all memory and resources from this tree
		 */
		void clear();

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Trace a ray through the elements in this tree, find
		 * best intersection.
		 *
		 * Given a ray, will find the best intersection of all
		 * the elements in this scene.
		 *
		 * @param i_best   The index of the best intersecting value.
		 *                 If out-of-bounds, then no element found
		 * @param t_best   The ray parameter at best intersection
		 * @param n_best   The normal of the surface at intersection
		 *                 point.
		 * @param ray      The ray to analyze
		 * @param shortcircuit   If set to true, then will return
		 *                       the first intersection found, not
		 *                       necessarily the closest one.
		 * @param t_min    The minimum valid t-value
		 * @param t_max    The maximum valid t-value
		 * @param elements The list of elements referenced by this
		 *                 tree.  These are necessary to perform
		 *                 the final raytracing operations.
		 */
		void trace(size_t& i_best, float& t_best, 
		           Eigen::Vector3f& n_best, const ray_t& ray,
			   bool shortcircuit, float t_min, float t_max,
			   const std::vector<element_t>& elements) const;

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Prints out tree to given output stream
		 *
		 * Will print out the contents of this tree to
		 * the given file stream.
		 *
		 * @param os  The output file stream to write to
		 */
		void print(std::ostream& os) const;
};

#endif
