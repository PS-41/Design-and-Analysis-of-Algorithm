#include <bits/stdc++.h>
using namespace std;

#define rn dist(rng)
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<long> dist(-10, 10);

int main()
{
    std::ofstream file;
    file.open("in.txt");
    int n;
    cin >> n;
    file << n << '\n';
    for (int i = 0; i < n; i++)
    {
        unordered_set<int> s;
        while (s.size() < 4) s.insert(rn);
        set<int> s1;
        auto it = s.begin();
        s1.insert(*it);
        it++;
        s1.insert(*it);
        set<int> s2;
        it++;
        s2.insert(*it);
        it++;
        s2.insert(*it);
        for (auto i: s1) file << i << ' ';
        for (auto i: s2) file << i << ' ';
        file << '\n';
    }
    file.close();
}