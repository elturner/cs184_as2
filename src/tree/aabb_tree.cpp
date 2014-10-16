#include "aabb_tree.h"
#include <tree/aabb_node.h>
#include <shape/aabb.h>
#include <shape/ray.h>
#include <scene/element.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>

/**
 * @file    aabb_tree.cpp
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

using namespace std;
using namespace Eigen;

/*--------------------------*/
/* function implementations */
/*--------------------------*/

aabb_tree_t::~aabb_tree_t()
{
	this->clear();
}
		
void aabb_tree_t::init(const std::vector<element_t>& elements)
{
	vector<aabb_node_t> leaf_nodes;
	aabb_t bounds;
	size_t i, n;

	/* clear any existing info from this tree */
	this->clear();

	/* take each original element, and generate bounding boxes
	 * for the leaf nodes that will be generated for it */
	n = elements.size();
	for(i = 0; i < n; i++)
	{
		/* does current element have valid shape? */
		if(elements[i].get_shape() == NULL)
			continue;
		
		/* generate bounding box for i'th element */
		elements[i].get_shape()->get_bounds(bounds);
		bounds.apply(elements[i].get_transform());

		/* add a new leaf node to represent this element */
		leaf_nodes.push_back(aabb_node_t());
		leaf_nodes.back().init(i, bounds);
	}

	/* create a root node */
	this->root = new aabb_node_t();

	/* populate tree with leaves */
	this->root->init(leaf_nodes);
}

void aabb_tree_t::clear()
{
	/* check if root is allocated */
	if(this->root != NULL)
	{
		/* this will recursively free the tree */
		delete (this->root);
		this->root = NULL;
	}
}

void aabb_tree_t::trace(size_t& i_best, float& t_best, 
		           Eigen::Vector3f& n_best, const ray_t& ray,
			   bool shortcircuit, float t_min, float t_max,
			   const std::vector<element_t>& elements) const
{
	/* initialize the output variables to indicate no intersections */
	i_best = elements.size();
	t_best = t_max;

	/* check if root exists */
	if(this->root == NULL)
		return; /* no intersections possible on empty tree */
	
	/* recursively search through the tree */
	this->root->trace(i_best, t_best, n_best, ray, 
			shortcircuit, t_min, t_max, elements);
}

void aabb_tree_t::print(std::ostream& os) const
{
	/* check if root exists */
	if(this->root == NULL)
		os << "[NULL TREE]" << endl;
	else
		this->root->print(os, "");
}

