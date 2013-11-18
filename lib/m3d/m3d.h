//
//  m3d.h
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <array>
#include <math.h>

#define m3d_rad(x) x * (M_PI / 180.0)

namespace m3d
{
	static std::string version = "1.0";
	
	typedef enum {
		m3d_axis_x = 1 << 0,
		m3d_axis_y = 1 << 1,
		m3d_axis_z = 1 << 2
	} m3d_axis;
	
	class Camera;
	class Matrix
	{
	public:
		Matrix(const std::array<double, 16> m) {
			this->m = m;
		};
		Matrix operator*(const Matrix& m) {
			Matrix tmp(*this);
			tmp._multiply(&m);
			
			return tmp;
		};
		Matrix& operator*=(const Matrix& m) {
			this->_multiply(&m);
			
			return *this;
		};
		Matrix* view(const Camera *camera);
		Matrix* projection(const double angle, const double aspect, const double near, const double far);
		Matrix* screen(const double x, const double y);
		
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
	private:
		Matrix* _multiply(const Matrix *m);
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
		Vector operator+(const Vector& v) {
			Vector tmp(*this);
			tmp.x += v.x;
			tmp.y += v.y;
			tmp.z += v.z;
			
			return tmp;
		};
		Vector& operator+=(const Vector& v) {
			x += v.x;
			y += v.x;
			z += v.x;
			
			return *this;
		};
		Vector operator-(const Vector& v) {
			Vector tmp(*this);
			tmp.x -= v.x;
			tmp.y -= v.y;
			tmp.z -= v.z;
			
			return tmp;
		};
		Vector& operator-=(const Vector& v) {
			x -= v.x;
			y -= v.x;
			z -= v.x;
			
			return *this;
		};
		double dot(const Vector *v);
		Vector* cross(const Vector *v);
		Vector* normalize();
		double size();
		Vector* scale(const double k);
		Vector* multiply(const Matrix *m);

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
	
	class Camera
	{
	public:
		Camera(Vector *eye, Vector *at, Vector *up) {
			this->eye = eye;
			this->at = at;
			this->up = up;
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
	
	class Light
	{
	public:
		Light(Vector *position, Vector *direction, double brightness) {
			this->position = position;
			this->direction = direction;
			this->brightness = brightness;
		};
		~Light() {
			delete position;
		};
		
		/* data */
		Vector *position;
		Vector *direction;
		double brightness;
	};
	
	class Polygon
	{
	public:
		Polygon(std::array<Vector *, 3> vertex, int color) {
			this->vertex = vertex;
			this->color = color;
		};
		~Polygon();
		int real_color(Light *light);
		
		/* data */
		std::array<Vector *, 3> vertex;
		int color;
	};
	
	class Object
	{
	public:
		Object(const std::vector<Vector *>vertex, const std::vector<Wire *>wire, const std::string name) {
			this->vertex = vertex;
			this->wire = wire;
			this->name = name;
		};
		Object(const std::vector<Polygon *>polygon, const std::string name) {
			this->polygon = polygon;
			this->name = name;
		};
		~Object();
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
		std::vector<Polygon *>polygon;
	};
}
