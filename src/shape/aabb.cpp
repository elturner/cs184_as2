#include "aabb.h"
#include <shape/shape.h>
#include <shape/ray.h>
#include <geometry/transform.h>
#include <Eigen/Dense>
#include <iostream>

/**
 * @file   aabb.cpp
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Implements axis-aligned bounding box geometry for ray tracing
 *
 * @section DESCRIPTION
 *
 * This file implements the aabb_t class, which is used to represent
 * axis-aligned bounding boxes, for the purposes of ray tracing.
 */

using namespace std;
using namespace Eigen;

/* the following constants are used for this code */

#define NUM_DIMS 3
#define NUM_CORNERS_PER_BOX 8

/*--------------------------*/
/* function implementations */
/*--------------------------*/

void aabb_t::expand_to(const Eigen::Vector3f& p)
{
	size_t i;

	/* iterate over dimensions of box */
	for(i = 0; i < NUM_DIMS; i++)
	{
		/* check if invalid */
		if(this->bounds(i,0) > this->bounds(i,1))
			this->bounds(i,0) = this->bounds(i,1) = p(i);
		else
		{
			/* update bounds */
			if(this->bounds(i,0) > p(i))
				this->bounds(i,0) = p(i);
			if(this->bounds(i,1) < p(i))
				this->bounds(i,1) = p(i);
		}
	}
}

void aabb_t::apply(const transform_t& t)
{
	Eigen::Vector3f orig_corners[NUM_CORNERS_PER_BOX];
	Eigen::Vector3f trans_corners[NUM_CORNERS_PER_BOX];
	size_t ci, di;

	/* compute the positions of the original box corners */
	for(ci = 0; ci < NUM_CORNERS_PER_BOX; ci++)
	{
		/* iterate through the dims of this corner */
		for(di = 0; di < NUM_DIMS; di++)
		{
			/* is this corner at the min or max for
			 * the current coordinate dimension? */
			orig_corners[ci](di) 
				= this->bounds(di, ((ci >> di) & 1) ); 
		}
	}

	/* compute the transformed position for these corners */
	for(ci = 0; ci < NUM_CORNERS_PER_BOX; ci++)
	{
		/* transform this corner */
		trans_corners[ci] = t.apply(orig_corners[ci]);
	}

	/* set this box to the first corner */
	for(di = 0; di < NUM_DIMS; di++)
		this->bounds(di,0) 
			= this->bounds(di,1) = trans_corners[0](di);

	/* expand box to the rest of the corners */
	for(ci = 1; ci < NUM_CORNERS_PER_BOX; ci++)
	{
		for(di = 0; di < NUM_DIMS; di++)
		{
			if(this->bounds(di,0) > trans_corners[ci](di))
				this->bounds(di,0) 
					= trans_corners[ci](di);
			if(this->bounds(di,1) < trans_corners[ci](di))
				this->bounds(di,1) 
					= trans_corners[ci](di);
		}
	}
}
		
bool aabb_t::intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const
{
	float invdir[NUM_DIMS];
	int s[NUM_DIMS];
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	size_t i, tmin_dim, tmax_dim;

	/* prepare cached values for this ray */
	for(i = 0; i < NUM_DIMS; i++)
	{
		invdir[i] = 1.0f / r.dir()(i);
		s[i] = (invdir[i] < 0);
	}
	
	/* we start by assuming the rays intersect at the x-faces */
	tmin_dim = tmax_dim = 0;

	/* compute intersections in x-coordinates */
	tmin = (bounds(0, s[0])   - r.get_origin()(0)) * invdir[0];
	tmax = (bounds(0, 1-s[0]) - r.get_origin()(0)) * invdir[0];

	/* check against input range */
	if( (tmax < t_min) || (tmin > t_max) )
		return false; /* guaranteed out-of-bounds */

	/* get equivalent values for y-coordinates */
	tymin = (bounds(1, s[1])   - r.get_origin()(1)) * invdir[1];
	tymax = (bounds(1, 1-s[1]) - r.get_origin()(1)) * invdir[1];

	/* check that line intersects the xy rectangle of the projection
	 * onto this box */
	if( (tmin > tymax) || (tymin > tmax) )
		return false; /* no intersection */
	if(tymin > tmin)
	{
		tmin = tymin; /* intersects y after x when entering */
		tmin_dim = 1; /* intersects y-plane */
	}
	if(tymax < tmax)
	{
		tmax = tymax; /* intersects y before x when leaving */
		tmax_dim = 1; /* intersects y-plane */
	}
	
	/* check against input range */
	if( (tmax < t_min) || (tmin > t_max) )
		return false; /* guaranteed out-of-bounds */

	/* get intersects points for the z-planes */
	tzmin = (bounds(2, s[2])   - r.get_origin()(2)) * invdir[2];
	tzmax = (bounds(2, 1-s[2]) - r.get_origin()(2)) * invdir[2];

	/* check cube intersections */
	if( (tmin > tzmax) || (tzmin > tmax) )
		return false; /* no intersection */
	if(tzmin > tmin)
	{
		tmin = tzmin; /* intersects z after x when entering */
		tmin_dim = 2; /* intersects z-plane */
	}
	if(tzmax < tmax)
	{
		tmax = tzmax; /* intersects z before x when leaving */
		tmax_dim = 2;
	}
	
	/* check against input range */
	if( (tmax < t_min) || (tmin > t_max) )
		return false; /* guaranteed out-of-bounds */

	/* at this point, we're guaranteed an intersection, so now
	 * we need to check which values are in range */
	n << 0.0f, 0.0f, 0.0f;
	if(tmin < t_min)
	{
		/* The ray starts within the box.
		 *
		 * Since the box is modeled as 'solid', we still
		 * want to consider it an intersection */
		n(tmax_dim) = s[tmax_dim] ? 1.0f : -1.0f;
		t = t_min; /* embedded in box */
	}
	else
	{
		/* the front-face of cube is intersected */
		n(tmin_dim) = s[tmin_dim] ? 1.0f : -1.0f;
		t = tmin; /* surface of box */
	}
	
	/* we intersect the ray */
	return true;
}
