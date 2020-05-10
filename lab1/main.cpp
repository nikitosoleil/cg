#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point
{
	int x, y;
	Point(int a, int b) : x(a), y(b) {}
	bool operator<(Point p)
	{
		return x < p.x || (x == p.x && y < p.y);
	}
};

int area(Point a, Point b, Point c)
{
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

bool right(Point a, Point b, Point c)
{
	return area(a, b, c) <= 0;
}

struct Sorter
{
	Point start;
	Sorter(Point u) : start(u) {}
	bool operator()(Point u, Point v)
	{
		return area(start, u, v) <= 0;
	}
};

int main()
{
	ifstream fin("../input.txt");

	int n, start = 0;
	fin >> n;
	vector < Point > polygon;

	for (int i = 0; i < n; ++i)
	{
		int x, y;
		fin >> x >> y;
		Point p(x, y);
		polygon.emplace_back(p);
		if (p < polygon[start])
			start = i;
	}

	swap(polygon[start], polygon[0]);
	sort(polygon.begin()+1, polygon.end(), Sorter(polygon[0]));

//	for(auto it: polygon)
//		cout << it.x << ' ' << it.y << endl;

	while (true)
	{
		cout << "X, Y:" << endl;
		int x, y;
		cin >> x >> y;
		Point input(x, y);

		int l = 0, r = n;
		while (r - l > 1)
		{
			int m = (l + r) >> 1;
			if (right(polygon[0], polygon[m], input))
				l = m;
			else
				r = m;
		}
		if (!right(polygon[0], polygon[1], input) || !right(polygon[l], polygon[(l + 1) % n], input))
			cout << "OUT\n";
		else
			cout << "IN\n";
		// break;
	}
	return 0;
}
