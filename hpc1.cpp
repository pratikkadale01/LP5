#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Undirected Graph class
class Graph {
private:
    int numVertices;
    vector<vector<int>> adjList;

public:
    Graph(int vertices) : numVertices(vertices), adjList(vertices) {}

    // Add edge between two vertices
    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
    }

    // Breadth First Search (BFS)
    void BFS(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int currVertex = q.front();
            q.pop();
            cout << currVertex << " ";

#pragma omp parallel for
            for (int i = 0; i < adjList[currVertex].size(); i++) {
                int neighbor = adjList[currVertex][i];

#pragma omp critical
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    // Depth First Search (DFS)
    void DFS(int startVertex) {
        vector<bool> visited(numVertices, false);
        stack<int> s;
        s.push(startVertex);

        while (!s.empty()) {
            int currVertex = s.top();
            s.pop();

            if (!visited[currVertex]) {
                cout << currVertex << " ";
                visited[currVertex] = true;

#pragma omp parallel for
                for (int i = 0; i < adjList[currVertex].size(); i++) {
                    int neighbor = adjList[currVertex][i];

#pragma omp critical
                    if (!visited[neighbor]) {
                        s.push(neighbor);
                    }
                }
            }
        }
    }
};

int main() {
    const int numVertices = 8;
    Graph graph(numVertices);

    // Add edges to the graph
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 5);
    graph.addEdge(2, 6);
    graph.addEdge(3, 7);

    // Sequential BFS
    auto startTime = high_resolution_clock::now();
    cout << "Sequential BFS: ";
    graph.BFS(0);
    auto endTime = high_resolution_clock::now();
    auto sequentialBFSDuration = duration_cast<milliseconds>(endTime - startTime);
    cout << "\nSequential BFS Execution Time: " << sequentialBFSDuration.count() << " ms\n\n";

    // Sequential DFS
    startTime = high_resolution_clock::now();
    cout << "Sequential DFS: ";
    graph.DFS(0);
    endTime = high_resolution_clock::now();
    auto sequentialDFSDuration = duration_cast<milliseconds>(endTime - startTime);
    cout << "\nSequential DFS Execution Time: " << sequentialDFSDuration.count() << " ms\n\n";

    // Parallel BFS
    omp_set_num_threads(4);
    startTime = high_resolution_clock::now();
    cout << "Parallel BFS: ";
    graph.BFS(0);
    endTime = high_resolution_clock::now();
    auto parallelBFSDuration = duration_cast<milliseconds>(endTime - startTime);
    cout << "\nParallel BFS Execution Time: " << parallelBFSDuration.count() << " ms\n\n";

    // Parallel DFS
    startTime = high_resolution_clock::now();
    cout << "Parallel DFS: ";
    graph.DFS(0);
    endTime = high_resolution_clock::now();
    auto parallelDFSDuration = duration_cast<milliseconds>(endTime - startTime);
    cout << "\nParallel DFS Execution Time: " << parallelDFSDuration.count() << " ms\n\n";

    return 0;
}
