#ifndef MODULEB_H
#define MODULEB_H

#include "Setup.h"

struct EdgeNode {
    string destBuildingName;
    string relation;        
    string status;          
    int weight;             

    EdgeNode* next;

    EdgeNode(const string& dest, const string& rel, const string& stat, int w);
};

struct BuildingNode {
    string buildingName;
    EdgeNode* adjList; 
    BuildingNode* next; 
    RoomsSchedule* roomTree; 

    BuildingNode(const string& name);
    ~BuildingNode(); 
};

class CampusMap {
private:
    BuildingNode* head; 
    int numBuildings;
    const int HUGE_DISTANCE = 999999; 

    void removeEdgeHelper(BuildingNode* src, const string& destName);
    void clearAllEdges(BuildingNode* building);
    void DFSHelper(BuildingNode* startNode, bool visited[], string buildingNames[], int count);

public:
    CampusMap();
    ~CampusMap();

    BuildingNode* findBuilding(const string& name) const;
    void addBuilding(const string& name);
    BuildingNode* removeBuilding(const string& name);
    void addPath(const string& src, const string& dest, const string& relation, const string& status, int weight);
    void removePath(const string& src, const string& dest);
    void displayMap() const;
    BuildingNode* getBuildingHead() const;
    
    void BFS(const string& startBuilding);
    void DFS(const string& startBuilding);
    void shortestPath(const string& startBuilding, const string& endBuilding);
    int getBuildingIndex(const string& name, string names[], int total) const;
};

#endif