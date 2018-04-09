#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "network.h"

// Defining types which will be used inside program
typedef pair <int, int> pii;
typedef vector <pii> vii;
typedef vector <int> vi;
typedef vector < vector <int> > v_2d;
typedef vector <string> vs;
typedef map <string, int> msi;

// Global Variables
vs itov; // index to vertex
msi vtoi; // vertex to index
v_2d alist; // adjacency list
v_2d wlist; // weight list

void connect(vs sip, int l){
  /*
  Input :- vector of vertices, sip = v1,v2...vN, and integer l if we want to connect i to every j with weight l where i,j belongs to sip and i != j

  What it does :- It adds j to alist[i] where v[i],v[j] belongs to sip and v[i]!=v[j]. It also adds weight of the edge (v[i],v[j]) to wlist[i][j]
  */
  vi iip;
  for (size_t i = 0; i < sip.size(); i++) {
    iip.push_back(vtoi[sip[i]]);
  }
  for (size_t i = 0; i < iip.size(); i++) {
    for (size_t j = i + 1; j < iip.size(); j++) {
      alist[iip[i]].push_back(iip[j]);
      wlist[iip[i]].push_back(l);

      alist[iip[j]].push_back(iip[i]);
      wlist[iip[j]].push_back(l);
    }
  }
}

vi d_spath(int s, int t){
  /*
  Notation :-
    v[i] is the vertex at the index i
    in[v] is the index of vertex v
  inputs :-
  alist[i] is the vector containing all u such that (v[i], v[u]) is an edge
  wlist[i][j] is the weight of the edge (v[i],v[alist[i][j]])
  s = in[a] and t = in[t] where we want to find shortest path between a and b
  output :-
  a1,a2,...,aN such that v[a1],v[a2],...,v[aN] is the shortest path between a and b
  */
  vi path;
  int n = alist.size();
  vi d(n,1000000000); // set all d to infinite
  vi prev(n, -1);
  d[s] = 0;

  priority_queue <pii, vii, greater<pii> > H;
  for (size_t i = 0; i < n; i++) {
    H.push({d[i], i});
  }
  while(!H.empty()){
    int x = H.top().second;
    H.pop();
    for (size_t i = 0; i < alist[x].size(); i++) {
      int y = alist[x][i];
      if (d[y] > d[x] + wlist[x][y]) {
        d[y] = d[x] + wlist[x][y];
        prev[y] = x;
      }
    }
  }

  vi rev_path;
  int m = t ;
  while (m != -1){
    rev_path.push_back(m);
    m = prev[m];
  }

  int path_len = rev_path.size();
  for (size_t i = 0; i < path_len; i++) {
    path.push_back(rev_path[path_len - i - 1]);
  }

  return path;
}

int main(){
  // storing vertices and giving it a index
  bool isNewIpStarting = true;
  for (size_t i = 0; i < IPaddress.length(); i++) {
    if (isNewIpStarting && IPaddress[i] <= '9' && IPaddress[i] >= '0') {
      itov.push_back("");
      itov[itov.size() - 1] += IPaddress[i];
      isNewIpStarting = false;
    }
    else if (IPaddress[i] == ',') {
      isNewIpStarting = true;
    }
    else if (! isNewIpStarting) {
      itov[itov.size() - 1] += IPaddress[i];
    }
  }

  for (size_t i = 0; i < itov.size(); i++) {
    vtoi[itov[i]] = i ;
  }

  //resizing alist and wlist
  int n = itov.size();
  alist.resize(n);
  wlist.resize(n);

  // storing routers
  vs stmp; // This will temporarily store
  isNewIpStarting = true;
  for (size_t i = 0; i < Routers.length(); i++) {
    if (isNewIpStarting && Routers[i] <= '9' && Routers[i] >= '0') {
      stmp.push_back("");
      stmp[stmp.size() - 1] += Routers[i];
      isNewIpStarting = false;
    }
    else if (Routers[i] == ',') {
      isNewIpStarting = true;
    }
    else if (Routers[i] == ';') {
      // connect all these vertices with edge weight = 0 and make that vector empty again
      connect(stmp,0);
      stmp.resize(0);
      isNewIpStarting = true;
    }
    else if (! isNewIpStarting) {
      stmp[stmp.size() - 1] += Routers[i];
    }
  }
  connect(stmp,0);
  stmp.resize(0);

  // storing edges into adjacency list
  isNewIpStarting = true;
  for (size_t i = 0; i < InterfaceDistance.length(); i++) {
    if (isNewIpStarting && InterfaceDistance[i] <= '9' && InterfaceDistance[i] >= '0') {
      stmp.push_back("");
      stmp[stmp.size() - 1] += InterfaceDistance[i];
      isNewIpStarting = false;
    }
    else if (InterfaceDistance[i] == ',') {
      isNewIpStarting = true;
    }
    else if (InterfaceDistance[i] == ')') {
      // store that number into edges and make that vector empty again
      int len = stoi(stmp.back());
      stmp.pop_back();
      connect(stmp,len);

      stmp.resize(0);
      isNewIpStarting = true;
    }
    else if (! isNewIpStarting) {
      stmp[stmp.size() - 1] += InterfaceDistance[i];
    }
  }

  while (true) {
    string source, dist ;
    std::cout << "Give source and destination IP: " ;
    cin >> source;
    // Checking if input is equal to bye
    std::transform(source.begin(), source.end(), source.begin(), ::tolower);
    if (source == "bye") {
      break;
    }
    // Remove comma from source
    string comma_removed_source = "";
    for (size_t i = 0; i < source.length(); i++) {
      if (source[i] == '.' || (source[i] >= '0' && source[i] <= '9')) {
        comma_removed_source += source[i];
      }
    }
    source = comma_removed_source;
    cin >> dist;
    vi path_indices = d_spath(vtoi[source], vtoi[dist]);
    std::cout << "The packet from " << source << " to " << dist << " goes via the following router interfaces: ";
    // Print the path after converting indices of path into string
    for (size_t i = 0; i < path_indices.size(); i++) {
      std::cout << itov[path_indices[i]] ;
      if (i != path_indices.size() - 1) {
        std::cout << "," ;
      }
      else{
        std::cout << '\n';
      }
    }
  }

  return 0;
}
