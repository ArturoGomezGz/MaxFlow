#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int rev;
    long long cap;
};

struct FordFulkerson {
    int n, s, t;
    vector<vector<Edge>> adj;
    vector<int> visited;

    FordFulkerson(int n, int s, int t) : n(n), s(s), t(t), adj(n), visited(n) {}

    void add_edge(int u, int v, long long cap) {
        Edge a = {v, (int)adj[v].size(), cap};
        Edge b = {u, (int)adj[u].size(), 0};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    long long dfs(int u, long long flow) {
        if (u == t) return flow;
        visited[u] = 1;
        for (auto &e : adj[u]) {
            if (!visited[e.to] && e.cap > 0) {
                long long pushed = dfs(e.to, min(flow, e.cap));
                if (pushed > 0) {
                    e.cap -= pushed;
                    adj[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    long long maxflow() {
        long long flow = 0, pushed;
        do {
            fill(visited.begin(), visited.end(), 0);
            pushed = dfs(s, LLONG_MAX);
            flow += pushed;
        } while (pushed > 0);
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string filename = "maxflow.dimacs";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << filename << endl;
        return 1;
    }

    int n = 0, m = 0, s = -1, t = -1;
    string line;
    vector<tuple<int,int,long long>> edges;

    while (getline(file, line)) {
        if (line.empty() || line[0] == 'c') continue;
        stringstream ss(line);
        char type;
        ss >> type;

        if (type == 'p') {
            string tmp;
            ss >> tmp >> n >> m;
        } else if (type == 'n') {
            int node;
            char label;
            ss >> node >> label;
            if (label == 's') s = node - 1;
            else if (label == 't') t = node - 1;
        } else if (type == 'a') {
            int u, v;
            long long c;
            ss >> u >> v >> c;
            edges.emplace_back(u - 1, v - 1, c);
        }
    }

    if (s == -1 || t == -1) {
        cerr << "Error: no se especificaron nodos fuente o sumidero.\n";
        return 1;
    }

    FordFulkerson ff(n, s, t);
    for (auto &[u, v, c] : edges)
        ff.add_edge(u, v, c);

    cout << "Calculando flujo máximo..." << endl;
    long long flow = ff.maxflow();
    cout << "Flujo máximo: " << flow << endl;

    return 0;
}

