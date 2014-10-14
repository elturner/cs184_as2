#include "scene.h"
#include <util/error_codes.h>
#include <io/mesh/mesh_io.h>
#include <color/color.h>
#include <shape/ray.h>
#include <shape/shape.h>
#include <shape/sphere.h>
#include <shape/triangle.h>
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
		this->elements[i].set_shape(NULL);
	}
	this->elements.clear();

	/* free all lights */
	this->lights.clear();
}
		
int scene_t::init(const std::string& filename, int rd, bool debug)
{
	/* prepare scene parameters */
	this->recursion_depth = rd;
	this->render_normal_shading = debug;

	// TODO implement me

	transform_t trans;
	phong_shader_t shader;
	light_t light;

	shader.ka.set(0.0f,0.0f,0.0f);
	shader.kd.set(0.0f, 0.0f, 0.0f);
	shader.ks.set(0.9f, 0.9f, 0.9f);
	shader.p = 1000;
	shader.kr.set(0.9f, 0.9f, 0.9f);
	trans.reset();
	trans.append_translation(3.0f, 4.0f, -30.0f);
	trans.append_rotation(45.0f, 45.0f, 45.0f);
	trans.append_scale(5.0f, 1.0f, 1.0f);
	this->add(new sphere_t(0.0f, 0.0f, 0.0f, 1.0f), trans, shader);

	trans.reset();
	trans.append_translation(-5.0f, -9.0f, -45.0f);
	trans.append_scale(5.0f, 5.0f, 5.0f);
	this->add(new sphere_t(0.0f,0.0f,0.0f,1.0f), trans, shader);

	shader.ka.set(0.1f,0.1f,0.1f);
	shader.kd.set(0.4f,0.3f,0.3f);
	shader.ks.set(0.4f,0.2f,0.2f);
	shader.p = 1000;
	shader.kr.set(0.1f,0.1f,0.1f);
	trans.reset();
	trans.append_scale(30.0f, 30.0f, 200.0f);
	trans.append_translation(-0.5f,-0.5f,-0.5f);
	this->add(mesh_io::mesh_t(string("input/cube.obj")), trans, shader);

	light.set_point(Eigen::Vector3f(-3.0f, 10.0f, -15.0f));
	light.get_color().set(1.0f, 1.0f, 1.0f);
	this->add(light);
	
	light.set_point(Eigen::Vector3f(-1.0f, 10.0f, -15.0f));
	light.get_color().set(1.0f, 1.0f, 1.0f);
	this->add(light);
	
	light.set_point(Eigen::Vector3f(-1.0f, 10.0f, -17.0f));
	light.get_color().set(1.0f, 1.0f, 1.0f);
	this->add(light);
	
	light.set_ambient();
	light.get_color().set(0.2f, 0.2f, 0.2f);
	this->add(light);

	/* success */
	return 0;
}
		
void scene_t::add(const mesh_io::mesh_t& mesh,
			const transform_t& transform,
				const phong_shader_t& shader)
{
	mesh_io::polygon_t poly;
	size_t i, n;

	/* Iterate over the polygons in this mesh.
	 * For the purposes of this function, we assume that every
	 * polygon is a triangle. */
	n = mesh.num_polys();
	for(i = 0; i < n; i++)
	{
		/* get this polygon */
		poly = mesh.get_poly(i);
		const mesh_io::vertex_t& a =mesh.get_vert(poly.vertices[0]);
		const mesh_io::vertex_t& b =mesh.get_vert(poly.vertices[1]);
		const mesh_io::vertex_t& c =mesh.get_vert(poly.vertices[2]);

		/* add triangle to this scene based on the given polygon */
		this->add(new triangle_t(a.x, a.y, a.z, 
					b.x, b.y, b.z, 
					c.x, c.y, c.z), transform, shader);
	}
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
	color_t result;
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
		/* check if the given ray intersects this element */
		if(!(this->elements[i].intersects(t, normal, ray,
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

	/* for debugging purposes, we want the ability to render
	 * the simplest representation of the scene.  If this
	 * flag is set, then will render just the normal map of
	 * the scene, without any advanced shading. */
	if(this->render_normal_shading)
		return this->elements[i_best].compute_normal_shading(
						normal_best);

	/* compute 3D position of intersection */
	pos = ray.point_at(t_best);
	viewdir = this->camera.get_eye() - pos;
	viewdir.normalize();

	/*------------------------------*/
	/* get coloring from each light */
	/*------------------------------*/
	
	for(j = 0; j < num_lights; j++)
	{
		/* check if this is an ambient light source */
		if(this->lights[j].is_ambient())
		{
			/* always apply ambient lights */
			result += this->elements[i_best].compute_ambient(
					this->lights[j]);
			continue;
		}

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
			if(!(this->elements[i].intersects(t, normal, shadow,
						EPSILON, lightdist)))
				continue; /* no intersection */

			/* the i'th element shadows us from this
			 * light source.  Don't shade here. */
			isshadowed = true;
			break;
		}

		/* apply effect of this light to scene */
		if(!isshadowed)
		{
			/* get color from this light on best surface if
			 * not shadowed */
			result += this->elements[i_best].compute_phong(
				pos, normal_best, viewdir, this->lights[j]);
		}
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
