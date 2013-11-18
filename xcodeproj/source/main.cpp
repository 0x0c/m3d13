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

	auto cube = Object::cube("cube");
	cube->transform(Matrix::scale(10, 10, 10));
	
	auto polygon = new Object({
		new Polygon({
			new Vector( 1,  1, -1),
			new Vector(-1,  1, -1),
			new Vector( 1,  1,  1)
		}, 0x0000ff),
		new Polygon({
			new Vector(-1,  1, -1),
			new Vector( 1,  1,  1),
			new Vector(-1,  1,  1)
		}, 0x00ff00),
		new Polygon({
			new Vector( 1, -1, -1),
			new Vector( 1,  1, -1),
			new Vector( 1,  1,  1)
		}, 0xff0000),
		new Polygon({
			new Vector( 1, -1, -1),
			new Vector( 1, -1,  1),
			new Vector( 1,  1,  1)
		}, 0x0000ff),
		new Polygon({
			new Vector(-1, -1,  1),
			new Vector( 1, -1,  1),
			new Vector( 1,  1,  1),
		}, 0x00ff00),
		new Polygon({
			new Vector(-1, -1,  1),
			new Vector( 1,  1,  1),
			new Vector(-1,  1,  1)
		}, 0xff0000),		
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector( 1, -1, -1),
			new Vector( 1,  1, -1)
		}, 0x0000ff),
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector( 1,  1, -1),
			new Vector(-1,  1, -1)
		}, 0x00ff00),
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector(-1, -1,  1),
			new Vector(-1,  1,  1)
		}, 0xff0000),
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector(-1, -1,  1),
			new Vector(-1,  1,  1)
		}, 0x0000ff),
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector( 1, -1, -1),
			new Vector( 1, -1,  1)
		}, 0x00ff00),
		new Polygon({
			new Vector(-1, -1, -1),
			new Vector(-1, -1,  1),
			new Vector( 1, -1,  1)
		}, 0xff0000)
	}, "polygon");
	polygon->transform(Matrix::scale(10, 10, 10));
	auto view = new xm3d(1500, 1500, "xm3d", [=](unsigned int life, XEvent e, Window window) {
		cube->transform(Matrix::rotate(m3d_axis_y, 0.3));
		polygon->transform(Matrix::rotate(m3d_axis_y, 0.3));
	});
	view->add_obj(polygon);
	view->run();

	return 0;
}
