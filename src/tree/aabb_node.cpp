#include "aabb_node.h"
#include <shape/aabb.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <Eigen/Dense>

/**
 * @file     aabb_node.cpp
 * @author   Eric Turner <elturner@eecs.berkeley.edu>
 * @brief    Implements the node class used for aabb trees
 *
 * @section DESCRIPTION
 *
 * This file implements the aabb_node_t class, which is used to
 * house a tree of Axis-Aligned Bounding Boxes (AABBs), which represent
 * a set of elements in space.
 */

using namespace std;
using namespace Eigen;

/*--------------------------*/
/* function implementations */
/*--------------------------*/

aabb_node_t::aabb_node_t()
{
	size_t i;

	/* initialize default values */
	this->index = -1;
	this->bounds.reset();
	for(i = 0; i < NUM_CHILDREN_PER_NODE; i++)
		this->children[i] = NULL;
}

aabb_node_t::~aabb_node_t()
{
	/* free all memory and resources */
	this->clear();
}
		
void aabb_node_t::clear()
{
	size_t i;

	/* free children */
	for(i = 0; i < NUM_CHILDREN_PER_NODE; i++)
	{
		/* check if child exists */
		if(this->children[i] == NULL)
			continue;

		/* free child */
		this->children[i]->clear();
		delete (this->children[i]);
		this->children[i] = NULL;
	}
	
	/* set default values */
	this->bounds.reset();
	this->index = -1;
}
	
void aabb_node_t::init(int i, const aabb_t& b)
{
	/* first, clear any existing values */
	this->clear();
	
	/* set input parameters */
	this->index  = i;
	this->bounds = b;
}
		
void aabb_node_t::init(const std::vector<
		std::vector<aabb_node_t>::const_iterator>& leaf_nodes)
{
	Vector3f mp;
	aabb_t midpoints;
	vector<vector<aabb_node_t>::const_iterator> left, right;
	size_t i, num, di, dim_to_split;
	float len, dim_size, pivot, child_mid;

	/* first, clear any existing info */
	this->clear();

	/* get the number of leaves being inserted */
	num = leaf_nodes.size();
	if(num == 0)
		return; /* no leaves to add */
	if(num == 1)
	{
		/* just set this node to the leaf */
		this->init(leaf_nodes[0]->index, leaf_nodes[0]->bounds);
		this->bounds = leaf_nodes[0]->bounds;
		return;
	}
	if(num == 2)
	{
		/* edge case, just set the children of this node
		 * to the two remaining leaves */
		for(i = 0; i < num; i++)
		{
			this->children[i] = new aabb_node_t();
			this->children[i]->init(leaf_nodes[i]->index, 
						leaf_nodes[i]->bounds);
			this->bounds.expand_to(leaf_nodes[i]->bounds);
		}
		return;
	}

	/* get the bounding box for all the shapes */
	midpoints.reset();
	for(i = 0; i < num; i++)
	{
		/* compute bounding box */
		this->bounds.expand_to(leaf_nodes[i]->bounds);

		/* compute midpoint of this leaf, and update
		 * our 'midpoints' bounding box, which we will
		 * use to split the subnodes */
		mp = leaf_nodes[i]->midpoint();
		midpoints.expand_to(mp);
	}

	/* Determine which dimension has the largest bounds.
	 * This is the dimension we will want to split */
	dim_size = 0.0f;
	dim_to_split = 0;
	for(di = 0; di < NUM_DIMS; di++)
	{
		/* get the span of this dimension */
		len = midpoints.max(di) - midpoints.min(di);
		if(len > dim_size)
		{
			/* this is the largest dimension so far */
			dim_size = len;
			dim_to_split = di;
		}
	}

	/* now that we know which dimension we're splitting on,
	 * we want to segment the leaves based on the midpoint of
	 * this box */
	pivot = midpoints.center(dim_to_split);
	for(i = 0; i < num; i++)
	{
		/* get the midpoint of this child */
		child_mid = leaf_nodes[i]->midpoint(dim_to_split);

		/* test this leaf against the current pivot point */
		if(child_mid < pivot)
			left.push_back(leaf_nodes[i]);
		else if(child_mid > pivot)
			right.push_back(leaf_nodes[i]);
		else
		{
			/* the midpoint is right on the pivot,
			 * so it could go either way.  To prevent
			 * degenerate cases, try to balance the two sides */
			if(left.size() < right.size())
				left.push_back(leaf_nodes[i]);
			else
				right.push_back(leaf_nodes[i]);
		}
	}

	/* now we have the input leaf nodes in two split groups, we want
	 * to create children for this node, and recursively sort the
	 * leaves. */
	this->children[0] = new aabb_node_t();
	this->children[0]->init(left);
	this->children[1] = new aabb_node_t();
	this->children[1]->init(right);
}
		
