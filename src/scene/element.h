#ifndef ELEMENT_H
#define ELEMENT_H

/**
 * @file   element.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  The element_t represents an object in the scene
 *
 * @section DESCRIPTION
 *
 * This file contains the element_t class, which is used to
 * represent an object in the scene.  Each element is defined
 * by a shape and a shading model (which represents material).
 */

#include <shape/shape.h>
#include <shape/aabb.h>
#include <geometry/transform.h>
#include <scene/phong_shader.h>

/**
 * The element_t class is used to represent an object in the scene
 */
class element_t
{
	/* parameters */
	private:

		/**
		 * The geometry of this object
		 */
		shape_t* shape;

		/**
		 * The transform to apply to this element
		 */
		transform_t transform;

		/**
		 * The material properties of this object
		 */
		phong_shader_t shader;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Initializes default element, with no shape
		 */
		element_t()
		{
			/* currently has no shape */
			this->shape = NULL;

			/* set some default texture */
			this->shader.ka.set(0.0f, 0.0f, 0.0f);
			this->shader.kd.set(0.9f, 0.2f, 0.2f);
			this->shader.ks.set(1.0f, 1.0f, 1.0f);
			this->shader.p = 255.0f;
		};

		/**
		 * Frees all memory and resources
		 *
		 * Elements will NOT delete their own shapes
		 */
		~element_t()
		{};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Retrieves the shape for this element
		 *
		 * @return   A pointer to this element's shape
		 */
		inline shape_t* get_shape() const
		{ return this->shape; };

		/**
		 * Set the shape for this element
		 *
		 * @param s  What shape to use for this element
		 */
		inline void set_shape(shape_t* s)
		{ this->shape = s; };

		/**
		 * Gets the transform for this element
		 *
		 * @return   A constant reference to the element's transform
		 */
		inline const transform_t& get_transform() const
		{ return this->transform; };

		/**
		 * Gets the transform for this element
		 *
		 * @return   Returns a non-const reference to the transform
		 */
		inline transform_t& get_transform()
		{ return this->transform; };

		/**
		 * Sets the transform for this element
		 *
		 * @param t   The transform to use for this element
		 */
		inline void set_transform(const transform_t& t)
		{ this->transform = t; };

		/**
		 * Sets this element's shader parameters
		 *
		 * @param s   The shader to use
		 */
		inline void set_shader(const phong_shader_t& s)
		{ this->shader = s; };
		
		/**
		 * Retrieves the shader for this model
		 *
		 * Will retrieve a constant reference to
		 * this model's shader.
		 */
		inline const phong_shader_t& get_shader() const
		{ return this->shader; };
		
		/**
		 * Retrieves the shader for this model
		 *
		 * Will retrieve a non-constant reference to
		 * this model's shader, for modifying
		 */
		inline phong_shader_t& get_shader()
		{ return this->shader; };

		/*----------*/
		/* geometry */
		/*----------*/

		/**
		 * This calls intersect on the element's shape
		 *
		 * This function is a wrapper around this element's
		 * shape's intersect function.  It will apply appropriate
		 * transformations and perform the intersect test.
		 *
		 * @param t      Where to store the distance along the ray
		 *               where the intersection occurs
		 * @param n      The normal vector at the point of 
		 *               intersection on the surface 
		 * @param r      The ray to analyze
		 * @param t_min  The minimum valid t value to cause x-tion
		 * @param t_max  The maximum valid t value to cause x-tion
		 *
		 *
		 * @return   Returns true iff the shape intersects the ray
		 */
		inline bool intersects(float& t, Eigen::Vector3f& n,
		                        const ray_t& r,
					float t_min, float t_max) const
		{
			ray_t s;
			float scale;
			bool res;

			/* check if we have a shape */
			if(this->shape == NULL)
				return false; /* no intersection */

			/* apply transformation to the given ray 
			 * to convert from scene coordinates to object
			 * coordinates */
			s = this->transform.apply_inverse(r, scale);

			/* call the shape's intersect function with the
			 * new ray. */
			res = this->shape->intersects(t,n,s,
						t_min*scale,
						t_max*scale);

			/* if the intersection occurred, then convert
			 * the normal vector from the shape coordinates
			 * to the scene coordinates */
			if(res)
			{
				/* update normal vector */
				n = this->transform.apply_normal(n);

				/* we also need to update the 't' value,
				 * since any scaling between object space
				 * and scene space will have to be accounted
				 * for. */
				t /= scale;
			}

			/* return the result */
			return res;
		};

		/*---------*/
		/* shading */
		/*---------*/

		/**
		 * Computes ambient light shading only on this object
		 *
		 * @param light   The light source to use
		 *
		 * @return  Returns the observed surface color
		 */
		inline color_t compute_ambient(const light_t& light) const
		{
			return (this->shader.ka * light.get_color());
		};

		/**
		 * Computes the phong illumination model for the
		 * specified point on a surface of the shape of this 
		 * element.
		 *
		 * Will transform the specified values to shape
		 * coordinates and then compute color of surface
		 * at this point.
		 *
		 * NOTE:  all directions are assumed to be normalized
		 * vectors.
		 *
		 * @param P        The point on surface to analyze
		 * @param N        The normal of the surface, in 
		 *                 world coordinates.
		 * @param V        The direction from the surface 
		 *                 to the viewer.
		 * @param light    The light source to use.
		 * 
		 * @return   Returns the observed surface color
		 */
		inline color_t compute_phong(
				const Eigen::Vector3f& P,
				const Eigen::Vector3f& N,
				const Eigen::Vector3f& V,
				const light_t& light) const
		{
			Eigen::Vector3f L;

			/* compute direction from light to surface */
			L = light.get_direction(P);
		
			/* get the shading */
			return this->shader.compute_phong(N,V,L,
						light.get_color_at(P));
		};

		/**
		 * Shades the element by its normal value
		 *
		 * This shader is useful for debugging
		 *
		 * @param N  The normal of the surface, in world coords
		 *
		 * @return   Returns the observed color
		 */
		inline color_t compute_normal_shading(
				const Eigen::Vector3f& N) const
		{
			return color_t(
				0.5f*(N(0) + 1.0f),
				0.5f*(N(1) + 1.0f),
				0.5f*(N(2) + 1.0f));
		};
};

#endif
