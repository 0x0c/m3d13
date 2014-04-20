//
//  xm3d.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "m3d.h"
#include "xm3d.h"

void xm3d::_z_sort()
{
	drawable_->clear();
	vector<Object>::iterator it_b = this->objects_->begin();
	for (it_b = this->objects_->begin(); it_b != this->objects_->end(); ++it_b) {
		Object object = (Object)*it_b;
		vector<Polygon>::iterator it_p;
		for (it_p = object.polygon.begin(); it_p != object.polygon.end(); ++it_p) {
			Polygon p = (Polygon)*it_p;
			drawable_->push_back(p);
		}
	}
	
	Vector c = Vector(camera_.eye);
	sort(drawable_->begin(), drawable_->end(), [=](Polygon a, Polygon b) {
		return a.far(b, c);
	});
}

void xm3d::_draw()
{
	if (this->draw_axis) {
		this->_draw_axis();
	}
	
	if (debug_mode) {
		XSetForeground(display_, graphic_context_, vertex_color);
		Vector v1 = Vector(light_.position);
		Vector v2 = Vector(light_.position);
		Vector v3 = Vector(light_.at);
		v1.multiply(m_);
		v2.multiply(m_);
		v3.multiply(m_);
		XPoint point[3] = {
			(XPoint){static_cast<short>(v1.x - 5), static_cast<short>(v1.y - 5)},
			(XPoint){static_cast<short>(v2.x + 5), static_cast<short>(v2.y + 5)},
			(XPoint){static_cast<short>(v3.x), static_cast<short>(v3.y)}
		};
		XFillPolygon(display_, pix_map_, graphic_context_, point, 3, Convex, CoordModeOrigin);
	}

	this->_z_sort();
	vector<Polygon>::iterator it_p;;
	for (it_p = drawable_->begin(); it_p != drawable_->end(); ++it_p) {
		Polygon p = (Polygon)*it_p;
		XSetForeground(display_, graphic_context_, line_color);
		
		int i = 0;
		if (p.front(camera_)) {
			XSetForeground(display_, graphic_context_, p.real_color(light_));
			Vector v1(*p.vertex[0]);
			v1 *= *m_;
			Vector v2(*p.vertex[1]);
			v2 *= *m_;
			Vector v3(*p.vertex[2]);
			v3 *= *m_;
			
			XPoint point[3] = {
				(XPoint){static_cast<short>(v1.x), static_cast<short>(v1.y)},
				(XPoint){static_cast<short>(v2.x), static_cast<short>(v2.y)},
				(XPoint){static_cast<short>(v3.x), static_cast<short>(v3.y)}
			};
			XFillPolygon(display_, pix_map_, graphic_context_, point, 3, Convex, CoordModeOrigin);
			
			if (debug_mode) {
				XSetForeground(display_, graphic_context_, vertex_color);
				std::ostringstream stream;
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v1.x - 5 / 2, v1.y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
				stream.str("");
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v2.x - 5 / 2, v2.y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
				stream.str("");
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v3.x - 5 / 2, v3.y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
			}
		}
	}
	
	XSetForeground(display_, graphic_context_, 0x000000);
}

void xm3d::_draw_axis()
{
	vector<Vector> axis = {
		Vector(0, 0, 0),
		Vector(1000, 0, 0),
		Vector(0, 0, 0),
		Vector(0, 1000, 0),
		Vector(0, 0, 0),
		Vector(0, 0, 1000)
	};
	
	int color = 0xff0000;
	for (int i = 0; i < 6; i+=2) {
		Vector v1 = axis[i];
		Vector v2 = axis[i + 1];
		XSetForeground(display_, graphic_context_, color);
		v1 *= *m_;
		v2 *= *m_;
		XDrawLine(display_, pix_map_, graphic_context_, v1.x, v1.y, v2.x, v2.y);
		color >>= 8;
	}
}

void xm3d::run()
{
	suspend = false;
	XSelectInput(display_ ,window_ , ExposureMask | ButtonPressMask | ButtonMotionMask | KeyPressMask);
	while(!suspend) {
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
			}
		}
		else {
			if (debug_mode) {
				cout << "--draw!(" << frame_ << ")--" << endl;
			}
			XDrawString(display_, quit_, graphic_context_, 4, 10, "QUIT", 4);
			XSetForeground(display_, graphic_context_, white_);
			XFillRectangle(display_, pix_map_, graphic_context_, 0, 0, width_, height_);
			XSetForeground(display_, graphic_context_, black_);
			this->_draw();
			XCopyArea(display_, pix_map_, window_, graphic_context_, 0, 0, width_, height_, 0, 0);
			XSetForeground(display_, graphic_context_, black_);
			std::ostringstream stream;
			stream << "m3d13 version:" << m3d::version;
			XDrawString(display_, window_, graphic_context_, 10, width_ - 10, stream.str().c_str(), (int)stream.str().length());
			
			if (fps > 0) {
				usleep(1.0 / fps * 1000000);
			}
		}
		
		if (event_callback_ != nullptr) {
			event_callback_(frame_, e_, window_);
		}
		XFlush(this->display_);
		frame_++;
	}
}

void xm3d::add_object(Object object)
{
	this->objects_->push_back(object);
}
