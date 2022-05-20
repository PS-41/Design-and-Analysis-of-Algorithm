#include <set>
#include <tuple>
#include <algorithm>
#include <fstream>
#include "guting.hpp"
#include "io.hpp"

/**
 * Calculates the partition of y-projections contained in vy.
 * @param[in] vy indicates the y-projections of endpoints of edges.
 * @returns the vector of interval denoting the partition of vy.
 */
std::vector<Guting::Interval> Guting::partitions(std::vector<double>& vy)
{
    std::vector<Interval> out;
    int n = int(vy.size());
    for(int i = 1; i < n; ++i)
        out.emplace_back(Interval(vy.at(i-1), vy.at(i)));
    return out;
}

/**
 * y partition of stripes in s is made equal to the y partitions in p.
 * @param[in] s indicates the vector of stripes whose y partition is
 * to be updated.
 * @param[in] p denotes y projections of the frame.
 * @param[in] frame indicates the entire frame of the merge step.
 * @returns the vector of stripe after copying.
 */
std::vector<Guting::Stripe> Guting::copy(
    std::vector<Stripe>& s,
    std::vector<double>& p,
    Interval frame
)
{
    std::vector<Stripe> sc;
    for(Interval intr : partitions(p))
        sc.emplace_back(Stripe(frame, intr, 0, new CTree()));
    auto it = s.begin();
    auto jt = sc.begin();
    while(it != s.end() and jt != sc.end())
    {
        if(jt->y_intr.inside(it->y_intr))
            jt->measure = it->measure, jt->ctree = it->ctree, ++jt;
        else
            ++it;
    }
    return sc;
}

/**
 * Blackens the stripes vs based on the edges in ve, meaning their
 * x union fields are updated to indicate that they are completely 
 * covered by a rectangle.
 * @param[in] vs indicates the vector of stripes to be blackened.
 * @param[in] ve indicates the vector of edges based on which we blacken vs.
 */
void Guting::blacken(std::vector<Stripe>& vs, std::vector<Edge>& ve)
{
    int n = int(vs.size());
    std::vector<std::tuple<Interval, int>> vii;
    std::vector<Interval> vi;
    for(int i = 0; i < n; i++)
        vii.push_back({vs.at(i).y_intr, i});
    for(Edge e : ve)
        vi.emplace_back(e.intr);
    std::sort(vii.begin(), vii.end());
    std::sort(vi.begin(), vi.end());
    std::set<double> ss;
    auto it = vi.begin();
    for(auto [intr, ia] : vii)
    {
        while(it != vi.end() and it->bot <= intr.bot)
            ss.insert(it->top), ++it;
        auto jt = ss.lower_bound(intr.top);
        if(jt != ss.end())
            vs.at(ia).measure = vs.at(ia).x_intr.width(), vs.at(ia).ctree = new CTree();
    }
}

/**
 * Concatenates the two adjacent stripes sl and sr
 * to form the stripes S of the merged frame.
 * @param[in] sl indicates the stripes of the left frame.
 * @param[in] sr indicates the stripes of the right frame.
 * @param[in] p indicates the y-projections of all endpoints
 * of vertical edges in the entire frame.
 * @param[in] frame indicates the x-interval of the entire frame,
 * that is the left and right frame.
 * @return the concatenated stripe of the merged frame.
 */
std::vector<Guting::Stripe> Guting::concat(
    std::vector<Stripe>& sl,
    std::vector<Stripe>& sr,
    std::vector<double>& p,
    Interval frame
)
{
    std::vector<Stripe> out;
    for(Interval intr : partitions(p))
        out.emplace_back(Stripe(frame, intr, 0, new CTree()));
    for(int i = 0; i < int(sl.size()); ++i)
    {
        auto s1 = sl.at(i), s2 = sr.at(i);
        auto& sout = out.at(i);
        sout.measure = s1.measure + s2.measure;
        if(s1.ctree and s2.ctree)
            sout.ctree = new CTree(s1.x_intr.top, RT, s1.ctree, s2.ctree);
        else if(s1.ctree and not s2.ctree)
            sout.ctree = s1.ctree;
        else if(not s1.ctree and s2.ctree)
            sout.ctree = s2.ctree;
        else
            sout.ctree = new CTree();
    }
    return out;
}

