#include "scene.h"
#include <util/cmd_args.h>
#include <util/error_codes.h>
#include <scene/light.h>
#include <scene/phong_shader.h>
#include <scene/sphere.h>
#include <scene/camera.h>
#include <Eigen/Dense>
#include <iostream>
#include <sstream>
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

/* The following constants are used to parse command-line args */
#define KA_FLAG     "-ka"
#define KD_FLAG     "-kd"
#define KS_FLAG     "-ks"
#define SP_FLAG     "-sp"
#define PL_FLAG     "-pl"
#define DL_FLAG     "-dl"
#define SPHERE_FLAG "--sphere"

/*--------------------------*/
/* function implementations */
/*--------------------------*/
		
scene_t::scene_t()
{
	/* default scene has one sphere and no lights */
	this->spheres.resize(1);
	this->rotate_lights = false;
}
		
int scene_t::init(int argc, char** argv)
{
	vector<string> params;
	stringstream ss;
	cmd_args_t args;
	float x, y, z, r, g, b;
	size_t i, n;
	int ret;

	/* initialize command-line arguments */
	args.set_program_description("CS-184 Assignment #1, Fall 2014\n\n"
			"This program will render a scene of one or more "
			"spheres with phong shading based on the provided "
			"material properties and light sources.");
	args.add(KA_FLAG, "Specifies ambient color coefficients:\n\n"
			"\t<R> <G> <B>", true, 3);
	args.add(KD_FLAG, "Specifies diffuse color coefficients:\n\n"
			"\t<R> <G> <B>", true, 3);
	args.add(KS_FLAG, "Specifies specular color coefficients:\n\n"
			"\t<R> <G> <B>", true, 3);
	args.add(SP_FLAG, "Specifies specular 'shinyness' exponent",
			true, 1);
	args.add(PL_FLAG, "Specifies a point light-source at the given "
			"position with the given color:\n\n"
			"\t<X> <Y> <Z> <R> <G> <B>", true, 6);
	args.add(DL_FLAG, "Specifies a directional light-source with the "
			"given direction and color:\n\n"
			"\t<X> <Y> <Z> <R> <G> <B>", true, 6);
	args.add(SPHERE_FLAG, "Adds another sphere to the environment with "
			"the specified geometry:\n\n"
			"\t<CENTER_X> <CENTER_Y> <CENTER_Z> <RADIUS>",
			true, 4);

	/* parse the command-line arguments */
	ret = args.parse(argc, argv);
	if(ret)
		return PROPEGATE_ERROR(-1, ret); /* error occurred */

	/* get and store the arguments */

	/* record ka coefficient */
	params.clear();
	if(args.tag_seen(KA_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get the values for ka coefficient */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* set this coefficient */
		for(i = 0; i < n; i += 3)
		{
			ss >> r >> g >> b;
			this->shader.ka.set(r, g, b);
		}
	}

	/* record kd coefficient */
	params.clear();
	if(args.tag_seen(KD_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get the values for kd coefficient */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* set this coefficient */
		for(i = 0; i < n; i += 3)
		{
			ss >> r >> g >> b;
			this->shader.kd.set(r, g, b);
		}
	}
	
	/* record ks coefficient */
	params.clear();
	if(args.tag_seen(KS_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get the values for ks coefficient */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* set this coefficient */
		for(i = 0; i < n; i += 3)
		{
			ss >> r >> g >> b;
			this->shader.ks.set(r, g, b);
		}
	}

	/* record specular p exponent */
	if(args.tag_seen(SP_FLAG))
		this->shader.p = args.get_val_as<float>(SP_FLAG);

	/* get point light sources */
	params.clear();
	if(args.tag_seen(PL_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get the values for pl coefficient */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* set this coefficient */
		for(i = 0; i < n; i += 6)
		{
			/* record this light source */
			ss >> x >> y >> z >> r >> g >> b;
			this->lights.push_back(
				light_t(true, x, y, z, r, g, b));
		}
	}
	
	/* get directional light sources */
	params.clear();
	if(args.tag_seen(DL_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get the values for dl coefficient */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* set this coefficient */
		for(i = 0; i < n; i += 6)
		{
			/* record this light source */
			ss >> x >> y >> z >> r >> g >> b;
			this->lights.push_back(
				light_t(false, x, y, z, r, g, b));
		}
	}

	/* get additional spheres */
	params.clear();
	if(args.tag_seen(SPHERE_FLAG, params))
	{
		/* set the tags */
		ss.clear();
		ss.str("");
		n = params.size();

		/* get values for sphere coefficients */
		for(i = 0; i < n; i++)
			ss << params[i] << " ";

		/* add the spheres */
		for(i = 0; i < n; i += 4)
		{
			/* add a sphere */
			ss >> x >> y >> z >> r;
			this->spheres.push_back(sphere_t(x,y,z,r));
		}
	}

	/* success */
	return 0;
}

void scene_t::set_display_size(int w, int h)
{
	/* record the display size in the camera */
	this->camera.set_display(w, h);

	/* we want the first sphere to be front-and-center */
	this->camera.set_scale(this->spheres);
}

void scene_t::render()
{
	size_t i, n;

	/* iterate over the spheres, rendering them */
	n = this->spheres.size();
	for(i = 0; i < n; i++)
	{
		/* render the sphere with our camera */
		this->camera.render(this->spheres, i, this->lights,
					this->shader);
	}

	/* check if we're rotating */
	if(this->rotate_lights)
	{
		/* rotate the lights */
		n = this->lights.size();
		for(i = 0; i < n; i++)
			this->lights[i].rotateY(0.01745f * 5.0f); 
					/* 1 degree/frame */
	}
}
		
void scene_t::print_info() const
{
	cout << endl
	     << "Scene Info" << endl
	     << "-------------------------" << endl
	     << "# spheres  :   " << this->spheres.size() << endl
	     << "# lights   :   " << this->lights.size() << endl
	     << "Shader ka  :   ";
	this->shader.ka.print(cout);
	cout << endl << "  ''   kd  :   ";
	this->shader.kd.print(cout);
	cout << endl << "  ''   ks  :   ";
	this->shader.ks.print(cout);
	cout << endl << "  ''   p   :   " << this->shader.p << endl
	     << "-------------------------" << endl
	     << endl
	     << endl
	     << "Controls:" << endl
	     << "-------------------------" << endl
	     << "[spacebar] : exits program" << endl
	     << "[r]        : toggle light rotation" << endl
	     << "-------------------------" << endl
	     << endl;
	     
}
