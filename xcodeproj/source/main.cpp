//
//  main.cpp
//  m3d
//
//  Created by Akira Matsuda on 2013/11/13.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#include <iostream>
#include <array>
#include "m3d.h"
#include "xm3d.h"

using namespace std;
using namespace m3d;

int main(int argc, const char * argv[])
{
	// insert code here...
	cout << "Hello, World!" << endl;
	
//	Vector planeVec[] = {
//		0	,0	,-7	,1,//0
//		2	,0	,0	,1,//1
//		-2	,0	,0	,1,//2
//		2	,-1	,0	,1,//3
//		-2	,-1	,0	,1,//4
//		6	,-1	,3	,1,//5
//		6	,-1	,4	,1,//6
//		-6	,-1	,3	,1,//7
//		-6	,-1	,4	,1,//8
//		1	,0	,4	,1,//9
//		1	,-1	,4	,1,//10
//		-1	,-1	,4	,1,//11
//		-1	,0	,4	,1,//12
//		0	,-1	,4	,1,//13
//		2	,0	,6	,1,//14
//		-2	,0	,6	,1,//15
//		1	,0	,6	,1,//16
//		1	,-1	,6	,1,//17
//		1	,1	,6	,1,//18
//		-1	,1	,6	,1,//19
//		-1	,-1	,6	,1,//20
//		-1	,0	,6	,1,//21
//		0	,1	,0	,1,//22
//		6	,0	,4	,1,//23
//		6	,0	,3	,1,//24
//		-6	,0	,4	,1,//25
//		-6	,0	,3	,1,//26
//		0	,0	,6	,1,//27
//	};
//	Wire planeWire[] = {
//		0,	1,	0,	2,	0,	3,	0,	4,	0,	22,//5
//		1,	3,	1,	5,	1,	9,	1,	22,//4
//		2,	4,	2,	7,	2,	12,	2,	22,//4
//		3,	4,	3,	5,//2
//		4,	7,	//1
//		5,	6,	5,	24,//2
//		6,	9,	6,	10,	6,	23,//3
//		7,	8,	7,	26,//2
//		8,	11,	8,	12,	8,	25,//3
//		9,	10,	9,	12,	9,	14,	9,	16,	9,	18,	9,	22,//6
//		10,	11,	10,	14,	10,	17,//3
//		11,	12,	11,	15,//2
//		12,	19,	12,	22,//2
//		13,	16,	13,	20,	13,	21,//3
//		14,	16,	14,	17,//2
//		15,	19,	15,	20,	15,	21,//3
//		16,	17,	16,	18,	16,	21,	16,	27,//4
//		17,	27,//1
//		18,	14,	18,	27,//2
//		19,	27,//1
//		20,	21,	20,	27,//2
//		23,	24,//1
//		25,	26,//1
//	};
	
	auto *cube = new Object({
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
	}, "cube");
	
	cube->transform(Matrix::scale(10, 10, 10));
	auto view = new xm3d(800, 800, "cube", [cube](unsigned int life, XEvent e, Window window) {
		cube->transform(Matrix::rotate(m3d_axis_y, 0.01));
	});
	view->add_obj(cube);
	view->run();
	
	return 0;
}
