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

	vector<pair<int, double>> result(size, {-1, -1});
	vector<pair<int, double>> global(size, {-1, -1});
	double res = 1e18;

	vector<int> perm(size);
	for (int i = 0; i < size; ++i) {
		perm[i] = i;
	}

	auto next_proc = [&](vector<int>& p) {
		for (int i = p.size() - 1; i >= 0; --i) {
			if (p[i] + 1 < proc) {
				++p[i];
				return true;
			}
			p[i] = 0;
		}
		return false;
	};

	do {
		vector<int> pos(size);
		for (int i = 0; i < size; ++i) {
			pos[perm[i]] = i;
		}
		bool good = true;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (data[i][j]) {
					if (pos[j] < pos[i]) good = false;
				}
			}
		}
		if (!good) continue;
		vector<int> p(size);
		for (int i = 0; i < size; ++i) {
			p[i] = 0;
		}
		do {
			result.assign(size, {-1, -1});
			// calc res
			for (int v: perm) {
				double est = 0;
				for (int i = 0; i < size; ++i) {
					if (result[i].first == p[v]) {
						est = max(est, result[i].second + W[result[i].first][i]);
					}
					if (data[i][v] == 0) continue;
					double res = result[i].second + W[result[i].first][i];
					if (result[i].first != p[v]) {
						res += L[result[i].first] + (double)data[i][v] / B[result[i].first][p[v]];
					}
					est = max(est, res);
				}
				result[v] = {p[v], est};
			}

			double max_time = 0;
			for (int i = 0; i < size; ++i) {
				max_time = max(max_time, result[i].second + W[result[i].first][i]);
			}
			if (max_time < res) {
				res = max_time;
				global = result;
			}

		} while (next_proc(p));
	} while (next_permutation(perm.begin(), perm.end()));

	result = global;

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

=========

2 2

0 0
0 0

1 10
1 1

1 2
2 1

0 0 

========
2 3

0 3 
0 0 

7 2 
3 10 
4 7 

6 4 7 
6 7 2 
10 3 9 

3 5 1 


*/