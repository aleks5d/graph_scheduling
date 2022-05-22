#include <bits/stdc++.h>

using namespace std;

int get_rand(int l, int r, int w = 0) {
	int d = r - l + 1;
	int ans = rand() % d;
	for (int i = 0; i < w; ++i) {
		ans = max(ans, rand() % d);
	}
	for (int i = 0; i > w; --i) {
		ans = min(ans, rand() % d);
	}
	return l + ans;
}

int get_srand(int argc, char **argv) {
	int mod = 1e9 + 7;
	int p = 37;
	long long int hash = 0;
	for (int i = 0; i < argc; ++i) {
		for (int j = 0; j < sizeof(argv[i]); ++j) {
			hash = hash * p + argv[i][j];
			hash %= mod;
		}
	}
	return hash;
}

void print(int size, int proc, vector<vector<int>> data, vector<vector<int>> W, vector<vector<int>> B, vector<int> L) {
	cout << size << " " << proc << '\n';
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			cout << data[i][j] << " ";
		}
		cout << '\n';
	}
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < size; ++j) {
			cout << W[i][j] << " ";
		}
		cout << '\n';
	}
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < proc; ++j) {
			cout << B[i][j] << " ";
		}
		cout << '\n';
	}
	for (int i = 0; i < proc; ++i) {
		cout << L[i] << " ";
	}
	cout << '\n';
}

void gen0(int argc, char **argv) {
	if (argc < 11) {
		cout << "Not enough params\n";
		cout << "size, lexical number, proc, data_min, data_max, W_min, W_max, B_min, B_max, L_min, L_max\n";
		exit(1);
	}

	auto get_int = [&]() mutable {
		int x = atoi(argv[0]);
		++argv;
		return x;
	};

	int size = get_int();
	int num = get_int();
	int proc = get_int();
	int data_min = get_int();
	int data_max = get_int();
	int W_min = get_int();
	int W_max = get_int();
	int B_min = get_int();
	int B_max = get_int();
	int L_min = get_int();
	int L_max = get_int();
	if (size <= 0 || size >= 7) {
		cout << "size must be in [1, 7]\n";
		exit(1);
	}
	int cnt_edges = size * (size - 1) / 2;
	if (num < 0 || num >= (1 << cnt_edges)) {
		cout << "lexical number must be in [0, 1<<cnt_edges)\n";
		cout << "cnt_edges = size * (size - 1) / 2\n";
		exit(1);
	}
	if (data_min > data_max) {
		cout << "data_min must be not greater than data_max\n";
		exit(1);
	}
	if (W_min > W_max) {
		cout << "W_min must be not greater than W_max\n";
		exit(1);
	}
	if (B_min > B_max) {
		cout << "B_min must be not greater than B_max\n";
		exit(1);
	}
	if (L_min > L_max) {
		cout << "L_min must be not greater than L_max\n";
		exit(1);
	}

	// 0 1, 0 2, ... 0 size-1, 1 2, ... 1 size-1, ... size-2 size-1
	vector<vector<int>> data(size, vector<int>(size));
	int from = 0, to = 1;
	for (int i = 0; i < cnt_edges; ++i) {
		if (num & (1 << i)) {
			data[from][to] = get_rand(data_min, data_max);
		}
		++to;
		if (to == size) {
			++from;
			to = from+1;
		}
	}

	vector<vector<int>> W(proc, vector<int>(size));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < size; ++j) {
			W[i][j] = get_rand(W_min, W_max);
		}
	}

	vector<vector<int>> B(proc, vector<int>(proc));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < proc; ++j) {
			B[i][j] = get_rand(B_min, B_max);
		}
	}

	vector<int> L(proc);
	for (int i = 0; i < proc; ++i) {
		L[i] = get_rand(L_min, L_max);
	}

	print(size, proc, data, W, B, L);
}