void aabb_node_t::trace(size_t& i_best, float& t_best, 
		           Eigen::Vector3f& n_best, const ray_t& ray,
			   bool shortcircuit, float t_min, float t_max,
			   const std::vector<element_t>& elements) const
{
	Vector3f n;
	float child_t[NUM_CHILDREN_PER_NODE];
	bool  child_intersect[NUM_CHILDREN_PER_NODE];
	size_t i, i_close, i_far;
	float t;

	/* check edge-case:  we've already found a solution and we're
	 * short-circuiting */
	if(shortcircuit && i_best < elements.size())
		return;

	/* check base-case:  this is a leaf? */
	if(this->isleaf())
	{
		/* check if valid leaf */
		if(this->index < 0)
			return; /* contains nothing */

		/* check if the element is intersected */ 
		if(!(elements[this->index].intersects(t,n,ray,t_min,t_max)))
		{
			/* no intersection occurred */
			return;
		}

		/* an intersection occurred, so we need to determine if
		 * it is better than any intersections found so far */
		if(t < t_best)
		{
			/* record as best so far */
			i_best = this->index;
			t_best = t;
			n_best = n;
		}
		return;
	}

	/* this node is not a leaf, so we need to check its subnodes to
	 * see which to explore for intersections */
	for(i = 0; i < NUM_CHILDREN_PER_NODE; i++)
	{
		/* perform intersection test on child, if it exists */
		child_intersect[i] = false;
		if(this->children[i] != NULL)
		{
			/* perform intersection */
			if(!(this->children[i]->bounds.intersects(
							child_t[i], n,
							ray, t_min, t_max)))
				continue; /* no intersection */

			/* we only actually care about this intersection
			 * if it stands a chance of being closer than any
			 * existing values.  Check the ray's entry of the
			 * box to the current best intersection point */
			if(child_t[i] < t_best)
				child_intersect[i] = true; /* we care */
		}
	}

	/* check which children had an intersection */
	if(child_intersect[0] && child_intersect[1])
	{
		/* both children had an intersection, so we need
		 * to explore both.  Start with the closer one */
		if(child_t[0] < child_t[1])
		{
			/* #0 is closer than #1 */
			i_close = 0;
			i_far = 1;
		}
		else
		{
			/* #1 is closer than #0 */
			i_close = 1;
			i_far = 0;
		}

		/* test the closer child */
		this->children[i_close]->trace(i_best, t_best, n_best, ray,
				shortcircuit, t_min, t_max, elements);

		/* check if we should also check the farther child */
		if(t_best < child_t[i_far])
			return; /* don't bother */

		/* check the farther child */
		this->children[i_far]->trace(i_best, t_best, n_best, ray,
				shortcircuit, t_min, t_max, elements);
	}
	else if(child_intersect[0])
	{
		/* only one child intersected.  Just test it */
		this->children[0]->trace(i_best, t_best, n_best, ray,
				shortcircuit, t_min, t_max, elements);
	}
	else if(child_intersect[1])
	{
		/* only one child intersected.  Just test it */
		this->children[1]->trace(i_best, t_best, n_best, ray,
				shortcircuit, t_min, t_max, elements);
	}
	else
	{
		/* no children intersected the ray, we're done here */
		return;
	}
}
		
void aabb_node_t::print(std::ostream& os, const std::string& indent) const
{
	string child_indent = indent + "\t";

	/* print first child */
	if(this->children[0] != NULL)
		this->children[0]->print(os, child_indent);
	
	/* print this node */
	os << indent << this->index << " --- " 
	   <<" ["<< this->bounds.min(0) <<","<< this->bounds.max(0) <<"]" 
	   <<" ["<< this->bounds.min(1) <<","<< this->bounds.max(1) <<"]" 
	   <<" ["<< this->bounds.min(2) <<","<< this->bounds.max(2) <<"]" 
	   << endl;

	/* print second child */
	if(this->children[1] != NULL)
		this->children[1]->print(os, child_indent);
}
