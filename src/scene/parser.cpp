#include "parser.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>


#include <scene/camera.h>
#include <scene/camera.cpp>

#include <shape/shape.h>
#include <shape/triangle.h>


#include <scene/light.h>
#include <scene/element.h>

#include <scene/phong_shader.cpp>
#include <scene/phong_shader.h>

#include <geometry/tranform.cpp>
#include <geometry/tranform.h>


using namespace std;
using namespace parser;

parser::parser(const string& filename)
{
	/* parse the file */
	this->read(filename);
}

int parser::read(const string& filename){
ifstream infile;
string line;
stringstream ss;
infile.open("myFile.txt"); //	infile.open(filename.c_str());

	if(!(infile.is_open())){
		/* report error */
			cerr << "[parser::read]\tUnable to open file for "
			     << "reading: " << filename << endl;
			return -1;
	}

	/* iterator */
	while(!(infile.eof())) {
		getline(infile, line);	

		ss.str(line);
		
		string val;
		ss >> val; /* "cam", "sph", etc. */

		if(val.compare("cam") == 0) {
			/* this is a cam */
			float ex, ey, ez, llx, lly, llz, lrx, lry, lrz, ulx, uly, ulz, urx, ury, urz;
			ss >> ex >> ey >> ez >> llx >> lly >> llz >> lrx >> lry >> lrz >> ulx >> uly >> ulz >> urx >> ury >> urz; 
		
			scene.eye = (ex, ey, ez);
			scene.UL = (ulx, uly, ulz);
			scene.UR = (urx, ury, urz);
			scene.LL = (llx, lly, llz);
			scene.LR = (lrx, lry, lrz);

			scene.get_camera().set(scene.eye, scene.UL, scene.UR, scene.LL, scene.LR);
		}

		else if(val.compare("sph") == 0) {
			float cx, cy, cz, r;
			ss >> cx >> cy >> cz >> r; 
	
			scene.shape_t newSph = scene.sphere_t(cx, cy, cz, r); //sphere doesn't have set, use constructor?
			scene.get_shape().set_shape(newSph); 
			//shape.get_shape().set(r);
		} 

		else if(val.compare("tri") == 0) {
			float ax, ay, az, bx, by, bz, cx, cy, cz;
			ss >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
			
			scene.get_shape().set(ax, ay, az, bx, by, bz, cx, cy, cz); 
		}



		else if(val.compare("obj") == 0) {
		/////
		//// how to work with this? ////
		/////
		}


		else if(val.compare("ltp") == 0) {
			float px, py, pz, r, g, b, falloff; //keep in brackets? where to specifiy lin/quad?
			ss >> px >> py >> pz >> r >> g >> b >> falloff;

			scene.light_t light;
			Eigen::Vector3f dir = Eigen::Vector3f(px,py,pz); //scene.cpp set_point etc.

			light.set_point(dir, falloff)
			light.set_color(r, g, b);
		}
		else if(val.compare("ltd") == 0) {
			float dx, dy, dz, r, g, b;
			ss >> dx >> dy >> dz >> r >> g >> b;

			scene.light_t light;
			Eigen::Vector3f dir = Eigen::Vector3f(dx,dy,dz);

			light.set_directional(dir);
			light.set_color(r,g,b);
		}
		else if(val.compare("lta") == 0) {
			float r, g, b;
			ss >> r >> g >> b;

			scene.light_t light;

			light.set_ambient();
			light.set_color(r, g, b);
		}
		

		else if(val.compare("mat") == 0) {
			float kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb, ksp, krr, krg, krb;
			ss >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb >> ksp >> krr >> krg >> krb; 
			
			scene.phong_shader_t shader;
			
			shader.ka.set(kar, kag, kab);
			shader.kd.set(kdr, kdg, kdb);
			shader.ks.set(ksr, ksg, ksb, ksp);
			shader.p.set(ksp);
			shader.kr.set(krr, krg, krb);

			scene.set_shader(shader);

		}
		else if(val.compare("xft") == 0) {
			float tx, ty, tz;
			ss >> tx >> ty >> tz;
			geometry.append_translation(tx, ty, tz);
		}
		else if(val.compare("xfr") == 0) {
			float rx, ry, rz;
			ss >> rx >> ry >> rz;
			geometry.append_rotation(rx, ry, rz);

		}
		else if(val.compare("xfs") == 0) {
			float rx, ry, rz;
			ss >> rx >> ry >> rz;
			geometry.append_scale(rx, ry, rz);
		}
		else if(val.compare("xfs") == 0) {
			geometry.reset();
		}
		else {
			cerr << "unsupported feature"
			return -1;
		}
	}

	infile.close();
	return 0;
}
