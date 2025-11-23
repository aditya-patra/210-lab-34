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

        cout << "\nLayer-by-Layer Connectivity Check (BFS) to Computer 0:\n";
        cout << "Purpose: estimate complexity of each computer's connection to Computer 0(how many computers serve as intermediates in connection)" << endl;

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();
            cout << "Computer " << node << " checking new connections:" << endl;

            for (auto &neighbor : adjList[node]) {
                int next = neighbor.first;
                if (!visited[next]) {
                    cout << " --> Computer " << next << ", Latency: " << neighbor.second << endl;
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

        cout << "\nBranch-by-branch Connectivity Check (DFS) to Computer 0: \n";
        cout << "Purpose: Find isolated lines of computers or breaks in overall network (every computer is supposed to be connected, but starting list is purposely missing connections)" << endl;

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();

            if (!visited[node]) {
                visited[node] = true;
                cout << "Computer " << node << " checking new connections:" << endl;

                // push neighbors in reverse to maintain natural order
                for (int i = 0; i < adjList[node].size(); i++) {
                    int next = adjList[node][i].first;
                    if (!visited[next]) {
                        cout << " --> Computer " << next << ", Latency: " << adjList[node][i].second << endl;
                        dq.push_front(next);
                    }
                }
            }
        }
        cout << endl;
    }

    void shortestPath() {
        vector<int> dist(SIZE, INT_MAX);
        vector<bool> visited(SIZE, false);

        dist[0] = 0;

        // A deque used just to hold nodes we haven't processed yet
        // (not acting as a priority queue)
        deque<int> dq;
        dq.push_back(0);

        while (!dq.empty()) {
            // -----------------------------------------------------
            // Find the unvisited node with the smallest distance
            // -----------------------------------------------------
            int smallestDist = INT_MAX;
            int smallestNode = -1;

            for (int i = 0; i < SIZE; i++) {
                if (!visited[i] && dist[i] < smallestDist) {
                    smallestDist = dist[i];
                    smallestNode = i;
                }
            }

            if (smallestNode == -1)
                break; // all reachable nodes processed

            dq.pop_front(); // we "process" this node
            visited[smallestNode] = true;

            // -----------------------------------------------------
            // Relax edges
            // -----------------------------------------------------
            for (int i = 0; i < adjList[smallestNode].size(); i++) {
                int next = adjList[smallestNode][i].first;
                int weight = adjList[smallestNode][i].second;

                if (!visited[next] &&
                    dist[smallestNode] + weight < dist[next]) {

                    dist[next] = dist[smallestNode] + weight;
                    dq.push_back(next);
                }
            }
        }

        cout << "\nShortest path distances from computer 0:\n";
        for (int i = 0; i < SIZE; i++) {
            cout << " 0 -> " << i << ": ";
            if (dist[i] == INT_MAX)
                cout << "Deleted node " << i << " as per Step 2\n";
            else
                cout << dist[i] << "\n";
        }
    }
    
    void minimumSpanningTree() {
        // Step 1: Collect all unique edges
        vector<Edge> edges;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < adjList[i].size(); j++) {
                int v = adjList[i][j].first;
                int w = adjList[i][j].second;
                if (i < v) {  // avoid duplicates in undirected graph
                    Edge e;
                    e.src = i;
                    e.dest = v;
                    e.weight = w;
                    edges.push_back(e);
                }
            }
        }

        // Step 2: Sort edges by weight (simple bubble sort)
        for (int i = 0; i < edges.size() - 1; i++) {
            for (int j = 0; j < edges.size() - i - 1; j++) {
                if (edges[j].weight > edges[j + 1].weight) {
                    Edge temp = edges[j];
                    edges[j] = edges[j + 1];
                    edges[j + 1] = temp;
                }
            }
        }

        // Step 3: Initialize Union-Find structure
        vector<int> parent(SIZE);
        for (int i = 0; i < SIZE; i++)
            parent[i] = i;

        // Step 4: Kruskal’s algorithm
        int totalWeight = 0;
        cout << "\nMinimum Spanning Tree (based on shortest edges):\n";

        for (int e = 0; e < edges.size(); e++) {
            int u = edges[e].src;
            int v = edges[e].dest;
            int w = edges[e].weight;

            // Find root of u
            int rootU = u;
            while (parent[rootU] != rootU)
                rootU = parent[rootU];

            // Find root of v
            int rootV = v;
            while (parent[rootV] != rootV)
                rootV = parent[rootV];

            // If adding the edge does NOT form a cycle
            if (rootU != rootV) {
                cout << "Connection from Computer " << u << " to Computer " << v << ", Latency: " << w << "\n";
                totalWeight += w;

                // Union
                parent[rootU] = rootV;
            }
        }

        cout << "Total Latency across computer network = " << totalWeight << "\n";
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Computer Network Grid: " << endl << "====================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Computer " << i << " connections: " << endl;
            for (Pair v : adjList[i]) {
                cout << " --> ";
                cout << "Computer " << v.first << ", Latency: " << v.second << endl;
            }
            if (adjList[i].size() == 0) {
                cout << "None" << endl;
            }
        }
    }
};

int main() {
    // Creates a vector of graph edges/weights 
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,13},{0,2,6},{0,3,14},{2,3,35},{2,4,1}, {2, 7, 11}, {4, 8, 13}, {3, 9, 14}, {1, 10, 19}, {0, 11, 2}, {11, 12, 6}, {12, 3, 6}, {8, 9, 23}, {3, 7, 1}, {7, 10, 3}, {9, 0, 12}, {10, 12, 3}, {7, 11, 14}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph 
    graph.printGraph();
    //driver program 
    graph.DFS();
    graph.BFS();
    graph.shortestPath();
    graph.minimumSpanningTree();

    // menu
    int choice = -1;
    while (choice != 6) {
        cout << "[1] Display water distribution network\n[2] Check contaminant spread (BFS)\n[3] Plan inspection route  (DFS)\n[4] Calculate shortest paths\n[5] Find Minimum Spanning Tree\n[0] Exit" << endl;
    }

    return 0;
}
