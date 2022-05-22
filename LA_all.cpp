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

#define ff first
#define ss second

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
		double w = 0;
		for (int j = 0; j < proc; ++j) {
			w += W[j][i];
		}
		w /= proc;
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

	auto HEFT = [&]() {
		auto res = result;
		for (int i = 0; i < size; ++i) {
			int v = sorted[i].second;
			if (res[v].first != -1) continue;
			for (int p = 0; p < proc; ++p) {
				double start_time = 0;
				for (int j = 0; j < result.size(); ++j) {
					if (res[j].first == p) {
						start_time = max(start_time, res[j].second + W[p][j]);
					} else if (data[j][v] > 0) {
						start_time = max(start_time,
							res[j].second + W[res[j].first][j] /* time of stop*/ + L[res[j].first] + (double)data[j][v] / B[res[j].first][p] /*time of data move*/
						);
					}
				}
				if (res[v].first == -1 || res[v].second + W[res[v].first][v] > start_time + W[p][v]) {
					res[v] = {p, start_time};
				}
			}
		}

		double max_time = 0;
		for (int i = 0; i < size; ++i) {
			max_time = max(max_time,
				res[i].second + W[res[i].first][i]
			);
		}

		return max_time;
	};


	for (int i = 0; i < size; ++i) {
		int v_res = -1;
		pair<double, int> res = {1e18, -1};
		for (int v = 0; v < size; ++v) {
			if (result[v].ff != -1) continue; // this task already scheduled
			bool is_good = true;
			for (int j = 0; j < size; ++j) {
				if (data[j][v] && result[j].ff == -1) {
					is_good = false;
					break;
				}
			}
			if (!is_good) continue;
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
				result[v] = {p, start_time};
				pair<double, int> cur_res = {HEFT(), p};
				result[v] = {-1, -1};
				if (cur_res.ff < res.ff) {
					res = cur_res;
					v_res = v;
				}
			}
		}
		int v = v_res;
		int p = res.second;
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
		result[v] = {p, start_time};
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