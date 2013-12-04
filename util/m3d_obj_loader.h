//
//  m3d_obj_loader.h
//  m3d
//
//  Created by Akira Matsuda on 2013/12/03.
//  Copyright (c) 2013年 Akira Matsuda. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "m3d.h"

using namespace std;
using namespace m3d;

namespace m3d
{
	class obj_loader {
	public:
		static Object* load(const string name, const string filename, int vertex_num, unsigned long color) {
			ifstream ifs(filename);
			string str;
			
			if (ifs.fail()) {
				cerr << "File do not exist.\n";
				exit(0);
			}
			
			cout << "-----" << endl;
			cout << "object = " << name << endl;
			cout << "path = " << filename << endl;
			
			int counter = 0;
			double a = 0, b = 0, c = 0;
			std::vector<Vector> v;
			while(counter < vertex_num && getline(ifs, str)) {
				counter++;
				a = 0;
				b = 0;
				c = 0;
				sscanf(str.data(), "%lf %lf %lf", &a, &b, &c);
				Vector vertex(a, b, c);
				v.push_back(vertex);
			}
			
			cout << "vertex = " << counter << endl;
			
			counter = 0;
			std::vector<Polygon> p;
			while (getline(ifs, str)) {
				counter++;
				a = 0;
				b = 0;
				c = 0;
				sscanf(str.data(), "%lf %lf %lf", &a, &b, &c);
				Polygon polygon({
					new Vector(v.at(a)),
					new Vector(v.at(b)),
					new Vector(v.at(c))
				}, color);
				p.push_back(polygon);
			}
			
			cout << "polygon = " << counter << endl;
			
			return new Object(p, name);
		};
	};
}