/**
 * The merge step of divide and conquer algorithm. It merges the
 * two frames and gives the L, R, P and S value for the combined frame.
 * @param[in] in1 indicates the L, R, P, S value of the left frame.
 * @param[in] in2 indicates the L, R, P, S value of the right frame.
 * @param[in] frame indicates the x-interval of the merged frame.
 * @param[in] xm indicates the median value of the x-coordinates of 
 * the edges of the frame.
 * @return a tuple of L, R, P and S for the merged frame.
 */
Guting::LRPS Guting::merge(LRPS in1, LRPS in2, Interval frame, double xm)
{
    auto& [l1, r1, p1, s1] = in1;
    auto& [l2, r2, p2, s2] = in2;
    LRPS out;
    auto& [l, r, p, s] = out;
    std::vector<Edge> l1c, r2c;

    for(Edge e : l1)
        if (e.xp > frame.top)
            l1c.emplace_back(e);
    l.insert(l.end(), l1c.begin(), l1c.end());
    l.insert(l.end(), l2.begin(), l2.end());

    for(Edge e : r2)
        if (e.xp < frame.bot)
            r2c.emplace_back(e);
    r.insert(r.end(), r1.begin(), r1.end());
    r.insert(r.end(), r2c.begin(), r2c.end());

    std::set_union(p1.begin(), p1.end(), p2.begin(), p2.end(), std::back_inserter(p));
    std::vector<Stripe> sl = copy(s1, p, Interval(frame.bot, xm));
    std::vector<Stripe> sr = copy(s2, p, Interval(xm, frame.top));

    blacken(sl, r2c);
    blacken(sr, l1c);
    s = concat(sl, sr, p, frame);

    return out;
}

/**
 * This is the main divide and conquer algorithm using recursion.
 * @param[in] ve represents the set of vertical edges of the rectangles.
 * @param[in] frame represents the current x interval we are considering,
 * it encloses the x-coordinates of all the edges in ve.
 * @return L, R, P and S for the current frame.
 * L contains those left edges of ve whose partner is not in frame.
 * R is symmetric to L (for right edges).
 * P contains the y-projections of all endpoints of edges in V plus the
 * frame boundary in y-direction, namely -INF and INF.
 * S is the set of Stripes that will be required to calculate
 * measure and contour.
 */
Guting::LRPS Guting::stripes(std::vector<Edge> ve, Interval frame)
{
    LRPS out;
    auto& [l, r, p, s] = out;
    if(ve.size() == 1)
    {
        Edge e = ve.front();
        if(e.side == LEFT)
            l = std::vector<Edge>(1, e);
        else
            r = std::vector<Edge>(1, e);
        p = {-INF, e.intr.bot, e.intr.top, INF};
        for(Interval intr : partitions(p))
        {
            if(intr == e.intr)
            {
                if(e.side == LEFT)
                    s.emplace_back(Stripe(frame, intr, frame.top - e.xc, new CTree(e.xc, LC)));
                else
                    s.emplace_back(Stripe(frame, intr, e.xc - frame.bot, new CTree(e.xc, RC)));
            }
            else
                s.emplace_back(Stripe(frame, intr, 0, new CTree()));
        }
    }
    else
    {
        int n = int(ve.size());
        int mid = n/2 - 1, median;
        auto& midedge = ve.at(mid);
        if(ve.front().xc == ve.back().xc or ve.at(mid).xc != ve.at(mid+1).xc)
            median = mid+1;
        else
        {
            int c1 = 0;
            for(int i = mid; i >= 1; --i)
            {
                if(ve.at(i).xc == midedge.xc)
                    ++c1;
                else
                    break;
            }
            int c2 = 0;
            for(int i = mid+1; i+1 < n; ++i)
            {
                if(ve.at(i).xc == midedge.xc)
                    ++c2;
                else
                    break;
            }
            int t1 = mid + c2 + 1;
            int t2 = n - t1;
            int t3 = mid - c1 + 1;
            int t4 = n - t3;
            int ch1 = abs(t1 - t2);
            int ch2 = abs(t3 - t4);
            if(ve.back().xc == midedge.xc)
                median = t3;
            else if(ve.front().xc == midedge.xc)
                median = t1;
            else if(ch1 <= ch2)
                median = t1;
            else
                median = t3;
        }
        std::vector<Edge> v1(ve.begin(), ve.begin() + median);
        std::vector<Edge> v2(ve.begin() + median, ve.end());
        double xm = (v1.back().xc + v2.front().xc)/2;
        out = merge(
            stripes(v1, Interval(frame.bot, xm)),
            stripes(v2, Interval(xm, frame.top)),
            frame,
            xm
        );
    }
    return out;
}

