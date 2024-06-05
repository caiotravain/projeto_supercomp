#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;
vector<vector<int>> gerartodasCombinacoes(vector<vector<int>> locations, int max_stop, vector<int> places);
bool verificarCapacidade(vector<int> &route, vector<vector<int>> &demands, int capacity);





vector<int> resolverVRPComdemanda(vector<vector<int>> &locations, vector<vector<int>> &demands, int capacity, int max_stop) {
    //call the function gerartodasCombinacoes
    vector<int> bestRoute;
    int menor_custo = INT_MAX;
    vector<int> places;
    for (int i = 0; i < locations.size(); i++) {
        if (find(places.begin(), places.end(), locations[i][0]) != places.end()) {
            continue;
        }
        
        places.push_back(locations[i][0]);
        if (find(places.begin(), places.end(), locations[i][1]) != places.end()) {
            continue;
        }
        places.push_back(locations[i][1]);
    }
    sort(places.begin(), places.end());
    vector<vector<int>> allRoutes;
    vector<vector<int>> allRoutes_best_route;
    int j = 0;
    vector<vector<int>> current_routes;
    allRoutes = gerartodasCombinacoes(locations, max_stop, places);

    menor_custo = INT_MAX;



    for (int i = 0; i < allRoutes.size(); i++) {

        if (verificarCapacidade(allRoutes[i], demands, capacity)) {
            int custo = allRoutes[i][allRoutes[i].size() - 1];
            if (custo < menor_custo) {
                menor_custo = custo;
                bestRoute = allRoutes[i];
            }
        }


    }
    allRoutes_best_route.push_back(bestRoute);
    return bestRoute ;

}


typedef tuple<int, int, int> Edge;

int find_cost_back(int place, vector<vector<int>>&locations) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i][0] == place && locations[i][1] == 0) {
            return locations[i][2];
        }
    }
    return 0;
}


// Function to perform DFS and find all paths from the origin to the destination
void dfs(int current, int destination, unordered_map<int, vector<pair<int, int>>>& graph,
         vector<int>& path, vector<vector<int>>& allPaths, unordered_set<int>& visited, vector<vector<int>> &locations,int weight = 0, int max_stop = 5, int ja_foi = 0, vector<int> places = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    // If the current node is the destination, and all nodes have been visited, then add the path to the list of all paths
    int visited_all = 1;
    //if all nodes have been visited and the current node is the destination, add the path to the list of all paths
    for (int i = 0; i < places.size(); i++) {
        if (visited.find(places[i]) == visited.end()) {
            visited_all = 0;
            break;
        }
    }
        

    if (current == destination && path.size() > 1 && visited_all == 1) {
        int weight_back_to_origin = find_cost_back(current, locations);
        //append the weight of the path
        path.push_back(weight+weight_back_to_origin);
        allPaths.push_back(path);
        path.pop_back();
    }
   
    if (path.size() > 1 && current != 0)
    {
    visited.insert(current);
    }

    // Recur for all the vertices adjacent to this vertex
    for (const auto& neighbor : graph[current]) {
        if (visited.find(neighbor.first) == visited.end()) {
            path.push_back(neighbor.first);
            dfs(neighbor.first, destination, graph, path, allPaths, visited, locations,weight + neighbor.second, max_stop, ja_foi,places);
            path.pop_back();
        }
    }

    // Remove the current node from visited set to allow other paths
    visited.erase(current);
}



// Function to find all paths from the origin to the destination
vector<vector<int>> findAllPaths(int origin, int destination, const vector<Edge>& edges, vector<vector<int>> &locations, int max_stop = 5) {
    // Build the graph from the edges
    unordered_map<int, vector<pair<int, int>>> graph;
    for (const auto& edge : edges) {
        int u = get<0>(edge);
        int v = get<1>(edge);
        int w = get<2>(edge);
        graph[u].emplace_back(v, w);
    }
    
    // Container to store all paths
    vector<vector<int>> allPaths;

    // Temporary path storage
    vector<int> path = {origin};

    // Set to keep track of visited nodes
    unordered_set<int> visited;
    vector<int> places;
    for (int i = 0; i < locations.size(); i++) {
        if (find(places.begin(), places.end(), locations[i][0]) != places.end()) {
            continue;
        }
        if (locations[i][0] == 0) {
            continue;
        }
        
        places.push_back(locations[i][0]);
        if (find(places.begin(), places.end(), locations[i][1]) != places.end()) {
            continue;
        }
        if (locations[i][1] == 0) {
            continue;
        }

        places.push_back(locations[i][1]);
    }
    sort(places.begin(), places.end());

    // Perform DFS to find all paths from origin to destination
    dfs(origin, destination, graph, path, allPaths, visited,locations, 0, max_stop,0, places  );
    vector<int> current_combination;

    return allPaths;
}


