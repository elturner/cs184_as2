#ifndef LIGHT_H
#define LIGHT_H

/**
 * @file   light.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  The light_t class represents a light source
 *
 * @section DESCRIPTION
 *
 * This file contains the light_t class, which is used to represent
 * a light source.  Light sources can be either point sources or
 * directional light sources.
 */

#include <color/color.h>
#include <Eigen/Dense>
#include <cmath>
#include <float.h>

/**
 * The light_t class represents a light source
 *
 * A light source can be represented either as a point source or
 * a directional source.
 */
class light_t
{
	/* constants */
	public:

		/* the types of lights in the scene */
		enum LIGHT_TYPE
		{
			DIRECTIONAL_LIGHT,
			POINT_LIGHT_NO_FALLOFF,
			POINT_LIGHT_LINEAR_FALLOFF,
			POINT_LIGHT_QUADRATIC_FALLOFF,
			AMBIENT_LIGHT
		};

	/* parameters */
	private:

		/**
		 * The type of this light.
		 *
		 * The light's type determines how the light behaves in
		 * the scene.
		 */
		LIGHT_TYPE type;

		/**
		 * The vector of this light.
		 *
		 * If this light is a point light source,
		 * this vector represents the position of the
		 * light in 3D space.
		 *
		 * If this light is a directional light source,
		 * this vector represents that normalized direction.
		 */
		Eigen::Vector3f v;

		/**
		 * The color of this light source
		 */
		color_t color;

	/* functions */
	public:

		/*--------------*/
		/* constructors */
		/*--------------*/

		/**
		 * Default constructor
		 *
		 * This gives a directional, white light in direction
		 * <1,0,0>.
		 */
		light_t()
		{
			this->type = DIRECTIONAL_LIGHT;
			this->v << 1.0f, 0.0f, 0.0f;
			this->color.set(1.0f, 1.0f, 1.0f);
		};

		/**
		 * Constructs this light from the other light
		 *
		 * @param other  The other light to copy
		 */
		light_t(const light_t& other)
			: type(other.type),
			  v(other.v), 
			  color(other.color)
		{};

		/**
		 * Constructs light with specified parameters
		 *
		 * @param t   Specifies if light source is point, 
		 *                directional, or ambient
		 * @param x   The x-value of the vector
		 * @param y   The y-value of the vector
		 * @param z   The z-value of the vector
		 * @param r   The red-component of the color
		 * @param g   The green-component of the color
		 * @param b   The blue-component of the color
		 */
		light_t(LIGHT_TYPE t, float x, float y, float z, 
				float r, float g, float b)
			: type(t), v(x,y,z), color(r,g,b)
		{
			/* if directional, normalize */
			if(this->type == DIRECTIONAL_LIGHT)
				v.normalize();
		};

		/*-----------*/
		/* accessors */
		/*-----------*/

		/**
		 * Gets the color for this light source
		 *
		 * @return    Returns the color of this light source
		 */
		inline const color_t& get_color() const
		{ return this->color; };

		/**
		 * Gets the color of this light source at the given point
		 *
		 * @param p   The point at which to sample the color
		 *
		 * @return    Returns the color of this light at point p
		 */
		inline color_t get_color_at(const Eigen::Vector3f& p) const
		{
			float d;

			/* what we do depends on type of light source */
			switch(this->type)
			{
				default:
				case AMBIENT_LIGHT:
				case DIRECTIONAL_LIGHT:
				case POINT_LIGHT_NO_FALLOFF:
					/* color doesn't change with
					 * position, so just return it */
					return this->color;

				case POINT_LIGHT_LINEAR_FALLOFF:
					/* Light fads linearly with
					 * distance to light.  Get the
					 * distance and scale it */
					d = this->get_distance(p);
					return (this->color) * (1.0f/d);

				case POINT_LIGHT_QUADRATIC_FALLOFF:
					/* light fads quadratically with
					 * distance to light.  Get distance
					 * and scale it */
					d = this->get_distance(p);
					return (this->color) * (1.0f/(d*d));
			}

			/* we shouldn't be able to get here, but
			 * if we do, just return the original color */
			return this->color;
		};

		/**
		 * Sets the color for this light source
		 *
		 * @param red    The red component of the color
		 * @param green  The green component of the color
		 * @param blue   The blue component of the color
		 */
		inline void set_color(float red, float green, float blue)
		{ this->color.set(red, green, blue); };