/**
 * Calculates the measure of the given set of rectangles
 * using the set of stripes that we got using divide and conquer
 * algorithm.
 * @param[in] stripes is the set of stripes obtained through divide and conquer.
 * @return measure of the rectangles.
 */
double Guting::find_measure(std::vector<Stripe>& stripes)
{
    double out = 0;
    for(Stripe stripe : stripes)
        if(stripe.measure)
            out += stripe.measure * stripe.y_intr.width();
    return out;
}

/**
 * A utility DFS function for use by construct_leaf(). Stores
 * the leaf of the tree of a given stripe.
 * @param[in] tree on which we have to perform DFS.
 */
void Guting::dfs(CTree *tree)
{
    if(not tree)
        return;
    if(tree->parity != RT)
        leaf.push_back(tree->x);
    CTree *ldaughter = tree->lson, *rdaughter = tree->rson;
    dfs(ldaughter);
    dfs(rdaughter);
}

/**
 * Constructs the leaf nodes for all the stripes.
 * @param[in] strips indicate the set of all the stripes obtained
 * using our divide and conquer algorithm.
 */
void Guting::construct_leaf(std::vector<Stripe>& strips)
{
    for(Stripe& strp: strips)
    {
        leaf.clear();
        leaf.emplace_back(-INF);
        dfs(strp.ctree);
        leaf.emplace_back(INF);
        strp.leaf = leaf;
    }
}

/**
 * Answers the free subinterval query, that is subinterval of xi that are
 * free with respect to leaf.
 * @param[in] xi indicates the interval for which we have to answer the query.
 * @param[in] leaf indicates the leaf nodes of stripe used to answer find the
 * free subinterval.
 * @param[in] ycoord indicates the y-coordinate of the interval.
 * @param[in] flag indicates whether the interval for which we are answering
 * the query belongs to a bottom edge or a top edge of the rectangle.
 */
void Guting::query(Interval xi, std::vector<double>& lf, double ycoord, bool flag)
{
    int n = int(lf.size());
    int idx = int(lower_bound(lf.begin(), lf.end(), xi.bot) - lf.begin());   
    if(idx % 2)
        idx--;
    for(int i = idx; i < n; i += 2)
    {
        double& cur = lf.at(i);
        double& nxt = lf.at(i+1);
        if (not(idx % 2) and (cur >= xi.top))
            break;
        if(cur < nxt)
        {
            if(nxt <= xi.bot or cur >= xi.top)
                continue;
            cont.push_back({ycoord, std::max(cur, xi.bot), std::min(nxt, xi.top), flag});
        }
    }
}

/**
 * Calculates the contour of the given set of rectangles
 * using the set of stripes that we got using divide and conquer
 * algorithm.
 * @param[in] strips is the set of stripes obtained through divide and conquer.
 * @param[in] hrx indicates the set of horizontal rectangle edges.
 * @return length of the contour.
 */
