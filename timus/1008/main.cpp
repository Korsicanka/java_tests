#include <vector>
#include <deque>
#include <stack>
#include <iostream>
#include <iterator>
#include <inttypes.h>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Point {
	uint32_t x;
	uint32_t y;

	Point(): x(0), y(0) {}

	Point(uint32_t x, uint32_t y) {
		this->x = x;
		this->y = y;
	}

	bool operator<(const Point &p) {
		if (y <= p.y && x <=p.x) {
			return true;
		} else {
			return false;
		}
	}

	bool operator>(const Point &p) {
		if (y >= p.y && x >= p.x) {
			return true;
		} else {
			return false;
		}
	}

	void output() {
		cout << "x=" << x << ",y=" << y << endl;
	}
};

struct OffPoint: public Point {
	string s;
	OffPoint(string str, uint32_t x, uint32_t y): Point(x,y), s(str) { }
};

void output_image(uint32_t *img, uint32_t x_sz, uint32_t y_sz) {
#ifdef DEBUG
	for (int32_t j = y_sz-1; j >= 0; --j) {
		for (uint32_t i = 0; i < x_sz; ++i) {
			char sym = '*';
			switch (img[i+x_sz*j]) {
				case 0:  sym = 'o'; break;
				case 1:  sym = '1'; break;
				case 2:  sym = '2'; break;
				default: sym = 'x'; break;
			}
			cout << " " << sym;
		}
		cout << endl;
	}
#endif
}

int main(int argc, char *argv[]) {

	vector<Point> points;
	uint32_t points_count;

	/* read input data */
	cin >> points_count;
	points.resize(points_count);

	uint32_t min_x = points_count, min_y = points_count,
					 max_x = 0, max_y = 0;
	Point min_point(points_count, points_count);

	for (uint32_t i = 0; i < points_count; ++i) {
		cin >> points[i].x >> points[i].y;
		if (points[i].x < min_x) { min_x = points[i].x; }
		if (points[i].y < min_y) { min_y = points[i].y; }
		if (points[i].x > max_x) { max_x = points[i].x; }
		if (points[i].y > max_y) { max_y = points[i].y; }
		if (points[i] < min_point) { min_point = points[i]; }
	}

	/* transform data to image */
	uint32_t x_size = max_x + 1, y_size = max_y + 1;
	uint32_t image[x_size*y_size];
	memset(&image, 0, x_size*y_size*sizeof(uint32_t));

	for (vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
		image[it->x + x_size*it->y] = 1;
	}

	output_image(image, x_size, y_size);

	/* process data */
	deque<Point> neighbors;
	stringstream result;
	image[min_point.x+x_size*min_point.y] = 2;
	output_image(image, x_size, y_size);
	result << min_point.x << " " << min_point.y << endl;
	neighbors.push_back(min_point);

	vector<OffPoint> offsets;
	offsets.push_back(OffPoint("R",1,0));
	offsets.push_back(OffPoint("T",0,1));
	offsets.push_back(OffPoint("L",-1,0));
	offsets.push_back(OffPoint("B",0,-1));

	while (0 != neighbors.size()) {
		Point active_point = neighbors.front();
		neighbors.pop_front();
		for (vector<OffPoint>::const_iterator it = offsets.cbegin(); it != offsets.cend(); it++) {
			uint32_t x = active_point.x+it->x, y = active_point.y+it->y;
			if (x <= x_size && y <= y_size && image[x+x_size*y] == 1) {
				neighbors.push_back(Point(active_point.x+it->x, active_point.y + it->y));
				image[x+x_size*y] = 2;
				output_image(image, x_size, y_size);
				result << it->s;
			}
		}
		result << "," << endl;
	}
	result << ".";

	cout << result.str() << endl;

	/* output result  */
	return 0;
}

