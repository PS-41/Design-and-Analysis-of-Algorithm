#include <vector>
#include <limits>

/**
 * Represents a point (x, y) in 2-D plane.
 */
class Point
{
public:
    double x, y;
    Point(){}
    Point(double, double);
    bool operator<(Point&);
};

/**
 * Represents a line y = mx + c in 2-D plane.
 * Stores the value of slope(m) and y-intercept(c).
 */
class Line
{
public:
    double m, c;
    Line(){}
    Line(double, double);
};

/**
 * Implements the Segmented Least Squares Algorithm to solve the 
 * line fitting problem of finding (multiple) best fit lines.
 */
class SLS
{
    static constexpr double inf = std::numeric_limits<double>::infinity(); ///< Represents infinity.
    int n; ///< Number of points in the plane.
    std::vector<Point> points; ///< list of all the points.
    std::vector<std::vector<double>> err; ///< Stores the least squares error err[i][j] for the segment points[i] to points[j].
    std::vector<int> idx; ///< idx[i] represents the index of the point such that the segment from idx[i] to i is included in the optimal answer.
    std::vector<std::vector<Line>> lines; ///< Stores the best fit lines lines[i][j] for the segment points[i] to points[j].

    void init();
    void lse(int, int);
    double sls();

public:
    double min_penalty; ///< The minimum penalty achieved using the algorithm.
    int segments; ///< The optimal number of segments in which the set of points are partitioned.
    double C; ///< The cost multiplier for partitioning into segments.
    
    void read();
    void compute();
};
