#ifndef TRIANGLE_H
#define TRIANGLE_H

/**
 * @file   triangle.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  Defines the triangle shape for purposes of raytracing
 *
 * @section DESCRIPTION
 *
 * This file defines the triangle_t class, which represents a triangle
 * in the ray-tracing scene.  It has the functionality to intersect rays
 * in order to be rendered with a given texture.
 */

#include <shape/shape.h>
#include <shape/aabb.h>
#include <Eigen/Dense>
#include <iostream>

/**
 * The triangle_t class represents a triangle in the ray-tracing scene
 *
 * It has the functionality to intersect rays, and extends the shape_t
 * virtual class.
 */
class triangle_t : public shape_t
{
	/* fundamental constants */
	public:

		/**
		 * This value represents the number of vertices in a
		 * triangle.
		 */
		static const size_t NUM_VERTS_PER_TRI = 3;

		/**
		 * The dimensionality of a triangle.  Because a triangle
		 * is a 2D surface, it has dimensionality = 2.  This means
		 * that the surface of a triangle can be parameterized
		 * with two coordinates.
		 */
		static const size_t NUM_DIMS_PER_TRI = 2;

	/* parameters */
	private:

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * These points represent the vertices in this
		 * triangle
		 */
		Eigen::Vector3f verts[NUM_VERTS_PER_TRI];

		/*-------*/
		/* cache */
		/*-------*/

		/**
		 * This value is a cached computation of the normal
		 * vector of this triangle.  Note that, depending on
		 * the direction the triangle is being viewed, this vector
		 * may be pointing away from the camera.
		 */
		Eigen::Vector3f normal;

		/**
		 * These values denote the edge vectors of the triangle.
		 * Namely, edges[0] = verts[1] - verts[0] 
		 * and edges[1] = verts[2] - verts[0]
		 */
		Eigen::Vector3f edges[NUM_DIMS_PER_TRI];

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Default constructor makes a standard triangle
		 */
		triangle_t()
		{
			this->verts[0] << 0.0f,0.0f,0.0f,
			this->verts[1] << 1.0f,0.0f,0.0f,
			this->verts[2] << 0.0f,1.0f,0.0f,
			this->normal   << 0.0f,0.0f,1.0f,
			this->edges[0] << 1.0f,0.0f,0.0f;
			this->edges[1] << 0.0f,1.0f,0.0f;
		};

		/**
		 * Constructs triangle from given other triangle
		 *
		 * @param other   The triangle to copy
		 */
		triangle_t(const triangle_t& other)
		{
			this->verts[0] = other.verts[0];
			this->verts[1] = other.verts[1];
			this->verts[2] = other.verts[2];
			this->normal   = other.normal;
			this->edges[0] = other.edges[0];
			this->edges[1] = other.edges[1];
		};

		/**
		 * Constructs triangle from coordinates of input vertices
		 *
		 * @param ax   X-coordiante for vertex a
		 * @param ay   Y-coordiante for vertex a
		 * @param az   Z-coordiante for vertex a
		 * @param bx   X-coordiante for vertex b
		 * @param by   Y-coordiante for vertex b
		 * @param bz   Z-coordiante for vertex b
		 * @param cx   X-coordiante for vertex c
		 * @param cy   Y-coordiante for vertex c
		 * @param cz   Z-coordiante for vertex c
		 */
		triangle_t(float ax, float ay, float az,
				float bx, float by, float bz,
				float cx, float cy, float cz)
		{
			/* copy vertices */
			this->verts[0] << ax,ay,az;
			this->verts[1] << bx,by,bz;
			this->verts[2] << cx,cy,cz;
			
			/* construct edges */
			this->edges[0] = this->verts[1] - this->verts[0];
			this->edges[1] = this->verts[2] - this->verts[0];
			
			/* construct the normal */
			this->normal = this->edges[0].cross(this->edges[1]);
			this->normal.normalize();
		};

		/**
		 * Constructs triangle from input vertices
		 *
		 * @param a   The first vertex
		 * @param b   The second vertex
		 * @param c   The third vertex
		 */
		triangle_t(const Eigen::Vector3f& a, 
				const Eigen::Vector3f& b,
				const Eigen::Vector3f& c)
		{
			/* copy vertices */
			this->verts[0] = a;
			this->verts[1] = b;
			this->verts[2] = c;
			
			/* construct edges */
			this->edges[0] = this->verts[1] - this->verts[0];
			this->edges[1] = this->verts[2] - this->verts[0];
			
			/* construct the normal */
			this->normal = this->edges[0].cross(this->edges[1]);
			this->normal.normalize();
		};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Retrieves a const reference to the given vertex
		 *
		 * Will return a reference to the specified vertex,
		 * which is represented as a point in 3D space.
		 *
		 * @param i  The index of the vertex to retrieve, [0,1,2]
		 *
		 * @return   Returns const reference to vertex #i
		 */
		inline const Eigen::Vector3f& get_vertex(size_t i) const
		{ return this->verts[i]; };

