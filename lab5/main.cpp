#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Point
{
	int x, y;
	Point(int a, int b) : x(a), y(b) {}
	bool operator<(const Point &p) const
	{
		return x < p.x || (x == p.x && y < p.y);
	}
};

int area(const Point &a, const Point &b, const Point &c)
{
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

void f(Point l, Point r, const vector < Point > &candidates, vector < Point > &convex_hull)
{
	int best = 0;
	vector < Point > actual_candidates;
	for (Point candidate: candidates)
		if (area(l, r, candidate) >= 0)
		{
			actual_candidates.push_back(candidate);
			if (area(l, r, candidate) >= area(l, r, actual_candidates[best]))
				best = actual_candidates.size() - 1;
		}
	if (!actual_candidates.empty())
	{
		Point chosen = actual_candidates[best];
		actual_candidates.erase(actual_candidates.begin() + best);
		f(l, chosen, actual_candidates, convex_hull);
		convex_hull.push_back(chosen);
		f(chosen, r, actual_candidates, convex_hull);
	}
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
	Point l = in[start];
	in.erase(in.begin() + start);
	vector < Point > convex_hull = {l};
	Point r(l.x, l.y - 1);

	f(l, r, in, convex_hull);

	cout << convex_hull.size() << endl;
	for (Point p: convex_hull)
		cout << p.x << ' ' << p.y << endl;
	return 0;
}
