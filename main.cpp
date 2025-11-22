#include <iostream>
#include <vector>
#include <deque>
using namespace std;

const int SIZE = 13;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }
    void BFS() {
        vector<bool> visited(SIZE, false);
        deque<int> dq;

        visited[0] = true;
        dq.push_back(0);

        cout << "BFS starting at 0:\n";

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();
            cout << node << " ";

            for (auto &neighbor : adjList[node]) {
                int next = neighbor.first;
                if (!visited[next]) {
                    visited[next] = true;
                    dq.push_back(next);
                }
            }
        }
        cout << endl;
    }

    void DFS() {
        vector<bool> visited(SIZE, false);
        deque<int> dq;

        dq.push_back(0);

        cout << "DFS starting at 0: \n";

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();

            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";

                // push neighbors in reverse to maintain natural order
                for (int i = 0; i < adjList[node].size(); i++) {
                    int next = adjList[node][i].first;
                    if (!visited[next]) {
                        dq.push_front(next);
                    }
                }
            }
        }
        cout << endl;
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,12},{0,2,8},{0,3,21},{2,3,6},{2,4,4}, {2, 7, 13}, {4, 8, 3}, {3, 9, 14}, {1, 10, 19}, {0, 11, 2}, {11, 12, 6}, {12, 3, 6}, {8, 9, 23}, {3, 7, 1}, {7, 10, 3}, {9, 0, 12}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    //driver program
    graph.DFS();
    graph.BFS();

    return 0;
}
