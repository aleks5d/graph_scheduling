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
	
	vector<double> sl(size);
	vector<double> med(size);

	for (int i = 0; i < size; ++i) {
		vector<int> times;
		for (int j = 0; j < proc; ++j) {
			times.push_back(W[j][i]);
		}
		sort(times.begin(), times.end());
		med[i] = times[proc/2];
	}

	//calc static levels:
	for (int i = size - 1; i >= 0; --i) {
		sl[i] = med[i];
		for (int to = i + 1; to < size; ++to) {
			if (data[i][to]) // есть зависимость
				sl[i] = max(sl[i], med[i] + sl[to]);
		}
	}

	vector<pair<int, double>> result(size, {-1, -1}); // на каком проце и во сколько начинает

	for (int step = 0; step < size; ++step) {
		vector<int> unscheduled;
		for (int i = 0; i < size; ++i) {
			if (result[i].ff == -1) unscheduled.push_back(i);
		}
		vector<pair<double, pair<int, int>>> DL;
		for (int v : unscheduled) {
			for (int p = 0; p < proc; ++p) {
				double dl = 0;
				dl += sl[v];
				dl += W[p][v] - med[v];
				// EST calc
				double est = 0;
				for (int i = 0; i < size; ++i) {
					if (result[i].ff == p) {
						est = max(est, result[i].ss + W[result[i].ff][i]);
					}
					if (data[i][v] == 0) continue;
					if (result[i].ff == -1) { // задача от которой мы зависимы еще не зашедулена
						est = -1;
						break;
					}
					double res = result[i].ss + W[result[i].ff][i];
					if (result[i].ff != p) {
						res += L[result[i].ff] + (double)data[i][v] / B[result[i].ff][p];
					}
					est = max(est, res);
				}
				if (est == -1) continue;
				dl -= est;
				DL.push_back({dl, {v, p}});
			}
		}
		sort(DL.begin(), DL.end());
		int v = DL.back().ss.ff;
		int p = DL.back().ss.ss;
		double est = 0;
		for (int i = 0; i < size; ++i) {
			if (result[i].ff == p) {
				est = max(est, result[i].ss + W[result[i].ff][i]);
			}
			if (data[i][v] == 0) continue;
			double res = result[i].ss + W[result[i].ff][i];
			if (result[i].ff != p) {
				res += L[result[i].ff] + (double)data[i][v] / B[result[i].ff][p];
			}
			est = max(est, res);
		}
		result[v] = {p, est};
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