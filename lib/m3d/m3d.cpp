//
//  m3d.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "m3d.h"

using namespace std;
using namespace m3d;

#pragma mark - Matrix

void Matrix::multiply(const Matrix *m)
{
	Matrix *a = new Matrix(*this);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			double tmp = 0.0;
			for(int k = 0; k < 4; k++) {
				tmp += a->m[i * 4 + k] * m->m[k * 4 + j];
			}
			this->m[i * 4 + j] = tmp;
		}
	}
}

Matrix* Matrix::create_matrix_multiply(const Matrix *m)
{
	Matrix *result = new Matrix(*this);
	result->multiply(m);
	
	return result;
}

void Matrix::view(const Camera *camera)
{
	Vector *a = camera->eye->create_vector_sub(camera->at);
	Vector *z = a->normalized_vector();
	Vector *b = camera->up->create_vector_cross(z);
	Vector *x = b->normalized_vector();
	Vector *y = z->create_vector_cross(x);
	double qx = camera->eye->dot(x);
	double qy = camera->eye->dot(y);
	double qz = camera->eye->dot(z);
	this->multiply(new Matrix({
		x->x, y->x, z->x, 0,
		x->y, y->y, z->y, 0,
		x->z, y->z, z->z, 0,
		-qx,  -qy,  -qz, 1
	}));
}

void Matrix::projection(double angle, double aspect, double near, double far)
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
}

void Matrix::screen(double x, double y)
{
 	double w,h;
 	w = x * 0.5;
 	h = y * 0.5;

	this->multiply(new Matrix({
 		w	,0	,0	,0,
 		0	,h	,0	,0,
 		0	,0	,1	,0,
 		w	,h	,0	,1
 	}));
}

#pragma mark - Vector

void Vector::add(const Vector *v)
{
	this->x += v->x;
	this->y += v->y;
	this->z += v->z;
	this->w = 1;
}

void Vector::sub(const Vector *v)
{
	this->x -= v->x;
	this->y -= v->y;
	this->z -= v->y;
	this->w = 1;
}

double Vector::dot(const Vector *v)
{
	return this->x * v->x + this->y * v->y + this->z * v->z;
}

void Vector::cross(const Vector *v)
{
	double x = this->x;
	double y = this->y;
	double z = this->z;
	this->x = y * v->z - z * v->y;
	this->y = z * v->x - x * v->z;
	this->z = x * v->y - y * v->x;
	this->w = 1;
}

void Vector::normalize()
{
	double size = this->size();
	this->x /= size;
	this->y /= size;
	this->z /= size;
	this->w = 1;
}

double Vector::size()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

void Vector::scaleTo(const double k)
{
	this->x *= k;
	this->y *= k;
	this->z *= k;
	this->w = 1;
}

void Vector::multiply(const Matrix *m)
{
	double x = this->x;
	double y = this->y;
	double z = this->z;
	double w = this->w;
	this->x = m->m[ 0] * x + m->m[ 4] * y + m->m[ 8] * z + m->m[12] * w;
	this->y = m->m[ 1] * x + m->m[ 5] * y + m->m[ 9] * z + m->m[13] * w;
	this->z = m->m[ 2] * x + m->m[ 6] * y + m->m[10] * z + m->m[14] * w;
	this->w = m->m[ 3] * x + m->m[ 7] * y + m->m[11] * z + m->m[15] * w;
	
	this->scaleTo(1 / this->w);
}

Vector* Vector::create_vector_add(const Vector *v)
{
	Vector *result = new Vector(*this);
	result->add(v);
	
	return result;
}

Vector* Vector::create_vector_sub(const Vector *v)
{
	Vector *result = new Vector(*this);
	result->sub(v);
	
	return result;
}

Vector* Vector::create_vector_cross(const Vector *v)
{
	Vector *result = new Vector(*this);
	result->cross(v);
	
	return result;
}

Vector* Vector::normalized_vector()
{
	double size = this->size();
	Vector *result = new Vector(this->x / size, this->y / size, this->z / size);
	
	return result;
}

Vector* Vector::scale(const double k)
{
	return new Vector(this->x * k, this->y * k, this->z * k);
}

#pragma mark - Object

Object* Object::transform(Matrix *m)
{
	vector<Vector *>::iterator it_v = this->vertex.begin();
	for (it_v = this->vertex.begin(); it_v != this->vertex.end(); ++it_v) {
		Vector *v = (Vector *)*it_v;
		v->multiply(m);
	}
	
	return this;
}

Object* Object::transform(std::array<double, 16> m)
{
	Matrix t(m);
	vector<Vector *>::iterator it_v = this->vertex.begin();
	for (it_v = this->vertex.begin(); it_v != this->vertex.end(); ++it_v) {
		Vector *v = (Vector *)*it_v;
		v->multiply(&t);
	}
	
	return this;
}
