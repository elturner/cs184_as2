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
			: eye(0.0f,0.0f,3.0f),
			  UL(-1.0f, 1.0f,0.0f), UR( 1.0f, 1.0f,0.0f),
			  LL(-1.0f,-1.0f,0.0f), LR( 1.0f,-1.0f,0.0f)
		{};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Retrieves the eye position of the camera
		 */
		inline const Eigen::Vector3f get_eye() const
		{ return this->eye; };

		/**
		 * Sets the parameters of this camera
		 *
		 * @param e   The location of the eye, in 3D space
		 * @param ul  The upper-left corner of viewing plane
		 * @param ur  The upper-right corner of viewing plane
		 * @param ll  The lower-left corner of the viewing plane
		 * @param lr  The lower-right corner of the viewing plane
		 */
		inline void set(const Eigen::Vector3f& e,
				const Eigen::Vector3f& ul, 
				const Eigen::Vector3f& ur,
				const Eigen::Vector3f& ll, 
				const Eigen::Vector3f& lr)
		{
			this->eye = e;
			this->UL = ul;
			this->UR = ur;
			this->LL = ll;
			this->LR = lr;
		};

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
