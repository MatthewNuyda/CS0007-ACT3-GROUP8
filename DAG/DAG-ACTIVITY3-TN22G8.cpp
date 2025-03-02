#include <iostream> //used for input output
#include <fstream> //used for file handling
#include <cstdlib> //used for randomization
#include <ctime>  //used for randomization

using namespace std;

class Graph {
private:
    int n;                  // Number of nodes
    int m;                  // Number of dependencies
    bool** adjMatrix;       // Checker for adjacency
    int* visited;           // Depth First Search Visited Checking
    int* result;            // Store Sort result
    int resultIndex;        // Index for result array

public:
    // Constructor
    Graph(int nodes) {
        if (nodes <= 0 || nodes > 100) {
            cout << "Error: Invalid number of nodes. Must be between 1 and 100" << endl;
            exit(1);
        }

        n = nodes;
        m = 0;
        resultIndex = 0;

        // Construct the adjacency matrix
        adjMatrix = new bool*[n];
        for (int i = 0; i < n; i++) {
            adjMatrix[i] = new bool[n];
            for (int j = 0; j < n; j++) {
                adjMatrix[i][j] = false;
            }
        }
        //construct visited and result array
        visited = new int[n];
        result = new int[n];

        // Initialize visited array
        for (int i = 0; i < n; i++) {
            visited[i] = 0;
        }
    }

    // Destructor
    ~Graph() {
        for (int i = 0; i < n; i++) {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
        delete[] visited;
        delete[] result;
    }


    // function to add edge
    void addEdge(int u, int v) {
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << "Error: Node index out of range." << endl;
            return;
        }

        if (!adjMatrix[u][v]) {
            adjMatrix[u][v] = true;
            m++;
        }
    }

