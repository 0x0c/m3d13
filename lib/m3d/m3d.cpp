//
//  m3d.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include "m3d.h"

using namespace std;
using namespace m3d;

#pragma mark - Matrix

Matrix* Matrix::multiply(const Matrix *m)
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

Matrix* Matrix::create_matrix_multiply(const Matrix *m)
{
	Matrix *result = new Matrix(*this);
	result->multiply(m);
	
	return result;
}

Matrix* Matrix::view(const Camera *camera)
{
	Vector *a = Vector(*camera->eye).sub(camera->at);
	Vector *z = Vector(*a).normalize();
	Vector *b = Vector(*camera->up).cross(z);
	Vector *x = Vector(*b).normalize();
	Vector *y = Vector(*z).cross(x);
	double qx = camera->eye->dot(x);
	double qy = camera->eye->dot(y);
	double qz = camera->eye->dot(z);
	this->multiply(new Matrix({
		x->x, y->x, z->x, 0,
		x->y, y->y, z->y, 0,
		x->z, y->z, z->z, 0,
		 -qx,  -qy,  -qz, 1
	}));
	
	return this;
}

Matrix* Matrix::projection(const double angle, const double aspect, const double near, const double far)
{
	double sy = cos(angle * 0.5) / sin(angle * 0.5);
	double sx =  sy / aspect;
	double sz = far / (far - near);
	this->multiply(new Matrix({
 		sx			,0			,0			,0,
 		0			,sy			,0			,0,
 		0			,0			,sz			,1,
 		0			,0			,-sz * near	,0
	}));
	
	return this;
}

Matrix* Matrix::screen(const double x, const double y)
{
 	double w, h;
 	w = x * 0.5;
 	h = y * 0.5;

	this->multiply(new Matrix({
 		w	,0	,0	,0,
 		0	,h	,0	,0,
 		0	,0	,1	,0,
 		w	,h	,0	,1
 	}));
	
	return this;
}

#pragma mark - Vector

Vector* Vector::add(const Vector *v)
{
	this->x += v->x;
	this->y += v->y;
	this->z += v->z;
	this->w = 1;
	
	return this;
}

Vector* Vector::sub(const Vector *v)
{
	this->x -= v->x;
	this->y -= v->y;
	this->z -= v->y;
	this->w = 1;
	
	return this;
}

double Vector::dot(const Vector *v)
{
	return this->x * v->x + this->y * v->y + this->z * v->z;
}

Vector* Vector::cross(const Vector *v)
{
	double x = this->x;
	double y = this->y;
	double z = this->z;
	this->x = y * v->z - z * v->y;
	this->y = z * v->x - x * v->z;
	this->z = x * v->y - y * v->x;
	this->w = 1;
	
	return this;
}

Vector* Vector::normalize()
{
	double size = this->size();
	this->x /= size;
	this->y /= size;
	this->z /= size;
	this->w = 1;
	
	return this;
}

double Vector::size()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
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

#pragma mark - Object

Object* Object::transform(const Matrix *m)
{
//	vector<Polygon *>::iterator it_p = this->polygon.begin();
//	for (it_p = this->polygon.begin(); it_p != this->polygon.end(); ++it_p) {
//		Polygon *p = (Polygon *)*it_p;
//		array<Vector *, 3>::iterator it_a = p->vertex.begin();
//		for (it_a = p->vertex.begin(); it_a != p->vertex.end(); ++it_a) {
//			Vector *v = (Vector *)*it_a;
//			v->multiply(m);
//		}
//	}
	
	vector<Vector *>::iterator it_v = this->vertex.begin();
	for (it_v = this->vertex.begin(); it_v != this->vertex.end(); ++it_v) {
		Vector *v = (Vector *)*it_v;
		v->multiply(m);
	}
	
	return this;
}

Object* Object::transform(const std::array<double, 16> m)
{
	Matrix t(m);
	
	return this->transform(&t);
}
