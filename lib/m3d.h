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

#define m3d_rad	(M_PI / 180.0)

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
		Matrix(std::array<double, 16> m) {
			this->m = m;
		};
		void multiply(const Matrix *m);
		Matrix *create_matrix_multiply(const Matrix *m);
		void world_matrix(const Matrix *scale, const Matrix *rotate, const Matrix *move);
		void view(const Camera *camera);
		void projection(double angle, double aspect, double near, double far);
		void screen(double x, double y);
		static std::array<double, 16> rotate(m3d_axis axis, double angle) {
			std::array<double, 16> r;
			switch (axis) {
				case m3d_axis_x: {
					r = {
						1			,0			,0			,0,
						0			,cos(angle)	,sin(angle)	,0,
						0			,-sin(angle),cos(angle)	,0,
						0			,0			,0			,1
					};
				}
					break;
				case m3d_axis_y: {
					r = {
						cos(angle)	,0			,-sin(angle),0,
						0			,1			,0			,0,
						sin(angle)	,0			,cos(angle)	,0,
						0			,0			,0			,1
					};
				}
					break;
				case m3d_axis_z: {
					r = {
						cos(angle)	,sin(angle)	,0			,0,
						-sin(angle)	,cos(angle)	,0			,0,
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
		Vector *create_vector_add(const Vector *v);
		Vector *create_vector_sub(const Vector *v);
		Vector *create_vector_cross(const Vector *v);
		Vector* normalized_vector();
		Vector* scale(const double k);

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
		Object(std::vector<Vector *>v, std::vector<Wire *>w, std::string n) {
			vertex = v;
			wire = w;
			name = n;
		};
		void transform(Matrix *m);
		
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
		void lookup(Vector target);
		void move(Vector to);
		
		/* data */
		Vector *eye;
		Vector *at;
		Vector *up;
	};
}

#endif
