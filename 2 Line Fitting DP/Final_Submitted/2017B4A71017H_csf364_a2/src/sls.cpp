#include "sls.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

/**
 * Constructor. Initializes the x-coordinate and y-coordinate for the point (x,y).
 */ 
Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

/**
 * Comparator for sorting the list of points.
 */ 
bool Point::operator<(Point& b)
{
    if(x == b.x)
        return y < b.y;
    return x < b.x;
}

/**
 * Constructor. Initializes the value of m and c for the line y = mx + c.
 */ 
Line::Line(double m, double c)
{
    this->m = m;
    this->c = c;
}

/**
 * This function initializes all the necessary values like the multiplier C.
 * Removes duplicates from the set of points and initializes the points.
 * Resize all the vectors with the total size that is n.
 */ 
void SLS::init()
{
    std::sort(points.begin(), points.end());
    std::vector<Point> tmp;
    int cnt = 1;
    for (int i = 0; i < n - 1; i++)
    {
        if (points[i].x != points[i + 1].x or points[i].y != points[i + 1].y)
            tmp.push_back(points[i]), cnt++;
    }
    tmp.push_back(points[n - 1]);
    n = cnt;
    points.resize(n);
    for (int i = 0; i < n; i++)
        points[i] = tmp[i];
    err.resize(n);
    for(auto& it : err)
        it.resize(n);
    idx.resize(n);
    lines.resize(n);
    for(auto& it : lines)
        it.resize(n);
}

/**
 * Computes the least squares error for the segment points[l] to points[r].
 * Stores the error for the segment in err[l][r].
 * Stores the best fit lines for the segment in lines[l][r].
 * @param[in] l indicates the leftmost point in the segment.
 * @param[in] r indicates the rightmost point in the segment.
 */
void SLS::lse(int l, int r)
{
    int k = r - l + 1;
    double x = 0, y = 0, xy = 0, xx = 0;
    for(int i = l; i <= r; i++)
    {
        x += points[i].x;
        y += points[i].y;
        xy += (points[i].x * points[i].y);
        xx += (points[i].x * points[i].x);
    }
    double t1 = k * xy;
    double t2 = x * y;
    double t3 = k * xx;
    double t4 = x * x;
    double m = inf, c = x / k;
    if (t3 != t4)
    {
        m = (t1 - t2) / (t3 - t4);
        c = (y - m*x) / k;
    }
    double error = 0;
    for(int i = l; i <= r; i++)
    {
        x = points[i].x, y = points[i].y;
        double e = x - c;
        if (m != inf) e = y - m*x - c;
        error += e*e;
    }
    if (error < 0.000001) error = 0;
    err[l][r] = error;
    lines[l][r] = Line(m, c);
}

/**
 * Computes the minimum penalty in the optimal partition using dynamic programming.
 * Also stores the segments of the optimal partition in idx.
 * @returns The minimum penalty which is stored in dp[n].
 */
double SLS::sls()
{
    if (n == 1) return 0;
    std::vector<double> dp(n+1);
    for(int i = 0; i < n; ++i)
        for(int j = i+1; j < n; ++j)
            lse(i, j);
    for(int j = 1; j <= n; ++j)
    {
        double val = inf;
        int ind;
        for(int i = 1; i < j; i++)
            if(err[i-1][j-1] + C + dp[i-1] < val)
                val = err[i-1][j-1] + C + dp[i-1], ind = i;
        dp[j] = val;
        idx[j-1] = ind-1;
    }
    return dp[n];
}

/**
 * Reads n number of points (x, y).
 * Calls the init function to initialise and resize necessary attributes.
 */
void SLS::read()
{
    std::cin >> n >> C;
    points.resize(n);
    for(auto& it : points)
        std::cin >> it.x >> it.y;
    init();
}

/**
 * Computes the minimum penalty by calling the sls() function.
 * Writes the output (multiple best fit lines obtained) to a file for visualization.
 */
void SLS::compute()
{
    min_penalty = sls();
    std::ofstream file;
    file.open("..\\output\\out.txt");
    file << min_penalty << ' ' << C << '\n';
    segments = 0;
    if (n > 1)
    {
        for(int j = n-1; j >= 0; j = idx[j]-1)
        {
            double m = lines[idx[j]][j].m, c = lines[idx[j]][j].c;
            double start = points[idx[j]].x, stop = points[j].x;
            if (m == inf) start = points[idx[j]].y, stop = points[j].y;
            file << m << ' ' << c << ' ' << start << ' ' << stop << '\n';
            segments++;
        }
    }
    file.close();
}