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
bool verificarCapacidade(vector<int> &route, vector<vector<int>> &demands, int capacity);

typedef tuple<int, int, int> Edge;
int acha_valor(vector<vector<int>> &demands, int id) {
    for (int i = 0; i < demands.size(); i++) {
        if (demands[i][0] == id) {
            return demands[i][1];
        }
    }
    return 0;
}




vector<int> resolverVRPComdemanda(vector<vector<int>> &locations, vector<vector<int>> &demands, int capacity, int max_stop) {
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
    vector<vector<int>> current_routes;


    menor_custo = INT_MAX;
    vector<vector<int>> cheaper_route;
    int j = 0;
    int current_place = places[0];
    int peso = acha_valor(demands, current_place);
    int total_cost = 0;
    while (places.size() > 1) {
        vector<int> route;
        menor_custo = INT_MAX;
        route.push_back(current_place);
        int maior_price = INT_MAX;

        // cout << "current_place: " << current_place << endl;
        int achou = 0;
        for (int i = 0; i < locations.size(); i++) {
            int preco; 
    
            if (locations[i][0] == current_place && (peso+acha_valor(demands, locations[i][1])) <= capacity) {
                preco = locations[i][2];
                //if location[i][1] is in places 
                if (find(places.begin(), places.end(), locations[i][1]) != places.end()) {
                    if (preco < maior_price) {
                        maior_price = preco;
                        //clear the route
                        route.clear();
                        route.push_back(current_place);
                        route.push_back(locations[i][1]);
                        achou = 1;
                    }
                }  
            }
            
        }
        //if achou is 0 goes back to the depot
        if (achou == 0) {
            for (int i = 0; i < locations.size(); i++) {
                int preco;
                if (locations[i][0] == current_place && locations[i][1] == 0) {
                    preco = locations[i][2];
                    if (preco < maior_price) {
                        maior_price = preco;
                        //clear the route
                        route.clear();
                        route.push_back(current_place);
                        route.push_back(locations[i][1]);
                    }
                }
            }
        }
        total_cost = total_cost + maior_price;
        peso = peso + acha_valor(demands, route[route.size() - 1]);
        // cout << "route: ";        
        cheaper_route.push_back(route);

        if (current_place != 0) {
            places.erase(remove(places.begin(), places.end(), current_place), places.end());
        }


        current_place = route[route.size() - 1];
        if (current_place == 0) {
            peso = 0;
        }

        //remove the current place from the places
        
        j++;
    }
    cout << "Cheaper route: " << endl;
    for (int i = 0; i < cheaper_route.size(); i++) {
        for (int j = 0; j < cheaper_route[i].size(); j++) {
            cout << cheaper_route[i][j] << " ";
        }
        cout << endl;
        }
    cout << "Total cost: " << total_cost << endl;
    return bestRoute ;

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


}
 

