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
#include <float.h>

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
	/* don't need to do anything here */
}
		
int scene_t::init(const std::string& filename)
{
	// TODO implement me

	// TODO debugging:  hard-code a scene
	this->elements.resize(6);
	this->elements[0].set(new sphere_t(0.0f,0.0f,-26.0f,2.0f));
	this->elements[1].set(new sphere_t(2.0f,10.0f,-75.0f,2.0f));
	this->elements[2].set(new sphere_t(-12.0f,1.0f,-44.0f,2.0f));
	this->elements[3].set(new sphere_t(15.0f,-13.0f,-67.0f,2.0f));
	this->elements[4].set(new sphere_t(-10.0f,-10.0f,-47.0f,2.0f));
	this->elements[5].set(new sphere_t(2.0f,0.0f,-25.0f,1.0f));
	this->lights.resize(1);
	this->lights[0].set(true, Eigen::Vector3f(-5.0f, 10.0f, -26.0f));
	this->lights[0].get_color().set(1.0f, 1.0f, 1.0f);

	/* success */
	return 0;
}
		
color_t scene_t::trace(float u, float v) const
{
	color_t result, curr;
	ray_t ray;
	Vector3f pos, viewdir, normal, normal_best;
	size_t i, i_best, num_elems, j, num_lights;
	float t, t_best;

	/* construct ray from coordinates */
	this->camera.get_ray(ray, u, v);

	/* iterate over the elements of the scene */
	num_elems = this->elements.size();
	num_lights = this->lights.size();
	t_best = FLT_MAX; /* infinite distance */
	i_best = num_elems; /* invalid index */
	for(i = 0; i < num_elems; i++)
	{
		/* ignore invalid elements */
		if(this->elements[i].get_shape() == NULL)
			continue;

		/* check if the given ray intersects this element */
		if(!(this->elements[i].get_shape()->intersects(t,
							normal,ray)))
			continue; /* no intersection */

		/* check if this intersection is closer than
		 * previously discovered intersections */
		if(t > t_best)
			continue; /* this one is occluded */
		if(t < 0.0f)
			continue; /* wrong side of eye */

		/* keep track of best surface seen */
		t_best = t;
		i_best = i;
		normal_best = normal;
	}

	/* check if we saw anything */
	if(i_best == num_elems)
		return result; /* this is a black color */

	/* compute 3D position of intersection */
	pos = ray.point_at(t_best);
	viewdir = this->camera.get_eye() - pos;
	viewdir.normalize();

	/* get coloring from each light */
	for(j = 0; j < num_lights; j++)
	{
		// TODO check for occluding elements (shadows)

		/* get color from this light on best surface */
		curr = this->elements[i_best].get_shader().compute_phong(
				pos, normal_best, viewdir, this->lights[j]);

		/* add current color to result */
		result += curr;
	}	
		
	/* return the final color */
	return result;
}
		
void scene_t::print_info() const
{
	// TODO implement me
}