double Guting::find_contour(std::vector<Stripe>& strips, std::vector<Edge>& hrx)
{
    construct_leaf(strips);
    double out = 0;
    auto it = hrx.begin();
    auto jt = strips.begin();
    while(it != hrx.end() and jt != strips.end())
    {
        if(it->side == BOT)
        {
            if(jt->y_intr.top < it->xc)
                ++jt;
            else if(jt->y_intr.top == it->xc)
                query(it->intr, jt->leaf, it->xc, 0), ++it;
            else
                ++it;
        }
        else
        {
            if(jt->y_intr.bot < it->xc)
                ++jt;
            else if(jt->y_intr.bot == it->xc)
                query(it->intr, jt->leaf, it->xc, 1), ++it;
            else
                ++it;
        }
    }

    std::ofstream file;
    file.open("..\\output\\contour_edges.txt");

    int n = int(cont.size());
    std::sort(cont.begin(), cont.end());
    std::vector<std::tuple<double, double>> ep;
    
    auto& [y1, xl1, xr1, f1] = cont.front();
    for(auto& [y2, xl2, xr2, f2] : cont)
    {
        if(y2 == y1 and f2 == f1 and xr1 >= xl2)
        {
            xr1 = std::max(xr1, xr2);
            continue;
        }
        file << xl1 << ' ' << y1 << ' ' << xr1 - xl1 << ' ' << 0 << '\n';
        ep.push_back({xl1, y1});
        ep.push_back({xr1, y1});
        out += (xr1 - xl1);
        y1 = y2, xl1 = xl2, xr1 = xr2, f1 = f2;
    }

    file << xl1 << ' ' << y1 << ' ' << xr1 - xl1 << ' ' << 0 << '\n';
    ep.push_back({xl1, y1});
    ep.push_back({xr1, y1});
    out += (xr1 - xl1);
    
    std::sort(ep.begin(), ep.end());
    n = int(ep.size());
    for(int i = 0; i+1 < n; ++i)
    {
        auto [cx, cy] = ep.at(i);
        auto [nx, ny] = ep.at(i+1);
        if(cx == nx)
        {
            if(cy == ny)
                continue;
            else
            {
                out += (ny - cy);
                file << cx << ' ' << cy << ' ' << 0 << ' ' << ny - cy << '\n';
                ++i;
            }
        }
    }
    file.close();
    return out;
}

/**
 * Provides an algorithm for the recursive divide and conquer
 * algorithm that is stripes.
 * @return Measure and Contour of the set of rectangles.
 */
std::tuple<double, double> Guting::rectangle_DAC()
{
    std::vector<Edge> vrx;
    for(Rectangle rect : rects)
    {
        vrx.emplace_back(
            Edge(Interval(rect.y_bot, rect.y_top), rect.x_left, rect.x_right, LEFT)
        );
        vrx.emplace_back(
            Edge(Interval(rect.y_bot, rect.y_top), rect.x_right, rect.x_left, RIGHT)
        );
    }
    std::sort(vrx.begin(), vrx.end());
    std::vector<Edge> hrx;
    for(Rectangle rect : rects)
    {
        hrx.emplace_back(
            Edge(Interval(rect.x_left, rect.x_right), rect.y_bot, rect.y_top, BOT)
        );
        hrx.emplace_back(
            Edge(Interval(rect.x_left, rect.x_right), rect.y_top, rect.y_bot, TOP)
        );
    }
    std::sort(hrx.begin(), hrx.end());
    std::vector<Stripe> strips = std::get<3>(stripes(vrx, Interval(-INF, INF)));
    
    return {find_measure(strips), find_contour(strips, hrx)};
}

/**
 * Reads a set of rectangles using x_left, x_right, y_bot and y_top.
 * Constructs the list #rects storing the rectangles.
 */
void Guting::read()
{
    std::ofstream file;
    file.open("..\\output\\rectangles.txt");
    int n;
    Input in;
    in >> n;
    while(n--)
    {
        double xl, xr, yb, yt;
        in >> xl >> xr >> yb >> yt;
        if(xl == xr or yb == yt)
            continue;
        if(xl > xr)
            std::swap(xl, xr);
        if(yb > yt)
            std::swap(yb, yt);
        file << xl << ' ' << xr << ' ' << yb << ' ' << yt << '\n';
        rects.emplace_back(Rectangle(xl, xr, yb, yt));
    }
    file.close();
}

/**
 * Conputes the measure and contour using the divide and conquer
 * algorithm, and stores it in measure and contour variable respectively.
 */
void Guting::compute()
{
    std::tie(measure, contour) = rectangle_DAC();
    std::ofstream file;
    file.open("..\\output\\result.txt");
    file << measure << '\n';
    file << contour << '\n';
    file.close();
}