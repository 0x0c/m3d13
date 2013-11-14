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

	auto *cube = new Object({
		new Vector(-1, -1, -1),
		new Vector( 1, -1, -1),
		new Vector( 1,  1, -1),
		new Vector(-1,  1, -1),
		new Vector(-1, -1,  1),
		new Vector( 1, -1,  1),
		new Vector( 1,  1,  1),
		new Vector(-1,  1,  1)
	}, {
		new Wire(0, 1),
		new Wire(1, 2),
		new Wire(2, 3),
		new Wire(3, 0),
		new Wire(4, 5),
		new Wire(5, 6),
		new Wire(6, 7),
		new Wire(7, 4),
		new Wire(0, 4),
		new Wire(1, 5),
		new Wire(2, 6),
		new Wire(3, 7),
	}, "cube");

	cube->transform(Matrix::scale(10, 10, 10))->transform(Matrix::rotate(m3d_axis_x, 10));
	
	auto *view = new xm3d(800, 800, "cube", [cube](unsigned int life, XEvent e, Window window){
		cube->transform(Matrix::rotate(m3d_axis_y, 0.0001));
	});
	view->add_obj(cube);
	view->run();
	
    return 0;
}

