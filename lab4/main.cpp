#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Point
{
	int x, y;
	Point(int a = 0, int b = 0) : x(a), y(b) {}
	bool operator<(const Point &p) const
	{
		return x < p.x;
	}
	friend ostream &operator<<(ostream &os, const Point &p)
	{
		return os << p.x << ' ' << p.y;
	}
	friend istream &operator>>(istream &is, Point &p)
	{
		return is >> p.x >> p.y;
	}
};

class SegmentTree
{
	vector < vector < Point > > data;
	vector < Point > points;
	void build(int v, int l, int r)
	{
		data[v] = vector < Point >(r - l);
		int lp = 0, rp = 0;
		if (l + 1 == r)
			data[v][0] = points[l];
		else
		{
			int m = (l + r) >> 1;
			build(v << 1, l, m);
			build(v << 1 | 1, m, r);

			for (int i = 0; i < r - l; ++i)
			{
				if (lp != m - l && (rp == r - m || data[v << 1][lp].y < data[v << 1 | 1][rp].y))
				{
					data[v][i] = data[v << 1][lp];
					++lp;
				}
				else
				{
					data[v][i] = data[v << 1 | 1][rp];
					++rp;
				}
			}
		}
		cout << v << ' ' << l << ' ' << r << '\t' << lp << ' ' << rp << endl;
		for (Point p: data[v])
			cout << p << "  ";
		cout << endl;
	}
	int binary_search(const vector < Point > &v, int value, bool coordinate)
	{
		int l = -1, r = v.size();
		while (l < r - 1)
		{
			int m = (l + r) >> 1;
			int m_value = coordinate ? v[m].y : v[m].x;
			if (m_value < value)
				l = m;
			else
				r = m;
		}
		return l + 1;
	}
	void query(int v, int l, int r, int xl, int xr, int yl, int yr, vector < Point > &result)
	{
		if (l == xl && xr == r)
		{
			int ql = binary_search(data[v], yl, 1);
			int qr = binary_search(data[v], yr, 1);
			cout << v << ' ' << l << ' ' << r << ' ' << xl << ' ' << xr << ' ' << yl << ' ' << yr << '\t'
			     << ql << ' ' << qr << endl;
			for (int i = ql; i < qr; ++i)
				result.push_back(data[v][i]);
		}
		else
		{
			int m = (l + r) >> 1;
			if (xr <= m)
				query(v << 1, l, m, xl, xr, yl, yr, result);
			else if (xl >= m)
				query(v << 1 | 1, m, r, xl, xr, yl, yr, result);
			else
			{
				query(v << 1, l, m, xl, m, yl, yr, result);
				query(v << 1 | 1, m, r, m, xr, yl, yr, result);
			}
		}
	}
public:
	explicit SegmentTree(vector < Point > const &in)
	{
		points = in;
		sort(points.begin(), points.end());
		data.resize(in.size() << 2);
		build(1, 0, points.size());
	}
	vector < Point > query(Point lower_left, Point upper_right)
	{
		vector < Point > result;
		int xl = binary_search(points, lower_left.x, 0);
		int xr = binary_search(points, upper_right.x+1, 0);
		cout << xl << ' ' << xr << endl << endl;
		query(1, 0, points.size(), xl, xr, lower_left.y, upper_right.y+1, result);
		cout << endl;
		return result;
	}
};

int main()
{
	ifstream fin("../input.txt");

	int n;
	fin >> n;

	vector < Point > in;

	for (int i = 0; i < n; ++i)
	{
		int x, y;
		fin >> x >> y;
		in.emplace_back(x, y);
	}
	SegmentTree st = SegmentTree(in);
	while (true)
	{
		Point lower_left, upper_right;
		cout << "Enter four coordinates:" << endl;
		cin >> lower_left >> upper_right;
		vector < Point > response = st.query(lower_left, upper_right);
		cout << "Number of points found: " << response.size() << endl;
		for (Point p: response)
			cout << p << endl;
		cout << endl;
	}
	return 0;
}
