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
#include <cmath>

using namespace std;
using namespace Eigen;

/*--------------------------------*/
/* the following defines are used */
/*--------------------------------*/

/* define pi */
#ifndef M_PI
#define M_PI 3.14159265358979323846264
#endif

/* convert from degrees to radians */
#define DEG2RAD(x)  ( ((x)*M_PI) / 180.0 )

/*--------------------------*/
/* function implementations */
/*--------------------------*/

transform_t::transform_t()
{
	/* eigen structures to identity values */
	this->reset();
}

transform_t::~transform_t()
{
	/* no processing necessary */
}
		
void transform_t::reset()
{
	/* set this homogenous matrix to the identity */
	this->H << 1,0,0,0,
	           0,1,0,0
	           0,0,1,0
	           0,0,0,1;
	this->H_inv = this->H;
}
		
void transform_t::set_translation(float tx, float ty, float tz)
{
	this->H << 1,0,0,tx,
	           0,1,0,ty
	           0,0,1,tz
	           0,0,0,1;
	this->H_inv = this->H.inverse();
}
		
void transform_t::set_scale(float sx, float sy, float sz)
{
	this->H << sx,  0,  0,  0
	            0, sy,  0,  0
	            0,  0, sz,  0
	            0,  0,  0,  1;
	this->H_inv = this->H.inverse();
}

void transform_t::set_rotation(float rx, float ry, float rz)
{
	Vector3f r;  /* exponential map's unit vector */
	Matrix3f rx; /* cross-product matrix */
	Matrix3f R; /* 3x3 rotation matrix */
	float mag; /* magnitude of exponential map in radians */

	/* compute unit vector and magnitude */
	r(0) = DEG2ROT(rx);
	r(1) = DEG2ROT(ry);
	r(2) = DEG2ROT(rz);
	mag = r.norm();
	r /= mag;

	/* precompute matrix form of the cross-product */
	rx <<	0,	-r(2),	r(1),
		r(2),	0,	-r(0),
		-r(1),	r(0),	0;

	/* use rodriguez to generate rotation matrix */
	R = (r * r.transpose()) + (rx*sin(mag)) - (rx*rx*cos(mag));

	/* make homogenous matrix */
	this->H << R(0,0), R(0,1), R(0,2), 0,
	           R(1,0), R(1,1), R(1,2), 0,
	           R(2,0), R(2,1), R(2,2), 0,
	           0,      0,      0,      1;
}
		
void transform_t::cat(const transform_t& t)
{
	this->H = (t.H * this->H); 
	this->H_inv = this->H.inverse();
}

Eigen::Vector3f transform_t::apply(const Eigen::Vector3f& p) const
{
	Vector4f x; /* the vector to run through matrix */
	Vector3f y; /* the output vector */

	/* copy p into x */
	x << p(0), p(1), p(2), 1;

	/* apply the transform */
	x = (this->H) * x;

	/* get the output vector */
	y << x(0), x(1), x(2);
	return y;
}
		
Eigen::Vector3f transform_t::apply_inverse(const Eigen::Vector3f& p) const
{
	Vector4f x; /* the vector to run through matrix */
	Vector3f y; /* the output vector */

	/* copy p into x */
	x << p(0), p(1), p(2), 1;

	/* apply the transform */
	x = (this->H_inverse) * x;

	/* get the output vector */
	y << x(0), x(1), x(2);
	return y;
}
		
ray_t transform_t::apply(const ray_t& ray) const
{
	Vector3f p, q;
	ray_t output;

	/* get the two points defining the input ray */
	p = ray.get_origin();
	q = p + ray.get_direction();
	
	/* transform the ray */
	p = this->apply(p);
	q = this->apply(q);

	/* construct output ray */
	output.set(p, q-p);
	return output;
}
		
ray_t transform_t::apply_inverse(const ray_t& ray) const
{
	Vector3f p, q;
	ray_t output;

	/* get the two points defining the input ray */
	p = ray.get_origin();
	q = p + ray.get_direction();
	
	/* transform the ray */
	p = this->apply_inverse(p);
	q = this->apply_inverse(q);

	/* construct output ray */
	output.set(p, q-p);
	return output;
}