    //DAG Generator using stime
    bool generateRandomDAG(int edges) {
        if (edges < 0 || edges > n * (n - 1) / 2) {
            cout << "Error: Invalid number of edges. Must be between 0 and " << n * (n - 1) / 2 << endl;
            return false;
        }

        // Limit edges to 100
        if (edges > 100) {
            cout << "Warning: Requested edges (" << edges << ") exceeds maximum allowed (100) edges." << endl;
            edges = 100;
        }

        // Clear existing edges
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adjMatrix[i][j] = false;
            }
        }
        m = 0;

        //make sure that the graph made is acyclic low node -> high node
        int attempts = 0;
        while (m < edges && attempts < 1000) {
            int u = rand() % n;
            int v = rand() % n;

            // Ensure v > u to maintain acyclicity
            if (v > u && !adjMatrix[u][v]) {
                addEdge(u, v);
            }

            attempts++;

            // Just incase
            if (attempts >= 1000 && m < edges) {
                cout << "Warning: Could only generate " << m << " edges out of " << edges << " requested." << endl;
                break;
            }
        }

        return true;
    }

    // DFS for topological sort
    void DFS(int node, bool* onStack, bool printSteps) {
        visited[node] = 1;
        onStack[node] = true;

        if (printSteps) {
            cout << "Visiting node " << node << endl;
        }

        // Check all neighboring nodes
        for (int i = 0; i < n; i++) {
            if (adjMatrix[node][i]) {
                if (!visited[i]) {
                    DFS(i, onStack, printSteps);
                } else if (onStack[i]) {
                    cout << "Error: Cycle detected in the graph! Path includes nodes: " << node << " -> " << i << endl;
                    cout << "This graph is not a DAG. Topological sort is only possible on DAGs." << endl;
                    exit(1);
                }
            }
        }

        // Add node to result in reverse order (for topological sort)
        result[n - 1 - resultIndex++] = node;
        onStack[node] = false;

        if (printSteps) {
            cout << "Finished processing node " << node << ", adding to result" << endl;
        }
    }

    // Topological Sort using DFS
    void topologicalSort(bool printSteps) {
        // Reset visited array and result index
        for (int i = 0; i < n; i++) {
            visited[i] = 0;
        }
        resultIndex = 0;

        // track node existing in stack to track if its a cyclic graph
        bool* onStack = new bool[n];
        for (int i = 0; i < n; i++) {
            onStack[i] = false;
        }

        // Perform DFS on all unvisited nodes
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                DFS(i, onStack, printSteps);
            }
        }

        delete[] onStack;
    }

    // Print the graph
    void printGraph() {
        cout << "Graph with " << n << " nodes and " << m << " edges:" << endl;


        cout << "\nAdjacency Matrix:" << endl;
        int colWidth = 3;

        // top
        cout << "+";
        for (int i = 0; i < n + 1; i++) {
            for (int j = 0; j < colWidth; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;

        // header & col index
        cout << "|" << string(colWidth, ' ') << "|";
        for (int i = 0; i < n; i++) {
            cout << string((colWidth - 1) / 2, ' ') << i
                 << string(colWidth - ((colWidth - 1) / 2) - 1, ' ') << "|";
        }
        cout << endl;

        // divider
        std::cout << "+";
        for (int i = 0; i < n + 1; i++) {
            for (int j = 0; j < colWidth; j++) {
                std::cout << "-";
            }
            std::cout << "+";
        }
        std::cout << std::endl;

        // Print matrix rows
        for (int i = 0; i < n; i++) {
            // Print row index
            std::cout << "|" << std::string((colWidth - 1) / 2, ' ') << i
                      << std::string(colWidth - ((colWidth - 1) / 2) - 1, ' ') << "|";

            // Print row values
            for (int j = 0; j < n; j++) {
                std::cout << std::string((colWidth - 1) / 2, ' ')
                          << (adjMatrix[i][j] ? "1" : "0")
                          << std::string(colWidth - ((colWidth - 1) / 2) - 1, ' ') << "|";
            }
            std::cout << std::endl;
        }

        // bottom
        std::cout << "+";
        for (int i = 0; i < n + 1; i++) {
            for (int j = 0; j < colWidth; j++) {
                std::cout << "-";
            }
            std::cout << "+";
        }
        std::cout << std::endl;

        //Print Edges
        cout << "\nEdge List:" << endl;
        if (m == 0) {
            cout << "No edges in the graph." << endl;
            return;
        }

        int srcWidth = 6;
        int destWidth = 11;

        // top
        cout << "+" << string(srcWidth, '-') << "+"
                << string(destWidth, '-') << "+" << endl;

        // header
        cout << "|" << "Source" << "|" << "Destination" << "|" << endl;

        // divider
        std::cout << "+" << std::string(srcWidth, '-') << "+"
                  << std::string(destWidth, '-') << "+" << std::endl;

        // Print edges
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjMatrix[i][j]) {
                    std::cout << "|" << std::string((srcWidth - 1) / 2, ' ') << i
                              << std::string(srcWidth - ((srcWidth - 1) / 2) - 1, ' ') << "|"
                              << std::string((destWidth - 1) / 2, ' ') << j
                              << std::string(destWidth - ((destWidth - 1) / 2) - 1, ' ') << "|" << std::endl;
                }
            }
        }

        // bottom
        std::cout << "+" << std::string(srcWidth, '-') << "+"
                  << std::string(destWidth, '-') << "+" << std::endl;
    }

    // Print the topological sort
    void printTopologicalSort() {
        cout << "\nTopological Sort Result:" << endl;

        int colWidth = 5;  // Minimum width for "Order" header
        int nodeWidth = 5; // Minimum width for "Node" header

        // top
        std::cout << "+" << std::string(colWidth, '-') << "+"
                  << std::string(nodeWidth, '-') << "+" << std::endl;

        // header
        std::cout << "|" << "Order" << "|" << "Node " << "|" << std::endl;

        // divider
        std::cout << "+" << std::string(colWidth, '-') << "+"
                  << std::string(nodeWidth, '-') << "+" << std::endl;

        // results
        for (int i = 0; i < n; i++) {
            std::cout << "|" << std::string((colWidth - 1) / 2, ' ') << i + 1
                      << std::string(colWidth - ((colWidth - 1) / 2) - 1, ' ') << "|"
                      << std::string((nodeWidth - 1) / 2, ' ') << result[i]
                      << std::string(nodeWidth - ((nodeWidth - 1) / 2) - 1, ' ') << "|" << std::endl;
        }

        // bottom
        std::cout << "+" << std::string(colWidth, '-') << "+"
                  << std::string(nodeWidth, '-') << "+" << std::endl;
    }

    // Save to File
    bool saveToFile(const char* filename) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            cout << "Error: Unable to open file for writing." << endl;
            return false;
        }

        // Write number of nodes and edges
        file << "Graph with " << n << " nodes and " << m << " edges" << std::endl;

        // Write adjacency matrix
        file << "\nAdjacency Matrix:" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                file << (adjMatrix[i][j] ? "1" : "0") << " ";
            }
            file << std::endl;
        }

        // Write edge list
        file << "\nEdge List:" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjMatrix[i][j]) {
                    file << i << " -> " << j << std::endl;
                }
            }
        }

        // Write topological sort result
        file << "\nTopological Sort Result: ";
        for (int i = 0; i < n; i++) {
            file << result[i] << " ";
        }
        file << std::endl;

        file.close();
        return true;
    }

    // Load the graph from a file
    bool loadFromFile(const char* filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            cout << "Error: Unable to open file for reading." << endl;
            return false;
        }

        string line;

        // Skip graph info
        std::getline(file, line);

        // Skip adjacency matrix header
        std::getline(file, line);
        std::getline(file, line);

        // Read adjacency matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                char c;
                file >> c;
                adjMatrix[i][j] = (c == '1');
                if (adjMatrix[i][j]) m++;
            }
        }

        file.close();
        return true;
    }

    // Get number of nodes
    int getNodeCount() {
        return n;
    }

    // Get number of edges
    int getEdgeCount() {
        return m;
    }
};

