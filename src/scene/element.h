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
		 * Elements will delete their own shapes
		 */
		~element_t()
		{
			/* check if we need to free memory for shape */
			if(this->shape != NULL)
			{
				delete (this->shape);
				this->shape = NULL;
			}
		};

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
		inline void set(shape_t* s)
		{ this->shape = s; };

		/**
		 * Retrieves the shader for this model
		 *
		 * Will retrieve a non-constant reference to
		 * this model's shader, so it can be manipulated
		 */
		inline phong_shader_t& get_shader()
		{ return this->shader; };
		
		/**
		 * Retrieves the shader for this model
		 *
		 * Will retrieve a constant reference to
		 * this model's shader.
		 */
		const inline phong_shader_t& get_shader() const
		{ return this->shader; };
};

#endif
