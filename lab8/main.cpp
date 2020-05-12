#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

const int INF = 1e9;

struct Point
{
	int x, y;
	Point(int a, int b) : x(a), y(b) {}
	Point operator*(int mult) const
	{
		return Point(x*mult, y*mult);
	}
	void operator*=(int mult)
	{
		x *= mult;
		y *= mult;
	}
	Point operator-()
	{
		return (*this)*(-1);
	}

	bool operator<(const Point &p) const
	{
		return x < p.x || (x == p.x && y > p.y);
	}
	friend ostream &operator<<(ostream &os, Point const &p)
	{
		return os << p.x << ' ' << p.y;
	}
};

int area(const Point &a, const Point &b, const Point &c)
{
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

bool left(const Point &a, const Point &b, const Point &c)
{
	return area(a, b, c) > 0;
}

deque < int > andrew(int start, const vector < int > &bucket_top_i, const vector < Point > &in)
{
	deque < int > result;
	result.push_front(start);

	for (int i = 1; i < bucket_top_i.size(); ++i)
		if (i != -1)
		{
			int next = bucket_top_i[i];
			while (result.size() >= 2)
			{
				int front = result.front();
				int second_front = *(result.begin() + 1);
				if (left(in[second_front], in[front], in[next]))
					result.pop_front();
				else
					break;
			}
			result.push_front(next);
		}
	return result;
}

int main()
{
	ifstream fin("../input.txt");

	int n, k;
	fin >> n >> k;

	int upper_left_i = 0, lower_right_i = 0;
	vector < Point > in;

	for (int i = 0; i < n; ++i)
	{
		int x, y;
		fin >> x >> y;
		Point p(x, y);
		in.emplace_back(p);
		if (p < in[upper_left_i])
			upper_left_i = i;
		if (in[lower_right_i] < p)
			lower_right_i = i;
	}

	vector < int > start = {upper_left_i, lower_right_i};
	vector < vector < int > > bucket_top_i(2, vector < int >(k + 1, -1));
	vector < int > multiplier = {1, -1};
	vector < deque < int > > hull(2, deque < int >());

	for (int part = 0; part <= 1; ++part)
	{
		for (int i = 0; i < in.size(); ++i)
			in[i] *= multiplier[part];
		Point start_p = in[start[part]];
		Point end_p = in[start[1 - part]];
		for (int i = 0; i < in.size(); ++i)
			if (i != start[part])
			{
				Point p = in[i];
				int bucket = (p.x - start_p.x)*k/(end_p.x - start_p.x);
				if (bucket_top_i[part][bucket] == -1 || p.y > in[bucket_top_i[part][bucket]].y)
					bucket_top_i[part][bucket] = i;
			}
		hull[part] = andrew(start[part], bucket_top_i[part], in);
	}
	for (int part = 0; part <= 1; ++part)
		for (auto it = hull[part].rbegin(); it != hull[part].rend(); ++it)
		{
			int i = *it;
			if (i != hull[1 - part].front())
				cout << -in[i] << endl;
		}
	return 0;
}
