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
    // cout<<content.size()<<endl;
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
    cout<<"Please wait, it will take a minute to read data and process it."<<endl;
    unordered_map<string, string> trip_route = ReadTrips();
    // cout<<trip_route.size();
    routeInfo = read_stop_times("stop_times.txt",trip_route);
    // cout<<routeInfo.size()<<endl;
    stopInfo = createStopsInfo(routeInfo);
    // cout<<stopInfo.size()<<endl;

    string s0 = "740000597";
    string sd = "740031901";
    cout<<"Enter Source: "<<endl;
    cin>>s0;
    cout<<"Enter Destination: "<<endl;
    cin>>sd;
    cout<<"Direct Journeys: "<<endl;
    vector<string> paths = getPaths(s0, sd, 0);
    cout<<"There are "<<paths.size()<<" Journeys possible"<<endl;
    for(auto path : paths) cout<<path<<endl;
    cout<<endl<<endl;
    cout<<"Journeys with one transfer: "<<endl;
    paths = getPaths(s0, sd, 1);
    cout<<"There are "<<paths.size()<<" Journeys possible"<<endl;
    for(auto path : paths) cout<<path<<endl;
    cout<<endl<<endl;
    cout<<"Journeys with two tranfers "<<endl;
    paths = getPaths(s0, sd, 2);
    cout<<"There are "<<paths.size()<<" Journeys possible"<<endl;
    for(auto path : paths) cout<<path<<endl;
}