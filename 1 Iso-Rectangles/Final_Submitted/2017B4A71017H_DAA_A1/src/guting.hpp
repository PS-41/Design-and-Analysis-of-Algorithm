#include <vector>
#include <limits>

/**
 * Implements a solution to the Measure and Contour problem by using
 * a single time-optimal divide-and-conquer algorithm.
 */
class Guting
{
private:
    const double INF = std::numeric_limits<double>::infinity(); ///< Represents infinity.
    std::vector<double> leaf; ///< Stores the x-coordinate of leaf nodes of the binary search tree.
    std::vector<std::tuple<double, double, double, bool>> cont; ///< Stores the horizontal contour edges.

    /**
     * Represents interval which can be x interval or y interval.
     */
    class Interval
    {
    public:
        double bot, top;
        Interval(){}

        Interval(double bot, double top)
        {
            this->bot = bot, this->top = top;
        }

        bool operator<(const Interval& b) const
        {
            if(bot == b.bot)
                return top < b.top;
            return bot < b.bot;
        }

        bool operator==(const Interval& b) const
        {
            return bot == b.bot and top == b.top;
        }

        bool inside(const Interval& b) const
        {
            return b.bot <= bot and top <= b.top;
        }

        double width() const
        {
            return top - bot;
        }
    };

    /**
     * Represents a rectangle with x_left, x_right, y_bot and y_top
     */
    class Rectangle
    {
    public:
        double x_left, x_right, y_bot, y_top;

        Rectangle(double l, double r, double b, double t)
        {
            x_left = l, x_right = r, y_bot = b, y_top = t;
        }
    };

    std::vector<Rectangle> rects; ///< Stores the rectangles given in input.

    enum edgetype{LEFT, RIGHT, BOT, TOP}; ///< Side of the rectangle edge.
    
    /**
     * Represents an edge of the rectangle
     * usgin the interval, its x or y coordinate (xc), its partner (xp)
     * and the side it represents (bottom, top, left or right)
     */
    class Edge
    {
    public:
        Interval intr;
        double xc, xp;

        enum edgetype side;
        Edge(Interval intr, double xc, double xp, enum edgetype side)
        {
            this->intr = intr;
            this->xc = xc;
            this->xp = xp;
            this->side = side;
        }

        bool operator<(const Edge& b) const
        {
            if (xc == b.xc)
                return side < b.side;
            return xc < b.xc;
        }
    };

    enum LRU{LC, RC, RT}; ///< Represents Left Child(LC, Right Child(RC) or Inner node(RT)

    /**
     * Represents a binary search tree storing the endpoints
     * of the intervals in its leaves.
     */
    class CTree
    {
    public:
        double x;
        LRU parity;
        CTree *lson, *rson;
        CTree()
        {
            x = 0;
            parity = RT;
            lson = NULL;
            rson = NULL;
        }
        CTree(double x, LRU parity)
        {
            this->x = x;
            this->parity = parity;
            lson = NULL, rson = NULL;
        }
        CTree(double x, LRU parity, CTree* lson, CTree* rson)
        {
            this->x = x;
            this->parity = parity;
            this->lson = lson;
            this->rson = rson;
        }
    };

    /**
     * Represents a stripe which is a rectangular area
     * defined by x_interval and y_interval.
     */
    class Stripe
    {
    public:
        Interval x_intr, y_intr;
        double measure;
        CTree* ctree;
        std::vector<double> leaf;

        Stripe(Interval xi, Interval yi, double msr, CTree* ct)
        {
            x_intr = xi, y_intr = yi, measure = msr, ctree = ct;
        }
    };

    typedef std::tuple<
                std::vector<Edge>,
                std::vector<Edge>,
                std::vector<double>,
                std::vector<Stripe>
            > LRPS;

    std::vector<Interval> partitions(std::vector<double>&);
    std::vector<Stripe> copy(std::vector<Stripe>&, std::vector<double>&, Interval);
    void blacken(std::vector<Stripe>&, std::vector<Edge>&);
    std::vector<Stripe> concat(std::vector<Stripe>&, std::vector<Stripe>&, std::vector<double>&, Interval);
    LRPS merge(LRPS, LRPS, Interval, double);
    LRPS stripes(std::vector<Edge>, Interval);
    double find_measure(std::vector<Stripe>&);
    void dfs(CTree *tree);
    void construct_leaf(std::vector<Stripe>&);
    void query(Interval, std::vector<double>&, double, bool);
    double find_contour(std::vector<Stripe>&, std::vector<Edge>&);
    std::tuple<double, double> rectangle_DAC();

public:
    double measure; ///< Stores the measure (area) of the set of rectangles in input
    double contour; ///< Stores the contour (perimeter length) of the set of rectangles in input.
    void read();
    void compute();
};