		/**
		 * Sets the color for this light source
		 *
		 * @param c  The color to use
		 */
		inline void set_color(const color_t& c)
		{ this->color = c; };

		/**
		 * Returns true if this is a point light source
		 */
		inline bool is_point() const
		{ 
			switch(this->type)
			{
				case POINT_LIGHT_NO_FALLOFF:
				case POINT_LIGHT_LINEAR_FALLOFF:
				case POINT_LIGHT_QUADRATIC_FALLOFF:
					return true;
				default:
					return false;
			}
		};

		/**
		 * Retruns true iff this is an ambient light source
		 */
		inline bool is_ambient() const
		{ return this->type == AMBIENT_LIGHT; };

		/**
		 * Returns the type of this light.
		 */
		inline LIGHT_TYPE get_type() const
		{ return this->type; };

		/**
		 * Gets the value vector
		 *
		 * For a point light source, this is the position
		 *
		 * For a directional light source, this is the
		 * direction.
		 *
		 * @return Returns the vector component
		 */
		inline const Eigen::Vector3f& get_v() const
		{ return this->v; };

		/**
		 * Gets the direction of the light at the given point
		 *
		 * Given a point in 3D space, will specify the direction
		 * of the light generated by this light source.
		 *
		 * This is the direction FROM the source TO the point.
		 *
		 * @param p   The point to analyze
		 *
		 * @return    The direction of the light at p
		 */
		inline Eigen::Vector3f get_direction(
				const Eigen::Vector3f& p) const
		{
			/* check what type of light source this is */
			switch(this->type)
			{
				case POINT_LIGHT_NO_FALLOFF:
				case POINT_LIGHT_LINEAR_FALLOFF:
				case POINT_LIGHT_QUADRATIC_FALLOFF:
					/* need to compute displacement 
					 * of the point in order to 
					 * determine the direction of the 
					 * light */
					return (p - this->v).normalized();

				case AMBIENT_LIGHT:
					/* direction of light doesn't 
					 * matter */

				case DIRECTIONAL_LIGHT:
					/* this is a directional light 
					 * source, so v
					 * is the direction.  */
					return this->v;
			}
			
			/* should never get here */
			return this->v;
		};

		/**
		 * Gets the distance of the light source to the
		 * given point.
		 *
		 * Note that directional lights are assumed to be
		 * infinitely far away from everything.
		 *
		 * @param p  The point to analyze
		 *
		 * @return   The distance of the source of this light
		 *           to the specified point p
		 */
		inline float get_distance(const Eigen::Vector3f& p) const
		{
			/* get type of light */
			switch(this->type)
			{
				case POINT_LIGHT_NO_FALLOFF:
				case POINT_LIGHT_LINEAR_FALLOFF:
				case POINT_LIGHT_QUADRATIC_FALLOFF:
					/* distance to point */
					return (p - this->v).norm();

				case DIRECTIONAL_LIGHT:
					/* directional lights are
					 * infinitely far away */
					return FLT_MAX;

				case AMBIENT_LIGHT:
					/* ambient lights are all
					 * around us */
					return 0;
			}

			/* should never get here */
			return FLT_MAX;
		};

		/**
		 * Sets this light to a point light source
		 *
		 * @param val      This is the position
		 *                 of the light source in 3D space.
		 * @param falloff  The falloff type of this point light.
		 *                 (0 = no falloff, 1 = linear, 
		 *                 2 = quadratic)
		 */
		inline void set_point(const Eigen::Vector3f& val,
					int falloff=0)
		{
			/* save position of point light */
			this->v = val;

			/* save the falloff type */
			switch(falloff)
			{
				default:
					this->type = POINT_LIGHT_NO_FALLOFF;
					break;
				case 1: /* linear */
					this->type 
					= POINT_LIGHT_LINEAR_FALLOFF;
					break;
				case 2: /* quadratic */
					this->type
					= POINT_LIGHT_QUADRATIC_FALLOFF;
					break;
			}
		};

		/**
		 * Sets this light to a directional light source
		 *
		 * @param dir   The direction of this light
		 */
		inline void set_directional(const Eigen::Vector3f& dir)
		{
			this->type = DIRECTIONAL_LIGHT;
			this->v = dir;
			this->v.normalize();
		};

		/**
		 * Sets this light to an ambient light source
		 */
		inline void set_ambient()
		{
			this->type = AMBIENT_LIGHT;
			this->v << 0.0f,0.0f,0.0f;
		};
};

#endif
