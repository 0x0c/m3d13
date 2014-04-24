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
#include "m3d_obj_loader.h"

using namespace std;
using namespace m3d;

int main(int argc, const char * argv[])
{
	// insert code here...
	auto polygon = Object::fill_cube("cube", 0xff0000);
	polygon->transform(Matrix::scale(2, 2, 2));
	auto polygon2 = Object::fill_cube("cube", 0x00ff00);
	polygon2->transform(Matrix::move(3, 0, 0));
	polygon2->transform(Matrix::scale(2, 2, 2));
	auto polygon3 = Object::fill_cube("cube", 0x0000ff);
	polygon3->transform(Matrix::move(-3, 0, 0));
	polygon3->transform(Matrix::scale(2, 2, 2));
	Camera camera(Vector(30, 0, 0), Vector(0, 0, 0), Vector(0, 1, 0));
	Light light(Vector(5, 0, 0), Vector(0, 0, 0), 1.0);
	
	bool *move = new bool;
	*move = true;
	int *sx = new int, *sy = new int;
	auto view = new xm3d(800, 800, camera, light, "xm3d", [=](unsigned long frame, XEvent e, Window window) {});
	view->set_callback([=](unsigned long frame, XEvent e, Window window) {
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
					vector<Object>::iterator it_b = view->objects->begin();
					for (it_b = view->objects->begin(); it_b != view->objects->end(); ++it_b) {
						Object object = (Object)*it_b;
						object.transform(Matrix::rotate(m3d_axis_y, 0.15 * tx))->transform(Matrix::rotate(m3d_axis_z, 0.15 * ty));
					}
					*sx = e.xbutton.x;
					*sy = e.xbutton.y;
				}
				break;
		}
		
		if (*move) {
			vector<Object>::iterator it_b = view->objects->begin();
			for (it_b = view->objects->begin(); it_b != view->objects->end(); ++it_b) {
				Object object = (Object)*it_b;
				object.transform(Matrix::rotate(m3d_axis_y, 0.15))->transform(Matrix::rotate(m3d_axis_z, 0.15));
			}
		}
	});
	view->gui_button("STOP", 10, 20, 30, 12, [=]() {
		*move = !(*move);
	});
	
	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++) {
		int xsign = rand() % 2 == 0 ? -1 : 1;
		int ysign = rand() % 2 == 0 ? -1 : 1;
		int zsign = rand() % 2 == 0 ? -1 : 1;
		view->add_point(rand() % 800 * xsign, rand() % 800  * ysign, rand() % 800  * zsign, 0x000000);
	}
	view->add_object(*polygon);
	view->add_object(*polygon2);
	view->add_object(*polygon3);
	view->run();
	
	return 0;
}
