#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

#define abc(x) ((x) ? (x) > 0 : -(x))

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

bool left(Point a, Point b, Point c)
{
	return area(a, b, c) > 0;
}

bool collinear(Point a, Point b, Point c)
{
	return area(a, b, c) == 0;
}

bool between(Point a, Point b, Point c)
{
	return abs(c.x - a.x) <= abs(b.x - a.x) && abs(c.y - a.y) <= abs(b.y - a.y);
}

int main()
{
	ifstream fin("../input.txt");

	int n, start = 0;
	fin >> n;
	vector < Point > in;

	for (int i = 0; i < n; ++i)
	{
		int x, y;
		fin >> x >> y;
		Point p(x, y);
		in.emplace_back(p);
		if (p < in[start])
			start = i;
	}

	vector < bool > selected(in.size(), false);
	selected[start] = true;

	vector < Point > convex_hull;
	convex_hull.push_back(in[start]);

	while (true)
	{
		int best = -1;
		Point previous = convex_hull[convex_hull.size() - 1];
		for (int i = 0; i < in.size(); ++i)
		{
			if (!selected[i])
			{
				if (best == -1)
					best = i;
				else if (left(previous, in[best], in[i]))
					best = i;
				else if (collinear(previous, in[best], in[i]) && between(previous, in[best], in[i]))
					best = i;
			}
		}
		if (best == -1 || left(previous, in[best], in[start]))
			break;
		else
		{
			selected[best] = true;
			convex_hull.push_back(in[best]);
		}
	}

	cout << convex_hull.size() << endl;
	for (Point p: convex_hull)
		cout << p.x << ' ' << p.y << endl;
	return 0;
}
