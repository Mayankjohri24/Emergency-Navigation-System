#include <iostream>
#include <ctime>
#include <climits>
#include <cstdlib> // Added for malloc and free

#define Assets 11

using namespace std;

struct Information {
    int Id;
    string Name, Address;
    string Type;
} Info[Assets] = {
    {0, "SJM", "Sector 63", "h"},
    {1, "Prakash", "Sector 33", "h"},
    {2, "Jaypee", "Sector 128", "h"},
    {3, "Max", "Sector 19", "h"},
    {4, "Yatharth", "Sector 110", "h"},
    {5, "NMC", "Sector 30", "h"},
    {6, "Kailash", "Sector 27", "h"},
    {7, "Apollo", "Sector 26", "h"},
    {8, "Singh", "Sector 4", "W"},
    {9, "Mathura", "Sector 62", "W"},
    {10, "Maheshwari", "Sector 69", "W"}
};

// Data structure to store a graph edge
struct Node {
    int Val;
    Information *Data;
    Node *Next;
    int Traffic;
};

// Data structure to store a graph edge
struct Edge {
    int Source, Destination, Traffic;
};

class Graph {
    // Function to allocate a new node for the adjacency list
    Node *GetAdjListNode(int Destination, Node *Head, int Traffic) {
        Node *newNode = new Node;
        newNode->Val = Destination;
        // Point new node to the current Head
        newNode->Next = Head;
        newNode->Traffic = Traffic;
        newNode->Data = &Info[Destination];
        return newNode;
    }

    int N; // Total number of nodes in the graph
public:
    // An array of pointers to Node to represent the adjacency list
    Node **Head;

    // Constructor
    Graph(Edge Edges[], int n, int N) {
        Head = new Node *[N]();
        this->N = N;

        // Initialize Head pointer for all vertices
        for (int i = 0; i < N; i++) {
            Head[i] = nullptr;
        }

        // Add Edges to the directed graph
        for (unsigned i = 0; i < n; i++) {
            int Source = Edges[i].Source;
            int Destination = Edges[i].Destination;
            int Traffic = Edges[i].Traffic;

            // Insert at the beginning of adjacency list
            Node *newNode = GetAdjListNode(Destination, Head[Source], Traffic);

            // Point Head pointer to the new node
            Head[Source] = newNode;

            // Uncomment the following code for undirected graph
            newNode = GetAdjListNode(Source, Head[Destination], Traffic);

            // Change Head pointer to point to the new node
            Head[Destination] = newNode;
        }
    }

    void printList(Node *ptr) {
        while (ptr != nullptr) {
            cout << " Road to ";
            if (ptr->Data->Type == "h")
                cout << "hospital -";
            else
                cout << "warehouse -";

            cout << ptr->Data->Name << " has traffic level " << ptr->Traffic << "\n";
            ptr = ptr->Next;
        }
        cout << endl;
    }

    void printGraph() {
        for (int i = 0; i < N; i++) {
            cout << "Starting from " << Info[i].Name << ": \n";
            printList(this->Head[i]);
        }
    }

    int findDestinationId(string name, int a, int b, int c) {
        for (int i = 0; i < Assets; i++) {
            if (Info[i].Name == name) {
                return Info[i].Id;
            }
        }
        return -1;
    }

    int Vertices() {
        return N;
    }

    ~Graph() {
        for (int i = 0; i < N; i++) {
            Node* current = Head[i];
            Node* next;
            while (current != nullptr) {
                next = current->Next;
                delete current;
                current = next;
            }
        }
        delete[] Head;
    }
};

int random(int min, int max) {
    int random_variable = rand();
    return min + (random_variable % (max - min + 1));
}

int convert(int val) {
    return 5 * val;
}

// Structure to represent a min heap node
struct MinHeapNode {
    int v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

// Function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v,int dist) {
    struct MinHeapNode* minHeapNode = new MinHeapNode;
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Function to create a Min Heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = new MinHeap;
    minHeap->pos = new int[capacity];
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = new MinHeapNode*[capacity];
    return minHeap;
}