void gen1(int argc, char **argv) {
	if (argc < 12) {
		cout << "Not enough params\n";
		cout << "size, cnt_rerand, edge_rerand, proc, data_min, data_max, W_min, W_max, B_min, B_max, L_min, L_max\n";
		exit(1);
	}

	auto get_int = [&]() mutable {
		int x = atoi(argv[0]);
		++argv;
		return x;
	};

	int size = get_int();
	int cnt_rerand = get_int();
	int edge_rerand = get_int();
	int proc = get_int();
	int data_min = get_int();
	int data_max = get_int();
	int W_min = get_int();
	int W_max = get_int();
	int B_min = get_int();
	int B_max = get_int();
	int L_min = get_int();
	int L_max = get_int();

	assert(size > 0 && size < 1000);
	assert(proc > 0 && proc < 1000);
	assert(data_min <= data_max);
	assert(W_min <= W_max);
	assert(B_min <= B_max);
	assert(L_min <= L_max);

	vector<vector<int>> data(size, vector<int>(size));
	for (int i = 1; i < size; ++i) {
		int edges = get_rand(0, i, cnt_rerand);
		set<int> can;
		for (int j = 0; j < i; ++j) can.insert(j);
		for (int j = 0; j < edges; ++j) {
			int num = get_rand(0, can.size() - 1, edge_rerand);
			auto v = can.begin();
			for (int k = 0; k < num; ++k) v++;
			data[*v][i] = get_rand(data_min, data_max);
			can.erase(v);
		}
	}

	vector<vector<int>> W(proc, vector<int>(size));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < size; ++j) {
			W[i][j] = get_rand(W_min, W_max);
		}
	}

	vector<vector<int>> B(proc, vector<int>(proc));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < proc; ++j) {
			B[i][j] = get_rand(B_min, B_max);
		}
	}

	vector<int> L(proc);
	for (int i = 0; i < proc; ++i) {
		L[i] = get_rand(L_min, L_max);
	}

	print(size, proc, data, W, B, L);
}

void gen2(int argc, char **argv) {

	auto get_int = [&]() mutable {
		int x = atoi(argv[0]);
		++argv;
		return x;
	};

	int size = get_rand(1, 10);
	int cnt_rerand = get_rand(-2, 2);
	int edge_rerand = get_rand(-2, 2);
	int proc = get_rand(1, 10);
	int data_min = get_rand(1, 10);
	int data_max = data_min + get_rand(0, 10);
	int W_min = get_rand(1, 10);
	int W_max = W_min + get_rand(1, 10);
	int B_min = get_rand(1, 10);
	int B_max = B_min + get_rand(1, 10);
	int L_min = get_rand(1, 10);
	int L_max = L_min + get_rand(1, 10);

	vector<vector<int>> data(size, vector<int>(size));
	for (int i = 1; i < size; ++i) {
		int edges = get_rand(0, i, cnt_rerand);
		set<int> can;
		for (int j = 0; j < i; ++j) can.insert(j);
		for (int j = 0; j < edges; ++j) {
			int num = get_rand(0, can.size() - 1, edge_rerand);
			auto v = can.begin();
			for (int k = 0; k < num; ++k) v++;
			data[*v][i] = get_rand(data_min, data_max);
			can.erase(v);
		}
	}

	vector<vector<int>> W(proc, vector<int>(size));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < size; ++j) {
			W[i][j] = get_rand(W_min, W_max);
		}
	}

	vector<vector<int>> B(proc, vector<int>(proc));
	for (int i = 0; i < proc; ++i) {
		for (int j = 0; j < proc; ++j) {
			B[i][j] = get_rand(B_min, B_max);
		}
	}

	vector<int> L(proc);
	for (int i = 0; i < proc; ++i) {
		L[i] = get_rand(L_min, L_max);
	}

	print(size, proc, data, W, B, L);
}

vector<void (*)(int, char**)> gen = {gen0, gen1, gen2};

int main(int argc, char **argv) {
	srand(get_srand(argc, argv));
	if (argc < 2) {
		cout << "No generator type\n";
		exit(1);
	}
	int type = atoi(argv[1]);
	if (type < 0 || type >= gen.size()) {
		cout << "No such generator\n";
		exit(1);
	}
	(*gen[type])(argc-2, argv+2);
}