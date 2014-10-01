#include "scene.h"
#include <util/error_codes.h>
#include <color/color.h>
#include <shape/ray.h>
#include <shape/shape.h>
#include <shape/sphere.h>
#include <scene/light.h>
#include <scene/camera.h>
#include <scene/element.h>
#include <Eigen/Dense>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @file   scene.cpp
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  The scene_t class represents all scene components
 *
 * @section DESCRIPTION
 *
 * This file containts the scene_t class, which is used to hold
 * all components of a scene.  These components include objects,
 * lighting, and materials.
 */

using namespace std;
using namespace Eigen;

/*--------------------------*/
/* function implementations */
/*--------------------------*/
		
scene_t::scene_t()
{
	// TODO implement me
}
		
int scene_t::init(const std::string& filename)
{
	// TODO implement me

	// TODO debugging:  hard-code a sphere
	this->elements.resize(1);
	this->elements.back().set(new sphere_t(0.0f,0.0f,-2.0f,2.0f));

	/* success */
	return 0;
}
		
color_t scene_t::trace(float u, float v) const
{
	color_t result;
	ray_t ray;
	Vector3f normal;
	size_t i, n;
	float t;

	/* construct ray from coordinates */
	this->camera.get_ray(ray, u, v);

	/* iterate over the elements of the scene */
	n = this->elements.size();
	for(i = 0; i < n; i++)
	{
		/* check if the given ray intersects this element */
		if(!(this->elements[i].get_shape()->intersects(t,
							normal,ray)))
			continue; /* no intersection */

		/* an intersection occurred, color red */
		result.set(1.0f, 0.0f, 0.0f);
	
		// TODO record closest positive t
	}
		
	// TODO perform actual raytracing with shading

	/* return the final color */
	return result;
}
		
void scene_t::print_info() const
{
	// TODO implement me
}