//function gerartodasCombinacoes receive the locations only and return all the possible routes
vector<vector<int>> gerartodasCombinacoes(vector<vector<int>> locations, int max_stop, vector<int> places) {
 vector<vector<int>> allRoutes;
    vector<int> currentRoute;
    int max_routes = 0;
    int origin = 0;
    int destination = 9 ;
    vector<Edge> edges;
    for (int i = 0; i < locations.size(); i++) {
        edges.push_back({locations[i][0], locations[i][1], locations[i][2]});
    }

    vector<int> all_origins;
    vector<int> all_destinations;
    for (int i = 0; i < locations.size(); i++) {
        //check if the origin is already in the all_origins
        if (find(all_origins.begin(), all_origins.end(), locations[i][0]) != all_origins.end()) {
            continue;
        }
        all_origins.push_back(locations[i][0]);
        
    }
    for (int i = 0; i < locations.size(); i++) {
        //check if the destination is already in the all_destinations
        if (find(all_destinations.begin(), all_destinations.end(), locations[i][1]) != all_destinations.end()) {
            continue;
        }
        all_destinations.push_back(locations[i][1]);
        
    }
    //find all paths from all origins to all destinations
        for (int j = 0; j < all_destinations.size(); j++) {
            vector<vector<int>> allPaths = findAllPaths(0, all_destinations[j], edges, locations, max_stop);
            //check if the route is valid(have all places), if not remove it
            
            for (int k = 0; k < allPaths.size(); k++) {
                allRoutes.push_back(allPaths[k]);
            }
        }


    return allRoutes;
}
int acha_valor(vector<vector<int>> &demands, int id) {
    for (int i = 0; i < demands.size(); i++) {
        if (demands[i][0] == id) {
            return demands[i][1];
        }
    }
    return 0;
}

//function verificarCapacidade receive the route, demands and capacity of the vehicle
bool verificarCapacidade(vector<int> &route, vector<vector<int>> &demands, int capacity) {
    int carga_atual = 0;
    for (int i = 0; i < route.size(); i++) {
        carga_atual = carga_atual + acha_valor(demands, route[i]);
        if (route[i] == 0) {
            carga_atual = 0;
        }
        if (carga_atual > capacity) {
        return false;
    }

    }
    
    return true;
}


int main() {
    //   locations
    vector<vector<int>> locations;
    //vetor demands
    vector<vector<int>> demands;


    //capacity of the vehicle
    int capacity;
    // best route
    vector<int> bestRoute;
    // best cost
    int bestCost = 0;
    //read the file
    ifstream file("grafo.txt");
    string line;
    int n;
    getline(file, line);
    stringstream ss(line);
    ss >> n;
    // cout << n << endl;
    //read the demands
    for (int i = 0; i < n -1; i++) {
        getline(file, line);
        stringstream ss(line);
        vector<int> demand;
        int demanda;
        int id;
        ss >> id;
        ss >> demanda;
        demand.push_back(id);
        demand.push_back(demanda);
        demands.push_back(demand);

    }
    int arrestas;
    getline(file, line);
    stringstream ss1(line);
    ss1 >> arrestas;
    // cout << arrestas << endl;
    //read the locations
    for (int i = 0; i < arrestas; i++) {
        getline(file, line);
        stringstream ss(line);
        vector<int> location;
        int x, y, weight;
        ss >> x >> y >> weight;
        // cout << x << " " << y << " " << weight << endl;
        location.push_back(x);
        location.push_back(y);
        location.push_back(weight);
        locations.push_back(location);
    }




    int max_stop = 5;
    capacity = 15;
    //call the function resolverVRPComdemanda
    vector<int> allRoutes_best_route;
    vector<int> already_visited;
   allRoutes_best_route = resolverVRPComdemanda(locations, demands, capacity, max_stop);

    //split the best route when the value is 0
    vector<vector<int>> allRoutes;
    vector<int> currentRoute;
    for (int i = 0; i < allRoutes_best_route.size()-1; i++) {
        if (allRoutes_best_route[i] == 0) {
            currentRoute.push_back(allRoutes_best_route[i]);
            allRoutes.push_back(currentRoute);
            currentRoute.clear();
            currentRoute.push_back(allRoutes_best_route[i]);
        } else {
            currentRoute.push_back(allRoutes_best_route[i]);
        }
    }
    //remove the last element of the vector
    

    int size;
    //print the best route
    for (int i = 0; i < allRoutes.size(); i++) {
        if (allRoutes[i].size() < 2) {
            continue;
        }   
        size = allRoutes[i].size() - 1;
        cout << "[";
        for (int j = 0; j < (size); j++) {
            cout << allRoutes[i][j] << " ";
        }
        cout << allRoutes[i][size] << "],";
        cout << endl;
    }
    cout << "Weight: " << allRoutes_best_route[allRoutes_best_route.size() - 1] << endl;
}
 

