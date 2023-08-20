#include <iostream>
#include <climits>
#include <cstring>
using namespace std;

class Main {
public:
    static int N;
    static int* final_path;
    static bool* visited;
    static int final_res;

    Main(int n) {
        N = n;
        final_path = new int[n + 1];
        visited = new bool[n];
    }

    static void copyToFinal(int curr_path[]) {
        for (int i = 0; i < N; i++)
            final_path[i] = curr_path[i];
        if (N - 1 == 0)
            final_path[N - 1] = curr_path[0];
    }

    static int firstMin(int** adj, int i) {
        int min_val = INT_MAX;
        for (int k = 0; k < N; k++)
            if (adj[i][k] < min_val && i != k)
                min_val = adj[i][k];
        return min_val;
    }

    static int secondMin(int** adj, int i) {
        int first = INT_MAX, second = INT_MAX;
        for (int j = 0; j < N; j++) {
            if (i == j)
                continue;

            if (adj[i][j] <= first) {
                second = first;
                first = adj[i][j];
            } else if (adj[i][j] <= second && adj[i][j] != first)
                second = adj[i][j];
        }
        return second;
    }

    static void TSPRec(int** adj, int curr_bound, int curr_weight,
                       int level, int curr_path[]) {

        if (level == N) {
            if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
                int curr_res = curr_weight + adj[curr_path[level - 1]][curr_path[0]];

                if (curr_res < final_res) {
                    copyToFinal(curr_path);
                    final_res = curr_res;
                }
            }
            return;
        }

        for (int i = 0; i < N; i++) {
            if (adj[curr_path[level - 1]][i] != 0 && !visited[i]) {
                int temp = curr_bound;
                curr_weight += adj[curr_path[level - 1]][i];

                if (level == 1)
                    curr_bound -= ((firstMin(adj, curr_path[level - 1]) +
                                   firstMin(adj, i)) / 2);
                else
                    curr_bound -= ((secondMin(adj, curr_path[level - 1]) +
                                    firstMin(adj, i)) / 2);

                if (curr_bound + curr_weight < final_res) {
                    curr_path[level] = i;
                    visited[i] = true;

                    TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path);
                }

                curr_weight -= adj[curr_path[level - 1]][i];
                curr_bound = temp;

                memset(visited, false, N * sizeof(bool));
                for (int j = 0; j <= level - 1; j++)
                    visited[curr_path[j]] = true;
            }
        }
    }

    static void TSP(int** adj) {
        int* curr_path = new int[N + 1];

        int curr_bound = 0;
        memset(curr_path, -1, (N + 1) * sizeof(int));
        memset(visited, false, N * sizeof(bool));

        for (int i = 0; i < N; i++)
            curr_bound += (firstMin(adj, i) + secondMin(adj, i));

        curr_bound = (curr_bound == 1) ? curr_bound / 2 + 1 : curr_bound / 2;

        visited[0] = true;
        curr_path[0] = 0;

        TSPRec(adj, curr_bound, 0, 1, curr_path);
    }
};

int Main::N;
int* Main::final_path;
bool* Main::visited;
int Main::final_res = INT_MAX;

int main() {
    int n;
    cout << "Enter the number of cities visited: ";
    cin >> n;
    int** adj = new int*[n];
    for (int i = 0; i < n; i++) {
        adj[i] = new int[n];
        for (int j = 0; j < n; j++)
            cin >> adj[i][j];
    }

    Main ob(n);

    ob.TSP(adj);

    cout << "Minimum cost: " << Main::final_res << endl;
    cout << "Path Taken: ";
    for (int i = 0; i <= n; i++) {
        cout << Main::final_path[i] << " ";
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < n; i++)
        delete[] adj[i];
    delete[] adj;

    delete[] Main::final_path;
    delete[] Main::visited;

    return 0;
}