// Function to swap two nodes of min heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Function to heapify at given idx
void minHeapify(struct MinHeap* minHeap,int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
        smallest = right;
    if (smallest != idx) {
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Function to check if given vertex 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// Function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap *minHeap) {
    if (minHeap->size == 0)
        return NULL;
    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

// Function to decrease dist value of a given vertex 'v'
void decreaseKey(struct MinHeap* minHeap,int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] =(i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

struct Warehouse {
    int Id;
    int dist;
};

void SmallestRoute(int dist[], int n, int src, int dest) {
    cout<<"Time taken from warehouse " << Info[src].Name << " to reach hospital "<< Info[dest].Name << " is " << convert(dist[dest]) << " minutes." << endl;
}

Warehouse dijkstra(Graph* graph, int src, int dest) {
    int V = graph->Vertices();
    int dist[V];
    struct MinHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;
    while (minHeap->size != 0) {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        Node* pCrawl = graph->Head[u];
        while (pCrawl != NULL) {
            int v = pCrawl->Val;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->Traffic + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->Traffic;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->Next;
        }
    }
    SmallestRoute(dist, V, src, dest);
    Warehouse w = {src, dist[dest]};
    return w;
}

void bellmanFord(Graph* graph, int src, int dest) {
    int V = graph->Vertices();
    int dist[V];
    for (int i = 0; i < V; ++i) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    for (int i = 1; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            Node* pCrawl = graph->Head[j];
            while (pCrawl != NULL) {
                int u = j;
                int v = pCrawl->Val;
                int weight = pCrawl->Traffic;
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
                pCrawl = pCrawl->Next;
            }
        }
    }
    for (int i = 0; i < V; ++i) {
        Node* pCrawl = graph->Head[i];
        while (pCrawl != NULL) {
            int u = i;
            int v = pCrawl->Val;
            int weight = pCrawl->Traffic;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                cout << "Graph contains negative-weight cycle" << endl;
                return;
            }
            pCrawl = pCrawl->Next;
        }
    }
    SmallestRoute(dist, V, src, dest);
}

Graph* CreateMap() {
    Edge Edges[19] = {
        {0, 5}, {1, 5}, {1, 2}, {2, 3}, {2, 4}, {3, 5}, {3, 6}, {4, 7}, {4, 8},
        {5, 9}, {5, 10}, {6, 5}, {7, 6}, {7, 10}, {8, 7}, {8, 0}, {9, 2}, {9, 3},
        {10, 9}
    };

    for (int i = 0; i < sizeof(Edges) / sizeof(Edges[0]); i++) {
        Edges[i].Traffic = random(1, 10);
    }

    int N = 11;
    int n = 19;

    Graph* Graphic = new Graph(Edges, n, N);

    return Graphic;
}

int main() {
    srand(time(nullptr));

    Graph* G = CreateMap();

    cout << " -------- Welcome to Covid-19 Resource Delivery System\n\n";
    cout << "For the purpose of this demonstration, we have considered 8 hospitals and 3 warehouses\n";
    cout << "Details of the above are as follows: \n";

    for (int i = 0; i < Assets; i++) {
        if (Info[i].Type == "h") {
            cout << "Hospital : " << Info[i].Name << ", " << Info[i].Address << endl;
        } else {
            cout << "Warehouse : " << Info[i].Name << ", " << Info[i].Address << endl;
        }
    }
    cout << "\n";

    G->printGraph();

    cout << "Enter the name of the hospital that requires resources: ";
    string hospital;
    cin >> hospital;

    int a, b, c;

    cout << "Enter the required number of oxygen cylinders/concentrator: ";
    cin >> a;

    cout << "Enter the required number of PPE kits: ";
    cin >> b;

    cout << "Enter the required number of remdesivir: ";
    cin >> c;

    int dest = G->findDestinationId(hospital, a, b, c);
    cout << endl;

    if (dest == -1) {
        cout << "Invalid hospital name. Please enter a valid hospital name." << endl << endl;
        return 1;
    }

    Warehouse fastest_dijkstra = {-1, INT_MAX};
    Warehouse fastest_bellman = {-1, INT_MAX};

    for (int i = 8; i < 11; i++) {
        Warehouse temp_dijkstra = dijkstra(G, Info[i].Id, dest);
        if (temp_dijkstra.dist < fastest_dijkstra.dist)
            fastest_dijkstra = temp_dijkstra;

        Warehouse temp_bellman = dijkstra(G, Info[i].Id, dest);
        if (temp_bellman.dist < fastest_bellman.dist)
            fastest_bellman = temp_bellman;
    }

    cout << "\nUsing Dijkstra's algorithm the most efficient warehouse for delivering the required resources is : ";
    cout << Info[fastest_dijkstra.Id].Name << endl;

    cout << "\nUsing Bellman Ford's algorithm the most efficient warehouse for delivering the required resources is : ";
    cout << Info[fastest_bellman.Id].Name << endl;

    return 0;
}
