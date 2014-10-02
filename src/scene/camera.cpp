#include "camera.h"
#include <shape/ray.h>
#include <Eigen/Dense>
#include <cmath>
#include <float.h>

/**
 * @file    camera.cpp
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   Defines the camera position, orientation, and viewing
 *
 * @section DESCRIPTION
 *
 * The camera_t class is used to represent the camera's position
 * in space, its orientation, as well as other viewing characteristics
 */

using namespace std;
using namespace Eigen;

/*--------------------------*/
/* function implementations */
/*--------------------------*/

void camera_t::get_ray(ray_t& ray, float u, float v) const
{
	Vector3f p;

	/* compute 3D position of point on viewing plane based
	 * on the coordinates.
	 *
	 * This uses bilinear interpolation 
	 *
	 * 	       --u-->
	 *	   +---------------------+
	 *	   |                     |
	 *	 | |                     |
	 *	 v |                     |
	 *	 | |                     |
	 *	 V |                     |
	 *	   |                     |
	 *	   +---------------------+
	 *
	 */
	p = (1-u)*(  v*(this->LL) + (1-v)*(this->UL)  ) 
		+ u*(  v*(this->LR) + (1-v)*(this->UR)  );

	/* generate ray */
	ray.set(this->eye, p - this->eye);
}
