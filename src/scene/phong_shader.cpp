#include "phong_shader.h"
#include <color/color.h>
#include <scene/light.h>
#include <Eigen/Dense>
#include <cmath>

/**
 * @file   phong_shader.cpp
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  This file implements phong shading operations
 *
 * @section DESCRIPTION
 *
 * The phong_shader_t class is used to represent
 * the phong shader for a scene, and can apply this
 * shading appoach to a given scene element.
 */

using namespace std;
using namespace Eigen;

/* function implementations */

phong_shader_t::phong_shader_t()
{
	/* set default values */
	this->ka.set(0.0f, 0.0f, 0.0f);
	this->kd.set(0.0f, 0.0f, 0.0f);
	this->ks.set(0.0f, 0.0f, 0.0f);
	this->p = 1.0f;
}

color_t phong_shader_t::compute_phong(
				const Eigen::Vector3f& P,
				const Eigen::Vector3f& N,
				const Eigen::Vector3f& V,
				const light_t& light) const
{
	Vector3f L, R;
	color_t C;
	float lndot;

	/* initialize color result for this light */
	C.set(0.0f,0.0f,0.0f);

	/*---------------------*/
	/* add ambient shading */
	/*---------------------*/
		
	C += this->ka;

	/*---------------------*/
	/* add diffuse shading */
	/*---------------------*/
	
	/* direction from light to surface */
	L = light.get_direction(P);
	lndot = L.dot(N);
		
	/* add lighting */
	C += this->kd * std::max(-lndot, 0.0f);

	/*---------------------*/
	/* add specular shading */
	/*---------------------*/
		
	/* compute the reflectance direction */
	R = L - 2*lndot*N;

	/* add lighting */
	C += this->ks * powf(std::max(R.dot(V), 0.0f), this->p);

	/* apply result to the original light color, and 
	 * add to final color for surface */
	C *= light.get_color();

	/* return the final constructed color for this light */
	return C;
}
