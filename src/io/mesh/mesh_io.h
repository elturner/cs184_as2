#ifndef MESH_IO_H
#define MESH_IO_H

/**
 * @file   mesh_io.h
 * @author Eric Turner <elturner@eecs.berkeley.edu>
 * @brief  This class can read in various mesh file formats
 *
 * @section DESCRIPTION
 *
 * This file contains classes used to import and
 * export vertex and triangle information from common mesh
 * file formats.
 */

#include <iostream>
#include <string>
#include <vector>

/**
 * The mesh_io namespace represents all classes used for
 * mesh i/o operations.
 *
 * It is used to import/export vertex and polygon information
 * from a variety of common file formats.
 */
namespace mesh_io
{
	/* the following classes are defined in this namespace */
	class mesh_t;
	class vertex_t;
	class polygon_t;
	
	/**
	 * The reader_t class is used to import mesh information
	 * from disk.
	 */
	class mesh_t
	{
		/* parameters */
		private:

			/**
			 * The list of vertices of this mesh
			 */
			std::vector<vertex_t> vertices;
			
			/**
			 * The list of triangles of this mesh
			 */
			std::vector<polygon_t> polygons;

		/* functions */
		public:

			/*--------------*/
			/* constructors */
			/*--------------*/

			/**
			 * Constructs default (empty) mesh
			 */
			mesh_t() {};

			/**
			 * Constructs mesh from given other mesh
			 *
			 * @param other   The other mesh to copy
			 */
			mesh_t(const mesh_t& other)
				: vertices(other.vertices), 
				  polygons(other.polygons)
			{};

			/**
			 * Constructs mesh by parsing the given file
			 *
			 * Given a path to a file on disk,
			 * will initialize this mesh object by parsing
			 * the file.  The format of the file is
			 * deduced automatically.
			 *
			 * @param filename   The file to parse
			 */
			mesh_t(const std::string& filename);

			/*-----*/
			/* i/o */
			/*-----*/

			/**
			 * Parses this mesh from the specified file
			 *
			 * Will determine the format of the given file,
			 * and will import data from file.
			 *
			 * @param filename   The file to parse
			 *
			 * @return   Returns zero on success, 
			 *           non-zero on failure.
			 */
			int read(const std::string& filename);

			/*-----------*/
			/* accessors */
			/*-----------*/

			/**
			 * Retrieves the number of vertices in this 
			 * structure.
			 *
			 * @return   Returns number of vertices
			 */
			inline size_t num_verts() const
			{ return this->vertices.size(); };

			/**
			 * Retrieves the i'th vertex.
			 *
			 * Will retrieve the vertex at index i
			 *
			 * @param i   The index to check
			 *
			 * @return    Returns a reference to this vertex
			 */
			inline vertex_t& get_vert(size_t i)
			{ return this->vertices[i]; };

			/**
			 * Retrieves a constant reference to the i'th
			 * vertex.
			 *
			 * Will retrieve the vertex at index i.
			 *
			 * @param i   The index to check
			 *
			 * @return    Returns a const reference to vertex
			 */
			inline const vertex_t& get_vert(size_t i) const
			{ return this->vertices[i]; };

			/**
			 * Retrieves the number of polygons in this
			 * structure.
			 *
			 * @return   Returns the number of polygons
			 */
			inline size_t num_polys() const
			{ return this->polygons.size(); };

			/**
			 * Retrieves the i'th polygon.
			 *
			 * Will retrieve the polygon at index i
			 *
			 * @param i   The index to check
			 *
			 * @return    Returns a reference to this polygon
			 */
			inline polygon_t& get_poly(size_t i)
			{ return this->polygons[i]; };
			
			/**
			 * Retrieves a const reference to the i'th polygon.
			 *
			 * Will retrieve the polygon at index i
			 *
			 * @param i   The index to check
			 *
			 * @return    Returns a const reference to polygon
			 */
			inline const polygon_t& get_poly(size_t i) const
			{ return this->polygons[i]; };

			/**
			 * Sets this mesh to the specified vertices and
			 * polygons.
			 *
			 * @param vs   The vertices to set to
			 * @param ps   The polygons to set to
			 */
			inline void set(const std::vector<vertex_t>& vs,
			                const std::vector<polygon_t>& ps)
			{
				this->vertices.clear();
				this->vertices.insert(
					this->vertices.begin(), 
					vs.begin(), vs.end());
				this->polygons.clear();
				this->polygons.insert(
					this->polygons.begin(), 
					ps.begin(), ps.end());
			};

			/*-----------*/
			/* modifiers */
			/*-----------*/

			/**
			 * Clears all information from this mesh
			 *
			 * Will remove all vertex and polygon information
			 * from this structure
			 */
			void clear();

