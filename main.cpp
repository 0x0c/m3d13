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
	cout << "Hello, World!" << endl;

//	auto polygon = new Object({new Polygon({
//		new Vector(-1, -1,  1),
//		new Vector( 1, -1,  1),
//		new Vector( 1,  1,  1),
//	}, 0x0000ff)},"polygon");
	auto polygon = Object::fill_cube("polygon");
	polygon->transform(Matrix::scale(10, 10, 10));
	Camera camera(Vector(0, 50, 100), Vector(0, 0, 0), Vector(0, 1, 0));
	Light light(Vector(0, 50, 100), Vector(0, 0, 0), 1.0);
	auto view = new xm3d(800, 800, camera, light, "xm3d", [=](unsigned int life, XEvent e, Window window) {
		polygon->transform(Matrix::rotate(m3d_axis_y, 0.5));
	});
	view->add_object(polygon);
	view->run();

	return 0;
}
