//
//  main.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include <iostream>
#include <array>
#include "m3d.h"
#include "xm3d.h"

using namespace std;
using namespace m3d;

int main(int argc, const char * argv[])
{
	// insert code here...
	auto polygon = Object::fill_cube("cube", 0xff0000);
	polygon->transform(Matrix::scale(2, 2, 2));
	auto q = Quaternion(Vector(1, 1, 1), 45);
	auto m = q.matrix();
	polygon->transform(&m);
	Camera camera(Vector(20, 0, 0), Vector(0, 0, 0), Vector(0, 1, 0));
	Light light(Vector(5, 0, 0), Vector(0, 0, 0), 1.0);
	int *sx = new int, *sy = new int;
	auto view = new xm3d(800, 800, camera, light, "xm3d", [=](unsigned long frame, XEvent e, Window window) {
		switch (e.type) {
			case ButtonPress:
				if(e.xany.window == window) {
					*sx = e.xbutton.x;
					*sy = e.xbutton.y;
				}
				break;
			case MotionNotify:
				if(e.xany.window == window) {
					int tx = *sx - e.xbutton.x;
					int ty = *sy - e.xbutton.y;
					polygon->transform(Matrix::rotate(m3d_axis_y, 0.15 * tx))->transform(Matrix::rotate(m3d_axis_z, 0.15 * ty));
					*sx = e.xbutton.x;
					*sy = e.xbutton.y;
				}
				break;
		}
	});
	
	view->add_object(*polygon);
	view->run();
	
	return 0;
	
	return 0;
}