			/**
			 * Adds a vertex to this structure
			 *
			 * Adds the given vertex to this structure,
			 * at the end, increasing the number of vertices
			 * in the mesh by one.
			 *
			 * @param v   The vertex to add
			 */
			inline void add(const vertex_t& v)
			{ this->vertices.push_back(v); };

			/**
			 * Adds a polygon to this structure
			 *
			 * Adds the given polygon to this structure,
			 * at the end, increasing the number of
			 * polygons by one.
			 *
			 * @param p   The polygon to add
			 */
			inline void add(const polygon_t& p)
			{ this->polygons.push_back(p); };
	};

	/**
	 * The vertex_t class represents a single vertex in the mesh
	 *
	 * A vertex is represented by a point in 3D.  It may optionally
	 * have color information.
	 */
	class vertex_t
	{
		/* parameters */
		public:

			/**
			 * The positional coordinates of the vertex
			 */
			double x, y, z;

			/**
			 * The vertex may optionally have color
			 */
			int red, green, blue;
		
		/* functions */
		public:

			/*--------------*/
			/* constructors */
			/*--------------*/

			/**
			 * Initializes default vertex
			 */
			vertex_t() 
				: x(0), y(0), z(0), 
				  red(255), green(255), blue(255)
			{};

			/**
			 * Initializes vertex given another vertex
			 *
			 * @param other   The other vertex to copy
			 */
			vertex_t(const vertex_t& other)
				: x(other.x), y(other.y), z(other.z),
				  red(other.red), green(other.green),
				  blue(other.blue)
			{};

			/**
			 * Initializes vertex from coordinates
			 *
			 * @param xx  The x-coordinate of vertex
			 * @param yy  The y-coordinate of vertex
			 * @param zz  The z-coordinate of vertex
			 */
			vertex_t(double xx, double yy, double zz)
				: x(xx), y(yy), z(zz),
				  red(0), green(0), blue(0)
			{};

			/*-----------*/
			/* modifiers */
			/*-----------*/

			/**
			 * Sets the position of this vertex
			 *
			 * @param xx  The x-coordinate of vertex
			 * @param yy  The y-coordinate of vertex
			 * @param zz  The z-coordinate of vertex
			 */
			inline void set_pos(double xx, double yy, double zz)
			{
				this->x = xx;
				this->y = yy;
				this->z = zz;
			};

			/**
			 * Sets the color of this vertex
			 *
			 * @param r  The red component of color
			 * @param g  The green component of color
			 * @param b  The blue component of color
			 */
			inline void set_color(int r, int g, int b)
			{
				this->red = r;
				this->green = g;
				this->blue = b;
			};
	};

	/**
	 * The polygon_t class represents a single triangle in the mesh.
	 *
	 * Note that polygons are defined by referencing N vertex
	 * points, so each of the values in this polygon structure are
	 * indices into the list of vertices.
	 */
	class polygon_t
	{
		/* parameters */
		public:
			
			/**
			 * The indices of the vertices that make
			 * up this polygon
			 */
			std::vector<size_t> vertices;

		/* functions */
		public:

			/*--------------*/
			/* constructors */
			/*--------------*/

			/**
			 * Constructs a default (empty) polygon
			 */
			polygon_t() {};

			/**
			 * Contructs a triangle from the given vertices
			 *
			 * @param i  The first vertex index
			 * @param j  The second vertex index
			 * @param k  The third vertex index
			 */
			polygon_t(size_t i, size_t j, size_t k);

			/**
			 * Constructs a general polygon from the given list
			 *
			 * @param vs   The list of vertices to represent
			 *             this polygon
			 */
			polygon_t(const std::vector<size_t>& inds)
				: vertices(inds)
			{};

			/**
			 * Contructs the triangle from the other triangle
			 * given.
			 *
			 * @param other     The other triangle to copy to
			 *                  this one
			 */
			polygon_t(const polygon_t& other)
				:	vertices(other.vertices)
			{};

			/*-----------*/
			/* modifiers */
			/*-----------*/

			/**
			 * Clears all vertex indices from this polygon
			 */
			inline void clear()
			{ this->vertices.clear(); };

			/**
			 * Sets this triangle to a particular value
			 *
			 * @param i  The first vertex index
			 * @param j  The second vertex index
			 * @param k  The third vertex index
			 */
			inline void set(size_t i, size_t j, size_t k)
			{
				this->vertices.clear();
				this->vertices.push_back(i);
				this->vertices.push_back(j);
				this->vertices.push_back(k);
			};

			/**
			 * Sets this polygon to the list of vertices
			 *
			 * @param vs   The vertices to use for this polygon
			 */
			inline void set(const std::vector<size_t>& vs)
			{
				this->vertices.clear();
				this->vertices.insert(
					this->vertices.begin(),
					vs.begin(), vs.end());
			};
	};
}

#endif
