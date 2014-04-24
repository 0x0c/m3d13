//
//  xm3d.h
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>

using namespace std;
using namespace m3d;

typedef struct GUI
{
	int x;
	int y;
	int width;
	int height;
	Window w;
	string title;
	std::function<void()>callback;
} GUI;

class xm3d {
private:
	void _z_sort();
	void _object_z_sort();
	void _draw();
	void _draw_axis();

	Display *display_;
	Window window_;
	Window root_;
	GC graphic_context_;
	Pixmap pix_map_;
	XEvent e_;
	int screen_;
	unsigned long black_, white_;
	unsigned int width_;
	unsigned int height_;
	unsigned int border_ = 2;
	unsigned long frame_ = 0;
	Camera camera_;
	Light light_;
	vector<Polygon> *drawable_;
	Matrix *m_;
	std::function<void(unsigned long frame, XEvent e, Window window)> event_callback_;
public:
	xm3d(const unsigned int width, const unsigned int height, Camera camera, Light light, const std::string name, std::function<void(unsigned long frame, XEvent e, Window window)> event_callback) {
		width_ = width;
		height_ = height;
		
		display_ = XOpenDisplay("");
		screen_ = DefaultScreen(display_);
		root_ = DefaultRootWindow(display_);
		white_ = WhitePixel(display_, screen_);
		black_ = BlackPixel(display_, screen_);

		window_ = XCreateSimpleWindow(display_, DefaultRootWindow(display_), 100, 100, width_, height_, border_, black_, white_);
		graphic_context_ = XCreateGC(display_, window_, 0, NULL);
		pix_map_ = XCreatePixmap(display_, root_, width_, height_, DefaultDepth(display_, screen_));

		XStoreName(display_, window_, name.c_str());
		XSelectInput(display_, window_, ButtonPressMask | ButtonReleaseMask | KeyPressMask);
		XMapWindow(display_, window_);
		XMapSubwindows(display_, window_);

		gui = new vector<GUI>;
		objects = new vector<Object>;
		drawable_ = new vector<Polygon>;
		camera_ = camera;
		light_ = light;
		m_ = new Matrix(Matrix::identity());
		m_->view(camera_)->projection(m3d_rad(30), (double)width_ / (double)height_, 100.0, 1000.0)->screen(width_, height_);
		event_callback_ = event_callback;
	};
	~xm3d() {
		XFreePixmap(display_, pix_map_);
		XFreeGC(display_, graphic_context_);
		XDestroyWindow(display_, window_);
		XCloseDisplay(display_);
		delete objects;
	};
	vector<GUI> *gui;
	vector<Object> *objects;
	void run();
	void add_object(Object object);
	void add_point(int x, int y, int z, int color);
	void gui_button(string title, int x, int y, int width, int height, std::function<void()>callback);
	void set_callback(std::function<void(unsigned long frame, XEvent e, Window window)> callback);
	
	unsigned int fps = 60;
	bool debug_mode = false;
	bool suspend = false;
	bool draw_axis = false;
	unsigned int line_color = 0x1e90ff;
	unsigned int vertex_color = 0xff00ff;
};
