//
//  m3d.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include "m3d.h"
#include <algorithm>

using namespace std;
using namespace m3d;

#pragma mark - Matrix

Matrix* Matrix::_multiply(const Matrix *m)
{
	Matrix a(*this);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			double tmp = 0.0;
			for(int k = 0; k < 4; k++) {
				tmp += a.m[i * 4 + k] * m->m[k * 4 + j];
			}
			this->m[i * 4 + j] = tmp;
		}
	}
	
	return this;
}

Matrix* Matrix::view(const Camera camera)
{
	Vector a = camera.eye - camera.at;
	Vector z = Vector(a).normalize();
	Vector b = camera.up & z;
	Vector x = Vector(b).normalize();
	Vector y = z & x;
	double qx = camera.eye * x;
	double qy = camera.eye * y;
	double qz = camera.eye * z;
	*this *= Matrix({
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-qx, -qy, -qz, 1
	});
	
	return this;
}

Matrix* Matrix::projection(const double angle, const double aspect, const double near, const double far)
{
	double sy = cos(angle * 0.5) / sin(angle * 0.5);
	double sx =  sy / aspect;
	double sz = far / (far - near);
	*this *= Matrix({
 		sx			,0			,0			,0,
 		0			,sy			,0			,0,
 		0			,0			,sz			,1,
 		0			,0			,-sz * near	,0
	});
	
	return this;
}

Matrix* Matrix::screen(const double x, const double y)
{
 	double w, h;
 	w = x * 0.5;
 	h = y * 0.5;

	*this *= Matrix({
 		w	,0	,0	,0,
 		0	,h	,0	,0,
 		0	,0	,1	,0,
 		w	,h	,0	,1
 	});
	
	return this;
}

#pragma mark - Vector

Vector Vector::normalize()
{
	double size = this->size();
	this->x /= size;
	this->y /= size;
	this->z /= size;
	this->w = 1;
	
	return *this;
}

double Vector::size()
{
	return sqrt(pow(this->x, 2.0) + pow(this->y, 2.0) + pow(this->z, 2.0));
}

Vector* Vector::scale(const double k)
{
	this->x *= k;
	this->y *= k;
	this->z *= k;
	this->w = 1;
	
	return this;
}

Vector* Vector::multiply(const Matrix *m)
{
	double x = this->x;
	double y = this->y;
	double z = this->z;
	double w = this->w;
	this->x = m->m[ 0] * x + m->m[ 4] * y + m->m[ 8] * z + m->m[12] * w;
	this->y = m->m[ 1] * x + m->m[ 5] * y + m->m[ 9] * z + m->m[13] * w;
	this->z = m->m[ 2] * x + m->m[ 6] * y + m->m[10] * z + m->m[14] * w;
	this->w = m->m[ 3] * x + m->m[ 7] * y + m->m[11] * z + m->m[15] * w;
	
	this->scale(1 / this->w);
	
	return this;
}

#pragma mark - Polygon

unsigned long Polygon::real_color(Light light)
{
	Vector direction = light.at - light.position;
	double brightness = (this->normal_vector() * direction.normalize()) * light.brightness * -1;
	brightness = brightness < 0 ? 0 : brightness;
	unsigned long real_color = ((unsigned long)((this->color & 0xff0000) * brightness) & 0xff0000) + ((unsigned long)((this->color & 0x00ff00) * brightness) & 0x00ff00) + ((unsigned long)((this->color & 0x0000ff) * brightness) & 0x0000ff);
	
	return 0 < real_color ? real_color : 0;
}

Vector Polygon::center()
{
	return Vector(
				  (this->vertex[0]->x + this->vertex[1]->x + this->vertex[2]->x) / 3.0,
				  (this->vertex[0]->y + this->vertex[1]->y + this->vertex[2]->y) / 3.0,
				  (this->vertex[0]->z + this->vertex[1]->z + this->vertex[2]->z) / 3.0
				  );
}


bool Polygon::far(Polygon p, Vector from)
{
	double position1 = (this->center() - from).size();
	double position2 = (p.center() - from).size();

	return position1 > position2;
}

bool Polygon::front(Camera camera)
{
	Vector direction = camera.at - camera.eye;
	return this->normal_vector() * direction.normalize() < 0;
}

Vector Polygon::normal_vector()
{
	Vector v1 = *this->vertex[0] - *this->vertex[1];
	Vector v2 = *this->vertex[0] - *this->vertex[2];
	
	return (v1 & v2).normalize();
}

#pragma mark - Object

Object* Object::transform(const Matrix *m)
{
	vector<Polygon>::iterator it_p = this->polygon.begin();
	for (it_p = this->polygon.begin(); it_p != this->polygon.end(); ++it_p) {
		Polygon p = (Polygon)*it_p;
		array<Vector *, 3>::iterator it_a = p.vertex.begin();
		for (it_a = p.vertex.begin(); it_a != p.vertex.end(); ++it_a) {
			Vector *v = (Vector *)*it_a;
			v->multiply(m);
		}
	}
	
	return this;
}

Object* Object::transform(const std::array<double, 16> m)
{
	Matrix t(m);
	return this->transform(&t);
}

bool Object::far(Object object, Vector from)
{
	return false;
}
