#include <bits/stdc++.h>

using namespace std;

template<typename T1>
std::istream& operator>>(std::istream& in, std::vector<T1>& x) {
    for (auto& i : x) in >> i;
    return in;
}

template<typename T1>
std::ostream& operator<<(std::ostream& out, std::vector<T1>& x) {
    for (auto& i : x) out << i << " ";
    return out;
}


template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, std::pair<T1, T2>& x) {
    out << x.first << " " << x.second;
    return out;
}

int main() {
	int size, proc;
	cin >> size >> proc;
	vector<vector<int>> data(size, vector<int>(size));
	vector<vector<int>> W(proc, vector<int>(size));
	vector<vector<int>> B(proc, vector<int>(proc));
	vector<int> L(proc);
	cin >> data >> W >> B >> L;
	vector<double> rank(size);
	for (int i = size - 1; i >= 0; --i) {
		vector<int> ww;
		double w = 0;
		for (int j = 0; j < proc; ++j) {
			ww.push_back(W[j][i]);
		}
		sort(ww.begin(), ww.end());
		w = ww[ww.size() / 2];
		double k = 0;
		for (int to = i + 1; to < size; ++to) {
			if (data[i][to] == 0) continue;
			double c = 0;
			for (int pf = 0; pf < proc; ++pf) {
				for (int pt = 0; pt < proc; ++pt) {
					if (pf == pt) continue;
					c += (double)data[i][to] / B[pf][pt] + L[pf];
				}
			}
			c /= proc;
			c /= proc;
			k = max(k, c + rank[to]);
		}
		rank[i] = w + k;
	}
	vector<pair<double, int>> sorted;
	for (int i = 0; i < size; ++i) {
		sorted.emplace_back(rank[i], i);
	}
	sort(sorted.begin(), sorted.end());
	reverse(sorted.begin(), sorted.end());
	vector<pair<int, double>> result(size, {-1, -1}); // на каком проце и во сколько начнет исполнение задача i
	for (int i = 0; i < size; ++i) {
		int v = sorted[i].second;
		for (int p = 0; p < proc; ++p) {
			double start_time = 0;
			for (int j = 0; j < result.size(); ++j) {
				if (result[j].first == p) {
					start_time = max(start_time, result[j].second + W[p][j]);
				} else if (data[j][v] > 0) {
					start_time = max(start_time,
						result[j].second + W[result[j].first][j] /* time of stop*/ + L[result[j].first] + (double)data[j][v] / B[result[j].first][p] /*time of data move*/
					);
				}
			}
			if (result[v].first == -1 || result[v].second + W[result[v].first][v] > start_time + W[p][v]) {
				result[v] = {p, start_time};
			}
		}
	}

	double max_time = 0;
	for (int i = 0; i < size; ++i) {
		max_time = max(max_time,
			result[i].second + W[result[i].first][i]
		);
	}
	cout << max_time << '\n';
	for (int i = 0; i < size; ++i) {
		cout << "vertex " << i << " starts on proc " << result[i].first << " in time " << result[i].second << '\n';
	}
}

/*
2 2 

0 1
0 0

1 10
1 1

1 2
2 1

0 0
*/