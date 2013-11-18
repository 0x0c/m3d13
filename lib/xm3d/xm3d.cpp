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
#include "m3d.h"
#include "xm3d.h"

vector<Polygon *> xm3d::_z_sort()
{
	map<double, Polygon *> order;
	vector<Polygon *> result;
	
	vector<Object *>::iterator it_b = this->objects_->begin();
	for (it_b = this->objects_->begin(); it_b != this->objects_->end(); ++it_b) {
		Object *object = (Object *)*it_b;
		vector<Polygon *>::iterator it_p = object->polygon.begin();
		for (it_p = object->polygon.begin(); it_p != object->polygon.end(); ++it_p) {
			Polygon *p = (Polygon *)*it_p;
			double position = Vector(
				(Vector(*p->vertex[0]).multiply(m_)->x + Vector(*p->vertex[1]).multiply(m_)->x + Vector(*p->vertex[2]).multiply(m_)->x) / 3,
				(Vector(*p->vertex[0]).multiply(m_)->y + Vector(*p->vertex[1]).multiply(m_)->y + Vector(*p->vertex[2]).multiply(m_)->y) / 3,
				(Vector(*p->vertex[0]).multiply(m_)->z + Vector(*p->vertex[1]).multiply(m_)->z + Vector(*p->vertex[2]).multiply(m_)->z) / 3
			).size();
			order[position] = p;
		}
	}
	
	map<double, Polygon *>::iterator it_m = order.begin();
	for (it_m = order.begin(); it_m != order.end(); ++it_m) {
		result.push_back((*it_m).second);
	}
	
	reverse(result.begin(), result.end());
	
	return result;
}

void xm3d::_draw()
{
	if (this->draw_axis) {
		this->_draw_axis();
	}
	
	vector<Object *>::iterator it_b = this->objects_->begin();
	for (it_b = this->objects_->begin(); it_b != this->objects_->end(); ++it_b) {
		Object *object = (Object *)*it_b;
		int i = 0;
		if (debug_mode) {
			cout << "node:" << object->name.c_str() << endl;
			vector<Vector *>::iterator it_v = object->vertex.begin();
			XSetForeground(display_, graphic_context_, vertex_color);
			for(it_v = object->vertex.begin(); it_v != object->vertex.end(); ++it_v) {
				Vector v = Vector(*(Vector *)*it_v);
				v.multiply(m_);
				XFillArc(display_, pix_map_, graphic_context_, v.x - 5 / 2, v.y - 5 / 2, 5, 5, 0, 360 * 64);
				
				if (debug_mode) {
					std::ostringstream stream;
					stream << i;
					XDrawString(display_, pix_map_, graphic_context_, v.x - 5 / 2, v.y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
					cout << i++ << ":(" << v.x << "," << v.y << ")" << endl;
				}
			}
		}
		
		XSetForeground(display_, graphic_context_, line_color);
		vector<Wire *>::iterator it_w = object->wire.begin();
		for (it_w = object->wire.begin(); it_w != object->wire.end(); ++it_w) {
			Wire *w = (Wire *)*it_w;
			Vector *v1 = Vector(*object->vertex[w->a]).multiply(m_);
			Vector *v2 = Vector(*object->vertex[w->b]).multiply(m_);

			XDrawLine(display_, pix_map_, graphic_context_, v1->x, v1->y, v2->x, v2->y);
		}

		i = 0;
		vector<Polygon *>::iterator it_p = this->_z_sort().begin();
		for (it_p = object->polygon.begin(); it_p != object->polygon.end(); ++it_p) {
			Polygon *p = (Polygon *)*it_p;
			XSetForeground(display_, graphic_context_, p->color);
			
			Vector *v1 = Vector(*p->vertex[0]).multiply(m_);
			Vector *v2 = Vector(*p->vertex[1]).multiply(m_);
			Vector *v3 = Vector(*p->vertex[2]).multiply(m_);
			
			XPoint point[3] = {
				(XPoint){static_cast<short>(v1->x), static_cast<short>(v1->y)},
				(XPoint){static_cast<short>(v2->x), static_cast<short>(v2->y)},
				(XPoint){static_cast<short>(v3->x), static_cast<short>(v3->y)}
			};
			XFillPolygon(display_, pix_map_, graphic_context_, point, 3, Convex, CoordModeOrigin);
			if (debug_mode) {
				XSetForeground(display_, graphic_context_, vertex_color);
				std::ostringstream stream;
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v1->x - 5 / 2, v1->y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
				stream.str("");
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v2->x - 5 / 2, v2->y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
				stream.str("");
				stream << i++;
				XDrawString(display_, pix_map_, graphic_context_, v3->x - 5 / 2, v3->y - 5 / 2, stream.str().c_str(), (int)stream.str().length());
			}
		}
	}
	
	XSetForeground(display_, graphic_context_, 0x000000);
}

void xm3d::_draw_axis()
{
	vector<Object *>axis = {
		new Object({
			new Vector(100, 0, 0),
			new Vector(-100, 0, 0)
		}, {
			new Wire(0, 1)
		}, "x_axis"),
		new Object({
			new Vector(0, 100, 0),
			new Vector(0, -100, 0)
		}, {
			new Wire(0, 1)
		}, "y_axis"),
		new Object({
			new Vector(0, 0, 100),
			new Vector(0, 0, -100)
		}, {
			new Wire(0, 1)
		}, "z_axis")
	};
	
	int color = 0xff0000;
	for (int i = 0; i < 3; i++) {
		Object *object = axis[i];
		XSetForeground(display_, graphic_context_, color);
		vector<Wire *>::iterator it_w = object->wire.begin();
		for (it_w = object->wire.begin(); it_w != object->wire.end(); ++it_w) {
			Wire *w = (Wire *)*it_w;
			Vector *v1 = Vector(*object->vertex[w->a]).multiply(m_);
			Vector *v2 = Vector(*object->vertex[w->b]).multiply(m_);
			XDrawLine(display_, pix_map_, graphic_context_, v1->x, v1->y, v2->x, v2->y);
		}
		color >>= 8;
	}
}

void xm3d::run()
{
	suspend = false;
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
				default:
					break;
			}
		}
		else {
#ifdef __M3D__DEBUG__
			cout << "--draw!(" << life_ << ")--" << endl;
#endif
			XDrawString(display_, quit_, graphic_context_, 4, 10, "QUIT", 4);
			XSetForeground(display_, graphic_context_, white_);
			XFillRectangle(display_, pix_map_, graphic_context_, 0, 0, width_, height_);
			XSetForeground(display_, graphic_context_, black_);
			this->_draw();
			XCopyArea(display_, pix_map_, window_, graphic_context_, 0, 0, width_, height_, 0, 0);
			XSetForeground(display_, graphic_context_, black_);
			std::ostringstream stream;
			stream << "m3d version:" << m3d::version;
			XDrawString(display_, window_, graphic_context_, 10, width_ - 10, stream.str().c_str(), (int)stream.str().length());
			usleep(1.0 / fps * 1000000);
		}
		
		if (event_callback_ != nullptr) {
			event_callback_(life_, e_, window_);
		}
		XFlush(this->display_);
		life_++;
	}
}

void xm3d::add_obj(Object *object)
{
	this->objects_->push_back(object);
}
