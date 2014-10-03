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

/* the following defines are used in this code */
#define EPSILON 0.001

/*--------------------------*/
/* function implementations */
/*--------------------------*/
		
scene_t::scene_t()
{
	/* don't need to do anything here */
}
	
scene_t::~scene_t()
{
	size_t i, n;

	/* frees all dynamically allocated shapes */
	n = this->elements.size();
	for(i = 0; i < n; i++)
	{
		/* check if this element has a shape */
		if(this->elements[i].get_shape() == NULL)
			continue;

		/* free the shape */
		delete (this->elements[i].get_shape());
		this->elements[i].set(NULL);
	}
	this->elements.clear();

	/* free all lights */
	this->lights.clear();
}
		
int scene_t::init(const std::string& filename)
{
	// TODO implement me

	// TODO debugging:  hard-code a scene
	this->elements.resize(7);
	this->elements[0].set(new sphere_t(5.0f,-6.0f,-36.0f,2.0f));
	this->elements[0].get_shader().kd.set(0.0f, 1.0f, 0.0f);
	this->elements[1].set(new sphere_t(1.0f,10.0f,-55.0f,10.0f));
	this->elements[1].get_shader().ka.set(0.0f, 0.0f, 0.0f);
	this->elements[1].get_shader().kd.set(0.0f, 0.0f, 0.0f);
	this->elements[1].get_shader().ks.set(0.9f, 0.9f, 0.9f);
	this->elements[1].get_shader().p = 1000;
	this->elements[1].get_shader().kr.set(0.9f, 0.9f, 0.9f);
	this->elements[2].set(new sphere_t(-12.0f,2.0f,-44.0f,2.0f));
	this->elements[2].get_shader().kd.set(0.0f, 0.0f, 1.0f);
	this->elements[3].set(new sphere_t(15.0f,-5.0f,-67.0f,5.0f));
	this->elements[3].get_shader().ka.set(0.0f, 0.0f, 0.0f);
	this->elements[3].get_shader().kd.set(0.0f, 0.0f, 0.0f);
	this->elements[3].get_shader().ks.set(0.9f, 0.9f, 0.9f);
	this->elements[3].get_shader().p = 1000;
	this->elements[3].get_shader().kr.set(0.9f, 0.9f, 0.9f);
	this->elements[4].set(new sphere_t(-10.0f,-4.0f,-47.0f,6.0f));
	this->elements[4].get_shader().ka.set(0.0f, 0.0f, 0.0f);
	this->elements[4].get_shader().kd.set(0.0f, 0.0f, 0.0f);
	this->elements[4].get_shader().ks.set(0.9f, 0.9f, 0.9f);
	this->elements[4].get_shader().p = 1000;
	this->elements[4].get_shader().kr.set(0.9f, 0.9f, 0.9f);
	this->elements[5].set(new sphere_t(5.0f,-4.0f,-35.0f,1.0f));
	this->elements[6].set(new sphere_t(0.0f,-10000.0f,-25.0f,9990.0f));
	this->elements[6].get_shader().kd.set(0.8f, 0.8f, 0.8f);
	this->lights.resize(3);
	this->lights[0].set(true, Eigen::Vector3f(-5.0f, 10.0f, -22.0f));
	this->lights[0].get_color().set(1.0f, 1.0f, 1.0f);
	this->lights[1].set(true, Eigen::Vector3f(-1.0f, 10.0f, -22.0f));
	this->lights[1].get_color().set(1.0f, 1.0f, 1.0f);
	this->lights[2].set(true, Eigen::Vector3f(-1.0f, 10.0f, -27.0f));
	this->lights[2].get_color().set(1.0f, 1.0f, 1.0f);
	this->recursion_depth = 3;

	/* success */
	return 0;
}
		
color_t scene_t::trace(float u, float v) const
{
	ray_t ray;

	/* construct ray from coordinates */
	this->camera.get_ray(ray, u, v);

	/* trace this ray through the scene */
	return this->trace(ray, this->recursion_depth); 
}

color_t scene_t::trace(const ray_t& ray, int r) const
{
	color_t result, curr;
	ray_t shadow, bounce;
	Vector3f pos, viewdir, normal, normal_best, lightdir;
	size_t i, i_best, num_elems, j, num_lights;
	float t, t_best, lightdist;
	bool isshadowed;

	/* check for base case */
	if(r < 0)
		return result; /* we passed the recursion depth */

	/*--------------------------*/
	/* find object in the scene */
	/*--------------------------*/

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
						normal, ray,
						0.0f, t_best)))
			continue; /* no intersection */

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

	/*------------------------------*/
	/* get coloring from each light */
	/*------------------------------*/
	
	for(j = 0; j < num_lights; j++)
	{
		/* get direction from surface to this light */
		lightdir = -(this->lights[j].get_direction(pos));
		lightdist = this->lights[j].get_distance(pos);
		shadow.set(pos, lightdir);

		/* check for occluding elements (shadows) */
		isshadowed = false;
		for(i = 0; i < num_elems; i++)
		{
			/* check if the i'th element shadows this 
			 * position */
			if(!(this->elements[i].get_shape()->intersects(t,
						normal, shadow,
						EPSILON, lightdist)))
				continue; /* no intersection */

			/* the i'th element shadows us from this
			 * light source.  Don't shade here. */
			isshadowed = true;
			break;
		}

		/* only proceed if we are not shadowed */
		if(isshadowed)
			continue;

		/* get color from this light on best surface */
		curr = this->elements[i_best].get_shader().compute_phong(
				pos, normal_best, viewdir, this->lights[j]);

		/* add current color to result */
		result += curr;
	}	

	/*---------------------------------------------------*/
	/* get coloring from any reflections in this surface */
	/*---------------------------------------------------*/

	/* compute bounce direction */
	bounce.set(pos, -viewdir + 2*viewdir.dot(normal_best)*normal_best);
	
	/* recursively trace through the scene */
	result += (this->elements[i_best].get_shader().kr 
			* this->trace(bounce, r-1));

	/* return the final color */
	return result;
}
		
void scene_t::print_info() const
{
	// TODO implement me
}
