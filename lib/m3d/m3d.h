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
		Matrix* view(const Camera camera);
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
		Vector() {
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		};
		Vector(const double x, const double y, const double z) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 1;
		};
		Vector(const Vector &v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
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
		Vector operator-(const Vector& v) const {
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
		double operator*(const Vector& v) const {
			return this->x * v.x + this->y * v.y + this->z * v.z;
		};
		Vector operator*(const Matrix& m) const {
			Vector tmp(*this);
			double x = this->x;
			double y = this->y;
			double z = this->z;
			double w = this->w;
			tmp.x = m.m[ 0] * x + m.m[ 4] * y + m.m[ 8] * z + m.m[12] * w;
			tmp.y = m.m[ 1] * x + m.m[ 5] * y + m.m[ 9] * z + m.m[13] * w;
			tmp.z = m.m[ 2] * x + m.m[ 6] * y + m.m[10] * z + m.m[14] * w;
			tmp.w = m.m[ 3] * x + m.m[ 7] * y + m.m[11] * z + m.m[15] * w;
			
			tmp.scale(1 / tmp.w);
			
			return tmp;
		};
		Vector& operator*=(const Matrix& m) {
			double x = this->x;
			double y = this->y;
			double z = this->z;
			double w = this->w;
			this->x = m.m[ 0] * x + m.m[ 4] * y + m.m[ 8] * z + m.m[12] * w;
			this->y = m.m[ 1] * x + m.m[ 5] * y + m.m[ 9] * z + m.m[13] * w;
			this->z = m.m[ 2] * x + m.m[ 6] * y + m.m[10] * z + m.m[14] * w;
			this->w = m.m[ 3] * x + m.m[ 7] * y + m.m[11] * z + m.m[15] * w;
			
			this->scale(1 / this->w);
			
			return *this;
		};
		Vector operator&(const Vector& v) const {
			Vector tmp(*this);
			double x = tmp.x;
			double y = tmp.y;
			double z = tmp.z;
			tmp.x = y * v.z - z * v.y;
			tmp.y = z * v.x - x * v.z;
			tmp.z = x * v.y - y * v.x;
			tmp.w = 1;
		
			return tmp;
		};
		Vector& operator&=(const Vector& v) {
			double x = this->x;
			double y = this->y;
			double z = this->z;
			this->x = y * v.z - z * v.y;
			this->y = z * v.x - x * v.z;
			this->z = x * v.y - y * v.x;
			this->w = 1;
			
			return *this;
		};
		Vector normalize();
		double size();
		Vector* scale(const double k);
		Vector* multiply(const Matrix *m);

		/* data */
		double x;
		double y;
		double z;
		double w;
	};
	
	class Camera
	{
	public:
		Camera() {
			this->eye = Vector(0, 0, 0);
			this->at = Vector(0, 0, 0);
			this->up = Vector(0, 0, 0);
		};
		Camera(Vector eye, Vector at, Vector up) {
			this->eye = eye;
			this->at = at;
			this->up = up;
		};
		Camera(const Camera &c) {
			this->eye = c.eye;
			this->at = c.at;
			this->up = c.up;
		};
		~Camera() {
		};
		void lookup(const Vector target);
		void move(const Vector to);
		
		/* data */
		Vector eye;
		Vector at;
		Vector up;
	};
	
	class Light
	{
	public:
		Light() {
			this->position = Vector(0, 0, 0);
			this->at = Vector(0, 0, 0);
			this->brightness = 1.0;
		}
		Light(Vector position, Vector at, double brightness) {
			this->position = position;
			this->at = at;
			this->brightness = brightness;
		};
		~Light() {
		};
		
		/* data */
		Vector position;
		Vector at;
		double brightness;
	};
	
	class Polygon
	{
	public:
		Polygon(const std::array<Vector *, 3> vertex, const unsigned long color) {
			this->vertex = vertex;
			this->color = color;
		};
		~Polygon() {
			
		};
		unsigned long real_color(Light light);
		bool far(Polygon p, Vector from);
		bool front(Camera camera);
		Vector center();
		Vector normal_vector();
		
		/* data */
		std::array<Vector *, 3> vertex;
		unsigned long color;
	};
	
	class Object
	{
	public:
		Object() {
		};
		Object(const std::vector<Polygon>polygon, const std::string name) {
			this->polygon = polygon;
			this->name = name;
		};
		~Object() {
			
		};
		Object* transform(const Matrix *m);
		Object* transform(const std::array<double, 16> m);
		bool far(Object object, Vector from);
		static Object* fill_cube(const std::string name, unsigned long color) {
			return new Object({
				Polygon({
					new Vector( 1,  1, -1),
					new Vector(-1,  1, -1),
					new Vector( 1,  1,  1)
				}, color),
				Polygon({
					new Vector( 1,  1,  1),
					new Vector(-1,  1, -1),
					new Vector(-1,  1,  1)
				}, color),
				Polygon({
					new Vector( 1, -1, -1),
					new Vector( 1,  1, -1),
					new Vector( 1,  1,  1)
				}, color),
				Polygon({
					new Vector( 1, -1,  1),
					new Vector( 1, -1, -1),
					new Vector( 1,  1,  1)
				}, color),
				Polygon({
					new Vector( 1, -1, -1),
					new Vector(-1, -1, -1),
					new Vector( 1,  1, -1)
				}, color),
				Polygon({
					new Vector( 1,  1, -1),
					new Vector(-1, -1, -1),
					new Vector(-1,  1, -1)
				}, color),
				Polygon({
					new Vector(-1, -1,  1),
					new Vector( 1, -1,  1),
					new Vector( 1,  1,  1),
				}, color),
				Polygon({
					new Vector(-1, -1,  1),
					new Vector( 1,  1,  1),
					new Vector(-1,  1,  1)
				}, color),
				Polygon({
					new Vector(-1, -1, -1),
					new Vector(-1, -1,  1),
					new Vector(-1,  1,  1)
				}, color),
				Polygon({
					new Vector(-1, -1, -1),
					new Vector(-1,  1,  1),
					new Vector(-1,  1, -1)
				}, color),
				Polygon({
					new Vector(-1, -1, -1),
					new Vector( 1, -1, -1),
					new Vector( 1, -1,  1)
				}, color),
				Polygon({
					new Vector(-1, -1,  1),
					new Vector(-1, -1, -1),
					new Vector( 1, -1,  1)
				}, color)
			}, name);
		};
		
		/* data */
		std::string name;
		std::vector<Polygon>polygon;
	};
}
