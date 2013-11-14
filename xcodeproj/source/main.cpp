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

	auto *cube = Object::cube("cube");
	
	cube->transform(Matrix::scale(10, 10, 10));
	auto view = new xm3d(800, 800, "cube", [cube](unsigned int life, XEvent e, Window window) {
		cube->transform(Matrix::rotate(m3d_axis_y, 0.01));
	});
	view->add_obj(cube);
	view->run();
	
	return 0;
}
