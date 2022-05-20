// Author : ps41

#include <bits/stdc++.h>
using namespace std;
#define rn dist(rng)
#define rn2 dist2(rng)
#define rn3 dist3(rng)
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<long> dist(0, 45);
uniform_int_distribution<long> dist2(0, 20);
uniform_int_distribution<long> dist3(-10, 10);

/**
 * Generates test cases to be used by the algorithms
 * Takes the number of points n, cost value and required number of segments
 * as input from the user.
 * Writes the generated test case in the in.txt file.
 * n random points are taken on the x-axis.
 * We take first n / segments points and lift them by y = mx + c
 * where m and c are random values for this set of points.
 * Similarly, we lift the other sets of points.
 */
int main()
{
    cout << "Enter n, cost value and number of segments rspectively\n";
    std::ofstream file;
    file.open("..\\input\\in.txt");
    int n, segments;
    double C; 
    cin >> n >> C >> segments;
    file << n << '\n';
    file << C << '\n';

    set<int> s;
    while (s.size() < n) s.insert(rn);

    vector<double> x, y;
    for (auto i: s) x.push_back(i);

    int num = n / segments;

    vector<pair<double, double>> mc(segments);

    vector<int> t1(segments), t2(segments);

    for (int i = 0; i < segments; i++) 
        t1[i] = rn2, t2[i] = rn2;

    sort(t1.begin(), t1.end());
    sort(t2.begin(), t2.end());

    for (int i = 0; i < segments; i++) mc[i] = {t1[i], t2[i]};

    int idx = 0;
    double div = 10.0;
    for (int i = 0; i < segments - 1; i++)
    {
        double m = mc[i].first, c = mc[i].second;
        for (int j = 0; j < num; j++) 
            y.push_back(m * x[idx++] + c + rn3 / div);
    }
    
    double m = mc[segments - 1].first, c = mc[segments - 1].second;
    while(idx < n)
    {
        y.push_back(m * x[idx++] + c + rn3 / div);
    }

    for (int i = 0; i < n; i++) file << x[i] << ' ' << y[i] << '\n';

    file.close();
}