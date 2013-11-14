//
//  m3d.h
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#ifndef __m3d__
#define __m3d__

#include <string>
#include <vector>
#include <array>
#include <math.h>

#define m3d_rad(x) x * (M_PI / 180.0)

namespace m3d
{
	typedef enum {
		m3d_axis_x = 0,
		m3d_axis_y,
		m3d_axis_z
	} m3d_axis;
	
	class Camera;
	class Matrix
	{
	public:
		Matrix(const std::array<double, 16> m) {
			this->m = m;
		};
		Matrix* multiply(const Matrix *m);
		Matrix* create_matrix_multiply(const Matrix *m);
		void view(const Camera *camera);
		void projection(const double angle, const double aspect, const double near, const double far);
		void screen(const double x, const double y);
		static std::array<double, 16> rotate(const m3d_axis axis, const double angle) {
			std::array<double, 16> r;
			double rad = m3d_rad(angle);
			switch (axis) {
				case m3d_axis_x: {
					r = {
						1			,0			,0			,0,
						0			,cos(rad)	,sin(rad)	,0,
						0			,-sin(rad)	,cos(rad)	,0,
						0			,0			,0			,1
					};
				}
					break;
				case m3d_axis_y: {
					r = {
						cos(rad)	,0			,-sin(rad),0,
						0			,1			,0			,0,
						sin(rad)	,0			,cos(rad)	,0,
						0			,0			,0			,1
					};
				}
					break;
				case m3d_axis_z: {
					r = {
						cos(rad)	,sin(rad)	,0			,0,
						-sin(rad)	,cos(rad)	,0			,0,
						0			,0			,1			,0,
						0			,0			,0			,1
					};
				}
					break;
			}
			
			return r;
		};
		static std::array<double, 16> move(double x, double y, double z) {
			return {
				1		,0		,0		,0,
				0		,1		,0		,0,
				0		,0		,1		,0,
				x		,y		,z		,1
			};
		};
		static std::array<double, 16> scale(double x, double y, double z) {
			return {
				x		,0		,0		,0,
				0		,y		,0		,0,
				0		,0		,z		,0,
				0		,0		,0		,1
			};
		};
		static std::array<double, 16> identity() {
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		};
		
		/* data */
		std::array<double, 16> m;
	};
	
	class Vector
	{
	public:
		Vector(const double x, const double y, const double z) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 1;
		};
		void add(const Vector *v);
		void sub(const Vector *v);
		double dot(const Vector *v);
		void cross(const Vector *v);
		void normalize();
		double size();
		void scaleTo(const double k);
		void multiply(const Matrix *m);
		Vector* create_vector_add(const Vector *v);
		Vector* create_vector_sub(const Vector *v);
		Vector* create_vector_cross(const Vector *v);
		Vector* normalized_vector();
		Vector* scaled_vector(const double k);

		/* data */
		double x;
		double y;
		double z;
		double w;
	};

	class Wire
	{
	public:
		Wire(int a, int b){
			this->a = a;
			this->b = b;
		};

		/* data */
		int a;
		int b;
	};

	class Object
	{
	public:
		Object(const std::vector<Vector *>v, const std::vector<Wire *>w, const std::string n) {
			vertex = v;
			wire = w;
			name = n;
		};
		Object* transform(const Matrix *m);
		Object* transform(const std::array<double, 16> m);
		static Object* cube(std::string name) {
			Object *cube = new Object({
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
			}, name);
			
			return cube;
		};
		static Object* pyramid3(std::string name) {
			Object *pyramid3 = new Object({
				new Vector(-1, -1, -1),
				new Vector(-1,  1,  1),
				new Vector( 1,  1, -1),
				new Vector( 1, -1,  1),
			}, {
				new Wire(0, 1),
				new Wire(1, 2),
				new Wire(2, 0),
				new Wire(0, 3),
				new Wire(1, 3),
				new Wire(2, 3),
			}, name);
			
			return pyramid3;
		};

		/* data */
		std::string name;
		std::vector<Vector *>vertex;
		std::vector<Wire *>wire;
	};
	
	class Camera
	{
	public:
		Camera(Vector *e, Vector *a, Vector *u) {
			this->eye = e;
			this->at = a;
			this->up = u;
		};
		~Camera() {
			delete eye;
			delete at;
			delete up;
		};
		void lookup(const Vector target);
		void move(const Vector to);
		
		/* data */
		Vector *eye;
		Vector *at;
		Vector *up;
	};
}

#endif
