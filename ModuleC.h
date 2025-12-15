#ifndef MODULEC_H
#define MODULEC_H

#include "Setup.h"

struct RoomNode {
    string roomID;
    int floorNumber;
    string roomType; 
    bool isReserved;
    string reservedBy;
    
    RoomNode* left;
    RoomNode* right;
    int height; 

    RoomNode(const string& id, int floor, const string& type);
};

class RoomsSchedule {
private:
    RoomNode* root;

    int customMax(int a, int b);
    int getHeight(RoomNode* node) const;
    int getBalance(RoomNode* node) const;
    void updateHeight(RoomNode* node);
    RoomNode* rotateRight(RoomNode* y);
    RoomNode* rotateLeft(RoomNode* x);
    RoomNode* insertRoomRecursive(RoomNode* node, const string& id, int floor, const string& type);
    RoomNode* findMinNode(RoomNode* node);
    RoomNode* deleteRoomRecursive(RoomNode* rootNode, const string& id);
    void printInorderRecursive(RoomNode* node);
    void printPreorderRecursive(RoomNode* node);
    void printPostorderRecursive(RoomNode* node);
    void searchRoomsByTypeRecursive(RoomNode* node, const string& type);
    void deleteTreeRecursive(RoomNode* node);
    void cancelUserReservationsRecursive(RoomNode* node, const string& userName);

public:
    RoomsSchedule();
    ~RoomsSchedule();

    void insertRoom(const string& id, int floor, const string& type);
    void deleteRoom(const string& id);
    RoomNode* searchRoom(const string& roomID) const;
    void searchRoomsByType(const string& type);
    void cancelAllUserReservations(const string& userName);
    void printInorder();
    void printPreorder();
    void printPostorder();
    bool reserveRoom(const string& roomID, const string& userName); 
    bool cancelReservation(const string& roomID);
};

#endif