// Function to get integer input with validation
int getInteger(const char* prompt, int min, int max) {
    int value;
    bool valid = false;

    do {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                valid = true;
            } else {
                cout << "Value must be between " << min << " and " << max << endl;
            }
        } else {
            cout << "Invalid input. Please enter an integer." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    } while (!valid);

    return value;
}

// Main function
int main() {
    // Seed random number generator
    srand(time(NULL));

    int choice;
    bool exit = false;

    // Main menu loop
    while (!exit) {
        cout << "\n==== DAG Generator and Topological Sort ====" << endl;
        cout << "1. Generate New Graph" << endl;
        cout << "2. Load Graph from File" << endl;
        cout << "3. Print Graph" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                // Generate new graph
                int n = getInteger("Enter number of nodes (1-100): ", 1, 100);
                int maxEdges = n * (n - 1) / 2;  //middle aka maximum number of edges
                if (maxEdges > 100) maxEdges = 100;  // Apply 100 edge limit
                int m = getInteger(("Enter number of edges (0-" + to_string(maxEdges) + "): ").c_str(), 0, maxEdges);

                // Create new graph
                Graph graph(n);

                // Generate random DAG
                if (graph.generateRandomDAG(m)) {
                    cout << "Random DAG generated successfully." << endl;

                    // Print the graph
                    graph.printGraph();


                    cout << "Performing topological sort..." << endl;
                    graph.topologicalSort(true);  // true to show the process
                    graph.printTopologicalSort();

                    // save to DAG-GENERATED.txt
                    if (graph.saveToFile("DAG-GENERATED.txt")) {
                        cout << "Graph and topological sort saved to DAG-GENERATED.txt" << endl;
                    } else {
                        cout << "Failed to save to file." << endl;
                    }
                } else {
                    cout << "Failed to generate random DAG." << endl;
                }
                break;
            }
            case 2: {
                // Load graph from file
                char filename[100];
                cout << "Enter filename (default is DAG-GENERATED.txt): ";
                cin >> filename;

                // Create graph with default size (can be adjusted after loading)
                Graph graph(6);  // 6 is just an example default size

                if (graph.loadFromFile(filename)) {
                    cout << "Graph loaded successfully." << endl;
                    graph.printGraph();

                    // Perform topological sort on the loaded graph
                    cout << "Performing topological sort on loaded graph..." << endl;
                    graph.topologicalSort(true);
                    graph.printTopologicalSort();
                } else {
                    cout << "Failed to load graph." << endl;
                }
                break;
            }
            case 3: {
                // Create a test graph to display
                cout << "\n==== Test Case: N=6, M=8 ====" << endl;
                Graph testGraph(6);
                if (testGraph.generateRandomDAG(8)) {
                    cout << "Test graph generated successfully." << endl;
                    testGraph.printGraph();
                    cout << "Performing topological sort..." << endl;
                    testGraph.topologicalSort(true);
                    testGraph.printTopologicalSort();

                    // Save test graph to file
                    if (testGraph.saveToFile("DAG-TEST.txt")) {
                        cout << "Test graph saved to DAG-TEST.txt" << endl;
                    }
                } else {
                    cout << "Failed to generate test graph." << endl;
                }
                break;
            }
            case 4: {
                // Exit
                exit = true;
                break;
            }
            default: {
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                break;
            }
        }
    }

    return 0;
}
