#ifndef CAMERA_H
#define CAMERA_H

/**
 * @file    camera.h
 * @author  Eric Turner <elturner@eecs.berkeley.edu>
 * @brief   Defines the camera position, orientation, and viewing
 *
 * @section DESCRIPTION
 *
 * The camera_t class is used to represent the camera's position
 * in space, its orientation, as well as other viewing characteristics
 *
 * The camera is also responsible for making low-level rendering
 * calls to opengl.
 */

#include <shape/ray.h>
#include <Eigen/Dense>

/**
 * The camera_t class is used for representing the camera's attributes
 * and performing low-level rendering tasks.
 */
class camera_t
{
	/* parameters */
	private:

		/**
		 * The position of the camera (eye) in 3D space 
		 */
		Eigen::Vector3f eye;

		/**
		 * The position of the viewing plane in 3D space
		 *
		 * These four values represent the corner positions
		 * of the viewing plane's rectangle.
		 */
		Eigen::Vector3f UL, UR, LL, LR;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Default camera constructor
		 */
		camera_t() 
			: eye(0.0f,0.0f,1.0f),
			  UL(-1.0f, 1.0f,0.0f), UR( 1.0f, 1.0f,0.0f),
			  LL(-1.0f,-1.0f,0.0f), LR( 1.0f,-1.0f,0.0f)
		{};

		/*-----------*/
		/* modifiers */
		/*-----------*/

		/** 
		 * Sets the camera parameters
		 *
		 * @param e     The eye-center position of camera
		 * @param d     The viewing direction of camera
		 * @param hFOV  The horizontal field of view (radians)
		 * @param vFOV  The vertical field of view (radians)
		 */
		void set(const Eigen::Vector3f& e, const Eigen::Vector3f& d,
				float hFOV, float vFOV);

		/*----------*/
		/* geometry */
		/*----------*/
		
		/**
		 * Retrieves the ray that corresponds to the given
		 * coordintes on the viewing plane
		 *
		 * Given (u,v) coordinates on the camera's viewing plane,
		 * will compute the corresponding ray in 3D space that
		 * represents that coordinate pair.
		 *
		 * @param ray   Where to store the ray
		 * @param u     The horizontal coordinate [0,1]
		 * @param v     The vertical coordinate [0,1]
		 */
		void get_ray(ray_t& ray, float u, float v) const;
};

#endif
