#include "parser.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>


#include <scene/camera.h>

#include <shape/shape.h>
#include <shape/triangle.h>
#include <shape/sphere.h>

#include <scene/light.h>
#include <scene/element.h>

#include <scene/phong_shader.h>

#include <geometry/transform.h>

#include <Eigen/Dense>

using namespace std;
using namespace parser;
using namespace Eigen;

parser_t::parser_t(const string& filename, scene_t& scene)
{
	/* parse the file */
	this->read(filename, scene);
}

int parser_t::read(const string& filename, scene_t& scene){
ifstream infile;
string line;
phong_shader_t shader;
transform_t transform;
stringstream ss;
infile.open(filename.c_str());

	if(!(infile.is_open())){
		/* report error */
			cerr << "[parser_t::read]\tUnable to open file for "
			     << "reading: " << filename << endl;
			return -1;
	}

	/* iterator */
	while(!(infile.eof())) {
		getline(infile, line);	

		ss.clear();
		ss.str(line);

		/* check for blank line */
		if(ss.eof())
			continue;

		string val;
		ss >> val; /* "cam", "sph", etc. */

		if(val.compare("cam") == 0) {
			/* this is a cam */
			float ex, ey, ez, llx, lly, llz, lrx, lry, lrz, ulx, uly, ulz, urx, ury, urz;
			ss >> ex >> ey >> ez >> llx >> lly >> llz >> lrx >> lry >> lrz >> ulx >> uly >> ulz >> urx >> ury >> urz; 
		
			Vector3f eye(ex, ey, ez);
			Vector3f UL(ulx, uly, ulz);
			Vector3f UR(urx, ury, urz);
			Vector3f LL(llx, lly, llz);
			Vector3f LR(lrx, lry, lrz);

			scene.get_camera().set(eye, UL, UR, LL, LR);
		}

		else if(val.compare("sph") == 0) {
			float cx, cy, cz, r;
			ss >> cx >> cy >> cz >> r; 
	
			shape_t* newSph = new sphere_t(cx, cy, cz, r); //sphere doesn't have set, use constructor?
			scene.add(newSph, transform, shader);
		} 

		else if(val.compare("tri") == 0) {
			float ax, ay, az, bx, by, bz, cx, cy, cz;
			ss >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
			
			shape_t* newSph = new triangle_t(ax, ay, az, bx, by, bz, cx, cy, cz); 
			scene.add(newSph, transform, shader);
		}



		else if(val.compare("obj") == 0) {
		
			mesh_io::mesh_t mesh;
			string objfile;
			ss >> objfile;
			if(0 != mesh.read(objfile))
			{
				cerr << "cannot read file: " << objfile << endl;
				return -1;
			}
			scene.add(mesh, transform, shader);
		}


		else if(val.compare("ltp") == 0) {
			float px, py, pz, r, g, b, falloff; //keep in brackets? where to specifiy lin/quad?
			falloff = 0;
			ss >> px >> py >> pz >> r >> g >> b >> falloff;

			light_t light;
			Eigen::Vector3f dir = Eigen::Vector3f(px,py,pz); //scene.cpp set_point etc.

			light.set_point(dir, falloff);
			light.set_color(r, g, b);
			scene.add(light);
		}
		else if(val.compare("ltd") == 0) {
			float dx, dy, dz, r, g, b;
			ss >> dx >> dy >> dz >> r >> g >> b;

			light_t light;
			Eigen::Vector3f dir = Eigen::Vector3f(dx,dy,dz);

			light.set_directional(dir);
			light.set_color(r,g,b);
			scene.add(light);
		}
		else if(val.compare("lta") == 0) {
			float r, g, b;
			ss >> r >> g >> b;

			light_t light;

			light.set_ambient();
			light.set_color(r, g, b);
			scene.add(light);
		}
		

		else if(val.compare("mat") == 0) {
			float kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb, ksp, krr, krg, krb;
			ss >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb >> ksp >> krr >> krg >> krb; 
			
			
			shader.ka.set(kar, kag, kab);
			shader.kd.set(kdr, kdg, kdb);
			shader.ks.set(ksr, ksg, ksb);
			shader.p = ksp;
			shader.kr.set(krr, krg, krb);
		}
		else if(val.compare("xft") == 0) {
			float tx, ty, tz;
			ss >> tx >> ty >> tz;
			transform.append_translation(tx, ty, tz);
		}
		else if(val.compare("xfr") == 0) {
			float rx, ry, rz;
			ss >> rx >> ry >> rz;
			transform.append_rotation(rx, ry, rz);

		}
		else if(val.compare("xfs") == 0) {
			float rx, ry, rz;
			ss >> rx >> ry >> rz;
			transform.append_scale(rx, ry, rz);
		}
		else if(val.compare("xfz") == 0) {
			transform.reset();
		}
		else {
			cerr << "unsupported feature: " << line << endl;
		}

		/* check for additional params */
		if(!ss.eof())
		{
			cerr << "ignoring extra params on line:\n\t"
				<< line << endl;
		}
	}

	infile.close();
	return 0;
}
