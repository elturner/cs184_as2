#include "transform.h"

/**
 * @file   transform.cpp
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Implements the transform_t class
 * 
 * @section DESCRIPTION
 *
 * This file defines the transform_t class, which is used
 * to represent a rigid-body transform between coordinate
 * systems.
 */

#include <shape/ray.h>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

/* function implementations */

transform_t::transform_t()
{
	/* eigen structures to identity values */
	
}

transform_t::~transform_t()
{
	/* no processing necessary */
}
		
void transform_t::cat(const transform_t& t)
{
	// TODO
}
		
