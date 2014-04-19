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
//	polygon->transform(Matrix::move(1, 1, 1));
	Camera camera(Vector(10, 10, 10), Vector(0, 0, 0), Vector(0, 1, 0));
	Light light(Vector(3, 4, 5), Vector(0, 0, 0), 1.0);
	auto view = new xm3d(800, 800, camera, light, "xm3d", [=](unsigned long frame, XEvent e, Window window) {
//		polygon->transform(Matrix::rotate(m3d_axis_x, 1.0))->transform(Matrix::rotate(m3d_axis_y, .50))->transform(Matrix::rotate(m3d_axis_z, 1.0));
		polygon->transform(Matrix::rotate(m3d_axis_y, 1));
		
//		Matrix m = Quaternion(Vector(0, 1, 0), 1).matrix();
//		polygon->transform(&m);
	});
	view->add_object(*polygon);
	view->run();
	
	return 0;
	
	return 0;
}
