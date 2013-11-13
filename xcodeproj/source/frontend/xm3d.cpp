//
//  xm3d.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include <vector>
#include "xm3d.h"

void xm3d::add_obj(Object *object)
{
	object->transform(m_);
	this->objects_->push_back(object);
}

void xm3d::draw()
{
	vector<Object *>::iterator it_b = this->objects_->begin();
	for (it_b = this->objects_->begin(); it_b != this->objects_->end(); ++it_b) {
		Object *object = (Object *)*it_b;
		cout << "node:" << object->name.c_str() << endl;
		vector<Vector *>::iterator it_v = object->vertex.begin();
		
		int i = 0;
		XSetForeground(display_, graphic_context_, 0xffff00);
		for(it_v = object->vertex.begin(); it_v != object->vertex.end(); ++it_v) {
			Vector *v = (Vector *)*it_v;
			cout << i++ << ":(" << v->x << "," << v->y << ")" << endl;
			XFillArc(display_, pix_map_, graphic_context_, v->x, v->y, 5, 5, 0, 360 * 64);
		}
		
		XSetForeground(display_, graphic_context_, 0x1e90ff);
		vector<Wire *>::iterator it_w = object->wire.begin();
		for (it_w = object->wire.begin(); it_w != object->wire.end(); ++it_w) {
			Wire *w = (Wire *)*it_w;
			XDrawLine(display_, pix_map_, graphic_context_, object->vertex[w->a]->x, object->vertex[w->a]->y, object->vertex[w->b]->x, object->vertex[w->b]->y);
		}
	}
	
	XSetForeground(display_, graphic_context_, 0x000000);
}

void xm3d::run()
{
	while(true) {
		if(XPending(display_)) {
			XNextEvent(display_, &this->e_);
			switch (e_.type) {
				case EnterNotify:
					if (e_.xany.window == quit_) {
						XSetWindowBorderWidth(display_, quit_, 2);
					}
					break;
				case LeaveNotify: {
					if (e_.xany.window == quit_) {
						XSetWindowBorderWidth(display_, quit_, 1);
					}
				}
					break;
				case ButtonRelease: {
					if(e_.xany.window == quit_) {
						exit(0);
					}
				}
					break;
				default:
					break;
			}
		}
		else {
			std::cout << "--draw!(" << life_++ << ")--" << endl;
			XDrawString(display_, quit_, graphic_context_, 4, 10, "QUIT", 4);
			XSetForeground(display_, graphic_context_, white_);
			XFillRectangle(display_, pix_map_, graphic_context_, 0, 0, width_, height_);
			XSetForeground(display_, graphic_context_, black_);
			this->draw();
			XCopyArea(display_, pix_map_, window_, graphic_context_, 0, 0, width_, height_, 0, 0);
			XSetForeground(display_, graphic_context_, black_);
			usleep(1 / fps * 1000000);
		}
		
		if (event_callback_ != nullptr) {
			event_callback_(life_, e_, window_);
		}
	}
}
