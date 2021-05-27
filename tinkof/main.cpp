#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#define pb push_back
/*
  Задача "Минимальное покрытие"
Среди заданного множества отрезков прямой с целочисленными координатами концов [Li, Ri] необходимо выбрать подмножество наименьшей мощности,
целиком покрывающее отрезок [0, M], где M – целое положительное число.

  Исходные данные
В первой строке записано целое число M (1 ≤ M ≤ 5000). В последующих строках перечислены пары целых чисел Li и Ri (−50000 ≤ Li < Ri ≤ 50000),
каждая пара с новой строки, числа в парах отделены друг от друга одним пробелом.
В последней строке записана пара нулей. Множество содержит не менее одного и не более 99999 отрезков.

  Результат
Программа должна формировать в первой строке требуемое минимальное число отрезков из исходного множества, необходимое для покрытия отрезка [0, M].
Далее должен следовать список покрывающего подмножества, упорядоченный по возрастанию координат левых концов отрезков.
Список отрезков выводится в том же формате, что и во входe, завершающую пару нулей выводить не следует.
Если покрытие отрезка [0, M] исходным множеством отрезков [Li, Ri] невозможно, то следует вывести -1.
*/
using namespace std;
const long long N = 100000;

pair<long long, long long> dp[N], right_end[N];
vector<pair<long long, long long> > v;
vector<long long> ans;
int main() {
    for(long long i = 0; i < N; i++)
    {
        right_end[i].first = -50001;
        right_end[i].second = -50001;
    }
    long long m;
    cin >> m;
    long long l, r;
    long long k = 0;
    while(true)
    {
        cin >> l >> r;
        if (r<0 || l>m) continue;
        if(l == 0 && r == 0) break;
        if (l < 0) l=0;

        v.pb({l, r});

        if(r > right_end[l].first)
        {
            right_end[l].first = r;
            right_end[l].second = k;
        }
        k++;
    }

    long long i = 0;
    for(auto it: v)
    {
        if(it.first <= 0 && it.second > 0)
        {
            if(it.second > dp[0].first)
            {
                dp[0].first = it.second;
                dp[0].second = i;
            }
        }
        i++;
    }
    for(long long i = 1; i <= m; i++)
    {
        if(dp[i - 1].first >= right_end[i].first)
        {
            dp[i].first = dp[i - 1].first;
            dp[i].second = dp[i - 1].second;
        } else
        {
            dp[i].first = right_end[i].first;
            dp[i].second = right_end[i].second;
        }
    }
    for(long long i = 0; i <= m; i++)
    {
        if(dp[i].first < i)
        {
            cout << -1;
            return 0;
        }
    }
    long long pos = 0;
    while(pos < m)
    {
        ans.pb(dp[pos].second);
        if(dp[pos].first == pos)
        {
            cout << -1;
            return 0;
        }
        pos = dp[pos].first;
    }
    cout << ans.size();
    for(auto it: ans)
    {
        cout << v[(int)it].first << " " << v[(int)it].second << endl;
    }
    return 0;
}
