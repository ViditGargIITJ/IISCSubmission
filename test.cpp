#include<iostream>
#include<bits/stdc++.h>
using namespace std;

unordered_map<string, string> ReadTrips(string FileName = "trips.txt"){   
    unordered_map<string, string> trip_route;
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    fstream file (FileName, ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while(getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout<<"Could not open the file\n";
    for(int i=1;i<content.size();i++)
    {   
        int n = content[i].size();
        string route_id = content[i][0];
        string trip_id = content[i][2];
        trip_route[trip_id] = route_id;
    }
    return trip_route;
}



unordered_map<string, unordered_map<string, int>> read_stop_times(string FileName , unordered_map<string, string>& trip_route){
    // route_id, map(stop_id, position)
    unordered_map<string, unordered_map<string, int>> routesInfo;
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    fstream file (FileName, ios::in);
    cout<<content.size()<<endl;
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while(getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout<<"Could not open the file\n";
    for(int i= 1; i <content.size(); i++){
        string trip_id = content[i][0];
        string route_id = trip_route[trip_id];
        string stop_id = content[i][3];
        int stop_sequence = stoi(content[i][4]);
        routesInfo[route_id][stop_id] = stop_sequence;
    }
    return routesInfo;
}


unordered_map<string, unordered_map<string, int>> createStopsInfo(unordered_map<string, unordered_map<string, int>>& routesInfo){
    // stop_id, map(route_id, sequence_order)
    unordered_map<string, unordered_map<string, int>> stopsInfo;
    for(auto route : routesInfo){
        for(auto stop : route.second){
            stopsInfo[stop.first][route.first] = stop.second;
        }
    }
    return stopsInfo;
}


// route_id, map(stop_id, position)
unordered_map<string, unordered_map<string, int>> routeInfo;
// stop_id, map(route_id, position)
unordered_map<string, unordered_map<string, int>> stopInfo;
unordered_set<string> routesTaken;
unordered_set<string> visitedStations;

vector<string> getPaths(string s0, string sd, int transfers){
    vector<string> paths;
    if(s0 == sd) return paths;
    if(visitedStations.find(s0) != visitedStations.end()) return paths;
    // if(transfers < 0 ) return {};
    if(transfers == 0){
        for(auto s0Route : stopInfo[s0]){
            string route_id = s0Route.first;
            // cout<<"Route ID "<<route_id<<endl;
            int poition = s0Route.second;
            
            if(routeInfo[route_id].find(sd) != routeInfo[route_id].end()){
                // if(routeInfo[route_id][sd] > routeInfo[route_id][s0]) 
                if(routesTaken.find(route_id) == routesTaken.end())
                    paths.push_back(route_id + "( " + s0 + " > " + sd + " )");
            }
        }
        return paths;
    }

    for(auto s0Routes : stopInfo[s0]){
        string route_id = s0Routes.first;
        int position = s0Routes.second;
        if(routesTaken.find(route_id) != routesTaken.end()) continue;
        routesTaken.insert(route_id);
        for(auto stop : routeInfo[route_id]){
            // if(stop.second < position) // can't go back
                // continue;
            if(s0 == stop.first) continue;
            vector<string> reducedPath = getPaths(stop.first, sd, transfers-1);
            if(reducedPath.size() == 0) continue;
            for(auto path : reducedPath){
                string tempPath = route_id + "( " + s0 + " > " + stop.first + " ) - " + path;
                paths.push_back(tempPath); 
            }
        }
        routesTaken.erase(route_id);
    }
    visitedStations.erase(s0);
    return paths;
}

int main(){
    // map for (trip_id, route_id)
    routeInfo["R8"]["Sd"] = 1;routeInfo["R8"]["S6"] = 2; 
    routeInfo["R7"]["S5"] = 1;routeInfo["R7"]["S1"] = 1;routeInfo["R7"]["S6"] = 1; 
    routeInfo["R6"]["S0"] = 1;routeInfo["R6"]["S5"] = 1; 
    routeInfo["R5"]["S2"] = 1;routeInfo["R5"]["Sd"] = 1;
    routeInfo["R4"]["S0"] = 1;routeInfo["R4"]["S2"] = 1;routeInfo["R4"]["S4"] = 1;
    routeInfo["R3"]["S1"] = 1;routeInfo["R3"]["Sd"] = 1;
    routeInfo["R2"]["S1"] = 1;routeInfo["R2"]["S0"] = 1;
    routeInfo["R1"]["S0"] = 1;routeInfo["R1"]["S3"] = 1;routeInfo["R1"]["Sd"] = 1; 
    stopInfo = createStopsInfo(routeInfo);
    string s0 = "S0";
    string sd = "Sd";
    cout<<"Enter Source: ";
    cin>>s0;
    cout<<"Enter destination: ";
    cin>>sd;
    int transfers = 0;
    cout<<"Enter the number of transfers allowed: ";
    cin>>transfers;
    vector<string> paths = getPaths(s0, sd, transfers);
    cout<<"Total number of Journeys: "<<paths.size()<<endl;
    for(auto path : paths) cout<<path<<endl;
}