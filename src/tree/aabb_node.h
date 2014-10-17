#ifndef AABB_NODE_H
#define AABB_NODE_H

/**
 * @file     aabb_node.h
 * @author   Eric Turner <elturner@eecs.berkeley.edu>
 * @brief    Defines the node class used for aabb trees
 *
 * @section DESCRIPTION
 *
 * This file defines the aabb_node_t class, which is used to
 * house a tree of Axis-Aligned Bounding Boxes (AABBs), which represent
 * a set of elements in space.
 */

#include <shape/aabb.h>
#include <shape/ray.h>
#include <scene/element.h>
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

/**
 * The aabb_node_t class represents a node in the aabb tree
 *
 * Each node can have either two or zero children, a bounding box,
 * and an element index.  If the node has zero children, then it
 * is a leaf and the element index is valid.  If the node has two
 * children, then it is a non-leaf and the element index should be
 * ignored.
 */
class aabb_node_t
{
	/* constants */
	public:

		/**
		 * The number of children per tree node
		 *
		 * This is a binary tree, so each node has up to two
		 * children.  Note that no node can have one child, since
		 * then it should be simplified down to a leaf itself.
		 */
		static const size_t NUM_CHILDREN_PER_NODE = 2;

		/**
		 * The number of dimensions of the space this tree is
		 * embedded in.  Since all shapes and elements are 3D,
		 * this value is three.
		 */
		static const size_t NUM_DIMS = 3;

	/* parameters */
	private:

		/**
		 * The index of the element represented by this node
		 *
		 * This value is only valid at leaf nodes.  Otherwise,
		 * it should be ignored.
		 */
		int index;

		/**
		 * The bounding box of this node.
		 *
		 * This bounding box contains the bounding boxes of
		 * all child elements of this node
		 */
		aabb_t bounds;

		/**
		 * The child nodes of this node
		 *
		 * Note that either both children are null (in which
		 * case this is a leaf), or both children are non-null
		 * (in which case this is a non-leaf).
		 */
		aabb_node_t* children[NUM_CHILDREN_PER_NODE];

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Constructs default node
		 */
		aabb_node_t();

		/**
		 * Makes shallow copy of the given node
		 *
		 * @param other   The other node to copy
		 */
		aabb_node_t(const aabb_node_t& other)
			: index(other.index), bounds(other.bounds)
		{
			size_t i;
			
			/* make shallow copy of the children */
			for(i = 0; i < NUM_CHILDREN_PER_NODE; i++)
				this->children[i] = other.children[i];
		};

		/**
		 * Constructs a leaf node with the given index and bounds
		 *
		 * @param i   The index to use
		 * @param b   The bounds to use
		 */
		aabb_node_t(int i, const aabb_t& b)
		{
			size_t j;

			this->index = i;
			this->bounds = b;
			
			for(j = 0; j < NUM_CHILDREN_PER_NODE; j++)
				this->children[j] = NULL;
		};

		/**
		 * Frees all memory and resources under this node
		 */
		~aabb_node_t();

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Clears all memory and resources from this node
		 *
		 * This will recursively delete any children of this
		 * node.
		 */
		void clear();

		/**
		 * Initializes this structure as a leaf with the given info
		 *
		 * This will delete any other resources under this node
		 * that existed before this call.
		 *
		 * @param i   The index value to use
		 * @param b   The bounds to use
		 */
		void init(int i, const aabb_t& b);

		/**
		 * Initializes this structure recursively by populating
		 * its children with the given leaf nodes.
		 *
		 * The given list of leaf nodes will be inserted into the
		 * tree.  Note that the final leafs of this tree will be
		 * copies of the elements of this vector.  The actual
		 * objects in the list will not be explicitely referenced.
		 *
		 * Any existing children or values in this node will be
		 * destroyed in the course of this call.
		 *
		 * @param leaf_nodes   The list of leaves to insert
		 */
		void init(const std::vector<
				std::vector<aabb_node_t>::const_iterator >& 
					leaf_nodes);

		/**
		 * Returns true iff this node is a leaf
		 *
		 * @return   Returns true if this node is a leaf
		 */
		inline bool isleaf() const
		{ 
			return (this->children[0] == NULL 
					&& this->children[1] == NULL);
		};

		/**
		 * Retrieves the index value stored in this node
		 *
		 * @return   Returns the index value at this node
		 */
		inline int get_index() const
		{ return this->index; };

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * Retrieves the center position of this box along
		 * the specified dimension.
		 *
		 * @param d   The dimension to check (0 = x, 1 = y, 2 = z)
		 *
		 * @return    Returns the midpoint value of this box
		 */
		inline float midpoint(size_t d) const
		{ return this->bounds.center(d); };

		/**
		 * Retrieves the center position of this box.
		 *
		 * @return  Returns the midpoint of this box along the
		 *          current dim.
		 */
		inline Eigen::Vector3f midpoint() const
		{
			Eigen::Vector3f m;
			size_t i;

			/* compute midpoint for each dimension */
			for(i = 0; i < NUM_DIMS; i++)
				m(i) = this->midpoint(i); 
			
			/* return the computed point */
			return m;
		};

		/**
		 * Given a ray, will find the location of intersection
		 * of the elements described within this tree.
		 *
		 * This function will find the closest point of intersection
		 * with the elements in this tree, by performing recursive
		 * search through the nodes of the tree.
		 *
		 * This function is recursive.
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
		/* operators */
		/*-----------*/

		inline aabb_node_t& operator = (const aabb_node_t& other)
		{
			size_t i;
			
			/* copy values */
			this->index = other.index;
			this->bounds = other.bounds;

			/* make shallow copy of the children */
			for(i = 0; i < NUM_CHILDREN_PER_NODE; i++)
				this->children[i] = other.children[i];

			/* return the result */
			return (*this);
		};

		/*-----------*/
		/* debugging */
		/*-----------*/

		/**
		 * Recursively prints this node and subtree to given stream
		 *
		 * @param os      The output stream to print to
		 * @param indent  The indent string for this subtree
		 */
		void print(std::ostream& os, 
				const std::string& indent) const;
};

#endif