		/**
		 * Retrieves the normal vector of this triangle
		 *
		 * Will return a constant reference to the normal vector
		 * of this triangle.  Note that the normal may be pointing
		 * away from the camera.
		 *
		 * @return    The normal vector of this triangle
		 */
		inline const Eigen::Vector3f& get_normal() const
		{ return this->normal; };

		/**
		 * Sets the properties of this triangle to the given
		 * values.
		 *
		 * The values are specified as coordinates for the
		 * vertices of the triangle
		 *
		 * @param ax   X-coordiante for vertex a
		 * @param ay   Y-coordiante for vertex a
		 * @param az   Z-coordiante for vertex a
		 * @param bx   X-coordiante for vertex b
		 * @param by   Y-coordiante for vertex b
		 * @param bz   Z-coordiante for vertex b
		 * @param cx   X-coordiante for vertex c
		 * @param cy   Y-coordiante for vertex c
		 * @param cz   Z-coordiante for vertex c
		 */
		inline void set(float ax, float ay, float az,
				float bx, float by, float bz,
				float cx, float cy, float cz)
		{
			/* set the vertices */
			this->verts[0] << ax, ay, az;
			this->verts[1] << bx, by, bz;
			this->verts[2] << cx, cy, cz;

			/* construct edges */
			this->edges[0] = this->verts[1] - this->verts[0];
			this->edges[1] = this->verts[2] - this->verts[0];
			
			/* construct the normal */
			this->normal = this->edges[0].cross(this->edges[1]);
			this->normal.normalize();
		};

		/**
		 * Checks if this shape intersects a ray
		 *
		 * By checking this shape against the parameters
		 * of a ray, should determine if the 3D shape intersects 
		 * the given ray.
		 *
		 * @param t  Where to store the distance along the ray
		 *           where the intersection occurs
		 * @param n  The normal vector at the point of intersection
		 *           on the surface 
		 * @param r  The ray to analyze
		 * @param t_min  The minimum valid t value to cause x-tion
		 * @param t_max  The maximum valid t value to cause x-tion
		 *
		 * @return   Returns true iff the shape intersects the ray
		 */
		inline bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const
		{
			Eigen::Vector3f pvec, tvec, qvec;
			float det, inv_det, u, v;

			/* The following algorithm borrowed from:
			 *
			 * T. Moller and B. Trumbore, "Fast, Minimum
			 * Storage Ray/Triangle Intersection," 1997
			 *
			 * http://www.graphics.cornell.edu/pubs/1997
			 * 				/MT97.pdf
			 */

			/* compute determinant */
			pvec = r.dir().cross(this->edges[1]);
			det = this->edges[0].dot(pvec);
			inv_det = 1.0f / det;

			/* compute distance from verts[0] to ray origin */
			tvec = r.get_origin() - this->verts[0];

			/* Compute the u-coordinate along the triangle */
			u = tvec.dot(pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				return false; /* no intersection */

			/* prepare to test v-coordinate */
			qvec = tvec.cross(this->edges[0]);
			
			/* compute v-coordinate */
			v = r.dir().dot(qvec) * inv_det;
			if(v < 0.0f || (u+v) > 1.0f)
				return false; /* no intersection */

			/* at this point, we know that the ray
			 * intersects the triangle.  compute t-value */
			t = this->edges[1].dot(qvec) * inv_det;

			/* check if the t-value is within bounds */
			if(t < t_min || t > t_max)
				return false; /* out of bounds */

			/* record normal value */
			n = this->normal;
			if(det < 0)
				n *= -1; /* reverse normal direction */

			/* return successful intersection */
			return true;
		};
		
		/**
		 * Populates the axis-aligned bounding box for this shape
		 *
		 * Will generate the bounding box for this shape, and modify
		 * the given bounding box to represent the tightest bounds
		 * to this shape, restricted to being axis-aligned.
		 *
		 * @param bounds   Where to store the axis-aligned bounding
		 *                 box for this shape.
		 */
		void get_bounds(aabb_t& bounds) const
		{
			size_t i;

			/* insert the three vertices into bounding box */
			bounds.reset();
			for(i = 0; i < NUM_VERTS_PER_TRI; i++)
				bounds.expand_to(this->verts[i]);
		};
};

#endif
