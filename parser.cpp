#include <fstream>
#include <string>
#include <sstream>
#include <scene/camera.h>
#include <scene/camera.cpp>
#include <shape/shape.h>

std::ifstream infile;
infile.open("myFile.txt");

for (std::string line; getline(infile, line);) {		
	stringstream ss;
	ss.str(line);
	
	string val;
	ss >> val; /* "cam", "sph", etc. */

	if(val.compare("cam") == 0)
	{
		/* this is a cam */
		float ex, ey, ez, llx, lly, llz, lrx, lry, lrz, ulx, uly, ulz, urx, ury, urz;
		ss >> ex >> ey >> ez >> llx >> lly >> llz >> lrx >> lry >> lrz >> ulx >> uly >> ulz >> urx >> ury >> urz; 
	
		scene.eye = (ex, ey, ez);
		scene.UL = (ulx, uly, ulz);
		scene.UR = (urx, ury, urz);
		scene.LL = (llx, lly, llz);
		scene.LR = (lrx, lry, lrz);

		scene.set(scene.eye,  //camera.h
			scene.UL,
			scene.UR,
			scene.LL,
			scene.LR);
	}
	else if(val.compare("sph") == 0) {
		float cx, cy, cz, r;
		ss >> cx >> cy >> cz >> r; 

		shape.sphere_t(cx, cy, cz, r); //constructor from sphere.h
		shape.set_radius(r); //do we need this extra call?
	}
	else if(val.compare("tri") == 0) {
		float ax, ay, az, bx, by, bz, cx, cy, cz;
		ss >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;

		shape.trangle_t(ax, ay, az, bx, by, bz, cx, cy, cz); 
	}
	else if(val.compare("obj") == 0) {
	/////
	//// how to work with this? ////
	/////
	}
	else if(val.compare("ltp") == 0) {
		//////
		float px, py, pz, r, g, b, [falloff]; //keep in brackets? where to specifiy lin/quad?
		//////
		ss >> px >> py >> pz >> r >> g >> b >> [falloff];

		scene.light_t(POINT_LIGHT, px, py, pz, r, g, b);
	}
	else if(val.compare("ltd") == 0) {
		float dx dy dz r g b;
		ss >> dx >> dy >> dz >> r >> g >> b;

		scene.light_t(DIRECTIONAL_LIGHT, dx, dy, dz, r, g, b);
	}
	else if(val.compare("lta") == 0) {
		float r, g, b;
		ss >> r >> g >> b;

		scene.light_amb(AMBIENT_LIGHT, r, g, b); //create ambient light
	}
	else if(val.compare("mat") == 0) {
		float kar kag kab kdr kdg kdb ksr ksg ksb ksp krr krg krb;
		ss >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb >> ksp >> krr >> krg >> krb; 
		phong_shader_t shader;
		scene.shader.ka.set(kar, kag, kab); //phong_shader.cpp
		scene.shader.kd.set(kdr, kdg, kdb);
		scene.shader.ks.set(ksr, ksg, ksb, ksp);
		scene.shader.p.set(ksp);
		scene.shader.kr.set(krr, krg, krb);
	}
	else if(val.compare("xft") == 0) {
		float tx ty tz;
		ss >> tx >> ty >> tz;
		geometry.set_translation(tx, ty, tz);
	}
	else if(val.compare("xfr") == 0) {
		float rx ry rz;
		ss >> rx >> ry >> rz;
		geometry.set_rotation(rx, ry, rz);

	}
	else if(val.compare("xfs") == 0) {
		float rx ry rz;
		ss >> rx >> ry >> rz;
		geometry.set_scalre(rx, ry, rz);
	}

}

infile.close();
