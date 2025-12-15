#include "ModuleA.h"
#include "Moduleb.h"
#include "ModuleC.h"
#include "ModuleD.h"
#include "ModuleE.h"

HashEntry::HashEntry(const string& user, const string& pass, const string& r, 
          const string& dept, const string& mail) 
{
    userName = user;
    password = pass;
    role = r;
    department = dept;
    email = mail;
    lastBookedRoom = "None"; 
    next = nullptr;
}

int PeopleDirectory::hashFunction(const string& key) const {
    int hash_val = 0;
    for (int i = 0; i < key.length(); ++i) {
        hash_val = (hash_val + key[i]) % TABLE_CAPACITY;
    }
    return hash_val;
}

PeopleDirectory::PeopleDirectory() : size(0) {
    for (int i = 0; i < TABLE_CAPACITY; ++i) {
        table[i] = nullptr;
    }
}

PeopleDirectory::~PeopleDirectory() {
    for (int i = 0; i < TABLE_CAPACITY; ++i) {
        HashEntry* entry = table[i];
        while (entry != nullptr) {
            HashEntry* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
}

HashEntry* PeopleDirectory::getUser(const string& userName) const {
    int index = hashFunction(userName);
    HashEntry* entry = table[index];

    while (entry != nullptr) {
        if (entry->userName == userName) {
            return entry;
        }
        entry = entry->next;
    }
    return nullptr; 
}

bool PeopleDirectory::addUser(const string& userName, const string& password, const string& role, 
             const string& department, const string& email) {
    if (getUser(userName) != nullptr) {
        cout << "[ERROR] User '" << userName << "' already exists." << endl;
        return false;
    }

    int index = hashFunction(userName);
    HashEntry* newEntry = new HashEntry(userName, password, role, department, email);
    newEntry->next = table[index];
    table[index] = newEntry;
    size++;
    return true;
}

bool PeopleDirectory::authenticateUser(const string& userName, const string& password) const {
    HashEntry* user = getUser(userName);
    if (user == nullptr) {
        return false;
    }
    return user->password == password;
}

void PeopleDirectory::updateLastBookedRoom(const string& userName, const string& roomID) {
    HashEntry* user = getUser(userName);
    if (user != nullptr) {
        user->lastBookedRoom = roomID;
    }
}

bool PeopleDirectory::updateProfile(const string& userName, const string& newPass, const string& newDept) {
    HashEntry* user = getUser(userName);
    if (user != nullptr) {
        user->password = newPass;
        user->department = newDept;
        return true;
    }
    return false;
}

string PeopleDirectory::getUserRole(const string& userName) const {
    HashEntry* user = getUser(userName);
    if (user != nullptr) {
        return user->role;
    }
    return "";
}

bool PeopleDirectory::deleteUser(const string& userName) {
    int index = hashFunction(userName);
    HashEntry* current = table[index];
    HashEntry* prev = nullptr;

    while (current != nullptr && current->userName != userName) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "[ERROR] User not found for deletion." << endl;
        return false;
    }

    if (prev == nullptr) {
        table[index] = current->next;
    } else {
        prev->next = current->next;
    }
    delete current;
    size--;
    return true;
}

void PeopleDirectory::displayAllUsers() const {
    cout << endl << "--- Directory Contents (By Bucket) ---" << endl;
    for (int i = 0; i < TABLE_CAPACITY; ++i) {
        HashEntry* entry = table[i];
        if (entry != nullptr) {
            cout << "Bucket " << i << ": ";
            while (entry != nullptr) {
                cout << "[" << entry->userName << " (" << entry->role << ")] -> ";
                entry = entry->next;
            }
            cout << "NULL" << endl;
        }
    }
}

EdgeNode::EdgeNode(const string& dest, const string& rel, const string& stat, int w) {
    destBuildingName = dest;
    relation = rel;
    status = stat;
    weight = w;
    next = nullptr;
}

BuildingNode::BuildingNode(const string& name) {
    buildingName = name;
    adjList = nullptr;
    next = nullptr;
    roomTree = new RoomsSchedule(); 
}

BuildingNode::~BuildingNode() {
    delete roomTree;
}

void CampusMap::removeEdgeHelper(BuildingNode* src, const string& destName) {
    if (src == nullptr) {
        return;
    }

    EdgeNode* current = src->adjList;
    EdgeNode* prev = nullptr;

    while (current != nullptr) {
        if (current->destBuildingName == destName) {
            if (prev == nullptr) {
                src->adjList = current->next;
            } else {
                prev->next = current->next;
            }
            EdgeNode* temp = current;
            current = current->next;
            delete temp;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void CampusMap::clearAllEdges(BuildingNode* building) {
    EdgeNode* current = building->adjList;
    while (current != nullptr) {
        EdgeNode* temp = current;
        current = current->next;
        delete temp;
    }
    building->adjList = nullptr;
}

CampusMap::CampusMap() {
    head = nullptr;
    numBuildings = 0;
}

CampusMap::~CampusMap() {
    BuildingNode* current = head;
    while (current != nullptr) {
        clearAllEdges(current);
        BuildingNode* temp = current;
        current = current->next;
        delete temp;
    }
}

BuildingNode* CampusMap::findBuilding(const string& name) const {
    BuildingNode* current = head;
    while (current != nullptr) {
        if (current->buildingName == name) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void CampusMap::addBuilding(const string& name) {
    if (findBuilding(name) != nullptr) {
        cout << "[ERROR] Building '" << name << "' already exists." << endl;
        return;
    }
    
    BuildingNode* newNode = new BuildingNode(name);
    newNode->next = head;
    head = newNode;
    numBuildings++;
    cout << "[SUCCESS] Building " << name << " added." << endl;
}

BuildingNode* CampusMap::removeBuilding(const string& name) {
    BuildingNode* current = head;
    BuildingNode* prev = nullptr;
    
    while (current != nullptr && current->buildingName != name) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "[ERROR] Building '" << name << "' not found." << endl;
        return nullptr;
    }
    
    BuildingNode* walker = head;
    while (walker != nullptr) {
        removeEdgeHelper(walker, name);
        walker = walker->next;
    }

    clearAllEdges(current);

    if (prev == nullptr) {
        head = current->next; 
    } else {
        prev->next = current->next;
    }
    
    numBuildings--;
    cout << "[SUCCESS] Building '" << name << "' removed from map." << endl;
    return current; 
}

void CampusMap::addPath(const string& src, const string& dest, const string& relation, const string& status, int weight) {
    BuildingNode* srcNode = findBuilding(src);
    BuildingNode* destNode = findBuilding(dest);

    if (srcNode == nullptr || destNode == nullptr) {
        cout << "[ERROR] Source or destination building not found." << endl;
        return;
    }
    
    EdgeNode* edge1 = new EdgeNode(dest, relation, status, weight);
    edge1->next = srcNode->adjList;
    srcNode->adjList = edge1;

    EdgeNode* edge2 = new EdgeNode(src, relation, status, weight);
    edge2->next = destNode->adjList;
    destNode->adjList = edge2;
    
    cout << "[SUCCESS] Path added between " << src << " and " << dest << endl;
}

void CampusMap::removePath(const string& src, const string& dest) {
    BuildingNode* srcNode = findBuilding(src);
    BuildingNode* destNode = findBuilding(dest);

    if (srcNode == nullptr || destNode == nullptr) {
        cout << "[ERROR] One or both buildings not found." << endl;
        return;
    }

    removeEdgeHelper(srcNode, dest);
    removeEdgeHelper(destNode, src);
    cout << "[SUCCESS] Path removed between " << src << " and " << dest << endl;
}

void CampusMap::displayMap() const {
    BuildingNode* current = head;
    cout << endl << "--- Campus Map (Adjacency List) ---" << endl;
    while (current != nullptr) {
        cout << current->buildingName << " -> ";
        EdgeNode* edge = current->adjList;
        while (edge != nullptr) {
            cout << "[" << edge->destBuildingName << ", " << edge->weight << "m] -> ";
            edge = edge->next;
        }
        cout << "NULL" << endl;
        current = current->next;
    }
}

BuildingNode* CampusMap::getBuildingHead() const {
    return head;
}

int CampusMap::getBuildingIndex(const string& name, string names[], int total) const {
    for (int i = 0; i < total; i++) {
        if (names[i] == name) return i;
    }
    return -1;
}

void CampusMap::BFS(const string& startBuilding) {
    if (findBuilding(startBuilding) == nullptr) {
        cout << "[ERROR] Start building not found." << endl;
        return;
    }

    string visited[100];
    int visitedCount = 0;
    string queue[100];
    int front = 0, rear = 0;

    queue[rear++] = startBuilding;
    visited[visitedCount++] = startBuilding;

    cout << endl << "--- BFS Traversal ---" << endl;
    while (front < rear) {
        string currentName = queue[front++];
        cout << currentName << " ";

        BuildingNode* b = findBuilding(currentName);
        if (b != nullptr) {
            EdgeNode* adj = b->adjList;
            while (adj != nullptr) {
                bool alreadyVisited = false;
                for (int i = 0; i < visitedCount; i++) {
                    if (visited[i] == adj->destBuildingName) {
                        alreadyVisited = true;
                        break;
                    }
                }
                if (!alreadyVisited) {
                    visited[visitedCount++] = adj->destBuildingName;
                    queue[rear++] = adj->destBuildingName;
                }
                adj = adj->next;
            }
        }
    }
    cout << endl;
}

void CampusMap::DFSHelper(BuildingNode* startNode, bool visited[], string buildingNames[], int count) {
    if (startNode == nullptr) return;

    int idx = getBuildingIndex(startNode->buildingName, buildingNames, count);
    visited[idx] = true;
    cout << startNode->buildingName << " ";

    EdgeNode* adj = startNode->adjList;
    while (adj != nullptr) {
        int neighborIdx = getBuildingIndex(adj->destBuildingName, buildingNames, count);
        if (neighborIdx != -1 && !visited[neighborIdx]) {
            DFSHelper(findBuilding(adj->destBuildingName), visited, buildingNames, count);
        }
        adj = adj->next;
    }
}

void CampusMap::DFS(const string& startBuilding) {
    BuildingNode* start = findBuilding(startBuilding);
    if (start == nullptr) {
        cout << "[ERROR] Start building not found." << endl;
        return;
    }

    string buildingNames[100];
    bool visited[100];
    int count = 0;

    BuildingNode* temp = head;
    while (temp != nullptr) {
        buildingNames[count] = temp->buildingName;
        visited[count] = false;
        count++;
        temp = temp->next;
    }

    cout << endl << "--- DFS Traversal ---" << endl;
    DFSHelper(start, visited, buildingNames, count);
    cout << endl;
}

void CampusMap::shortestPath(const string& startBuilding, const string& endBuilding) {
    if (findBuilding(startBuilding) == nullptr || findBuilding(endBuilding) == nullptr) {
        cout << "[ERROR] Buildings not found." << endl;
        return;
    }

    string names[50];
    int dist[50];
    bool visited[50];
    string prev[50];
    int count = 0;
    
    BuildingNode* temp = head;
    while (temp != nullptr) {
        names[count] = temp->buildingName;
        dist[count] = HUGE_DISTANCE;
        visited[count] = false;
        prev[count] = "";
        count++;
        temp = temp->next;
    }

    int startIdx = getBuildingIndex(startBuilding, names, count);
    dist[startIdx] = 0;

    for (int i = 0; i < count; i++) {
        int u = -1;
        int minVal = HUGE_DISTANCE;
        for (int j = 0; j < count; j++) {
            if (!visited[j] && dist[j] < minVal) {
                minVal = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        if (names[u] == endBuilding) break; 

        BuildingNode* uNode = findBuilding(names[u]);
        EdgeNode* edge = uNode->adjList;
        while (edge != nullptr) {
            int v = getBuildingIndex(edge->destBuildingName, names, count);
            if (v != -1 && !visited[v] && dist[u] != HUGE_DISTANCE && dist[u] + edge->weight < dist[v]) {
                dist[v] = dist[u] + edge->weight;
                prev[v] = names[u];
            }
            edge = edge->next;
        }
    }

    int endIdx = getBuildingIndex(endBuilding, names, count);
    if (dist[endIdx] == HUGE_DISTANCE) {
        cout << "No path exists between " << startBuilding << " and " << endBuilding << endl;
    } else {
        cout << "[RESULT] Shortest Distance: " << dist[endIdx] << "m" << endl;
        cout << "Path: ";
        string path[50];
        int pathLen = 0;
        string curr = endBuilding;
        while (curr != "") {
            path[pathLen++] = curr;
            if (curr == startBuilding) break;
            int cIdx = getBuildingIndex(curr, names, count);
            curr = prev[cIdx];
        }
        for (int k = pathLen - 1; k >= 0; k--) {
            cout << path[k];
            if (k > 0) cout << " -> ";
        }
        cout << endl;
    }
}

RoomNode::RoomNode(const string& id, int floor, const string& type) {
    roomID = id;
    floorNumber = floor;
    roomType = type;
    isReserved = false;
    reservedBy = "None";
    left = nullptr;
    right = nullptr;
    height = 1;
}

int RoomsSchedule::customMax(int a, int b) {
    if (a > b) return a;
    return b;
}

int RoomsSchedule::getHeight(RoomNode* node) const {
    if (node == nullptr) return 0;
    return node->height;
}

int RoomsSchedule::getBalance(RoomNode* node) const {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

void RoomsSchedule::updateHeight(RoomNode* node) {
    node->height = 1 + customMax(getHeight(node->left), getHeight(node->right));
}

RoomNode* RoomsSchedule::rotateRight(RoomNode* y) {
    RoomNode* x = y->left;
    RoomNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

RoomNode* RoomsSchedule::rotateLeft(RoomNode* x) {
    RoomNode* y = x->right;
    RoomNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

RoomNode* RoomsSchedule::insertRoomRecursive(RoomNode* node, const string& id, int floor, const string& type) {
    if (node == nullptr) {
        return new RoomNode(id, floor, type);
    }

    if (id < node->roomID) {
        node->left = insertRoomRecursive(node->left, id, floor, type);
    } else if (id > node->roomID) {
        node->right = insertRoomRecursive(node->right, id, floor, type);
    } else {
        cout << "[ERROR] Room ID " << id << " already exists." << endl;
        return node;
    }

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1) {
        if (id < node->left->roomID) {
            return rotateRight(node); 
        }
        if (id > node->left->roomID) {
            node->left = rotateLeft(node->left); 
            return rotateRight(node);
        }
    }

    if (balance < -1) {
        if (id > node->right->roomID) {
            return rotateLeft(node); 
        }
        if (id < node->right->roomID) {
            node->right = rotateRight(node->right); 
            return rotateLeft(node);
        }
    }

    return node;
}

RoomNode* RoomsSchedule::findMinNode(RoomNode* node) {
    RoomNode* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

RoomNode* RoomsSchedule::deleteRoomRecursive(RoomNode* rootNode, const string& id) {
    if (rootNode == nullptr) {
        return rootNode;
    }

    if (id < rootNode->roomID) {
        rootNode->left = deleteRoomRecursive(rootNode->left, id);
    } else if (id > rootNode->roomID) {
        rootNode->right = deleteRoomRecursive(rootNode->right, id);
    } else {
        if ((rootNode->left == nullptr) || (rootNode->right == nullptr)) {
            RoomNode* temp = rootNode->left ? rootNode->left : rootNode->right;
            if (temp == nullptr) {
                temp = rootNode;
                rootNode = nullptr;
            } else {
                rootNode->roomID = temp->roomID;
                rootNode->floorNumber = temp->floorNumber; 
                rootNode->roomType = temp->roomType;
                rootNode->isReserved = temp->isReserved;
                rootNode->reservedBy = temp->reservedBy;
                rootNode->left = temp->left;
                rootNode->right = temp->right;
                rootNode->height = temp->height;

                rootNode->left = nullptr;
                rootNode->right = nullptr;

                delete temp;
                temp = nullptr;
            }
        } else {
            RoomNode* temp = findMinNode(rootNode->right);
            rootNode->roomID = temp->roomID;
            rootNode->floorNumber = temp->floorNumber; 
            rootNode->roomType = temp->roomType;
            rootNode->isReserved = temp->isReserved;
            rootNode->reservedBy = temp->reservedBy;

            rootNode->right = deleteRoomRecursive(rootNode->right, temp->roomID);
        }
    }

    if (rootNode == nullptr) {
        return rootNode;
    }

    updateHeight(rootNode);
    int balance = getBalance(rootNode);

    if (balance > 1) {
        if (getBalance(rootNode->left) >= 0) {
            return rotateRight(rootNode); 
        } else {
            rootNode->left = rotateLeft(rootNode->left); 
            return rotateRight(rootNode);
        }
    }

    if (balance < -1) {
        if (getBalance(rootNode->right) <= 0) {
            return rotateLeft(rootNode); 
        } else {
            rootNode->right = rotateRight(rootNode->right); 
            return rotateLeft(rootNode);
        }
    }

    return rootNode;
}

void RoomsSchedule::printInorderRecursive(RoomNode* node) {
    if (node != nullptr) {
        printInorderRecursive(node->left);
        cout << node->roomID << " (Fl: " << node->floorNumber << ", Type: " << node->roomType << ", Rsvd: " << (node->isReserved ? "Yes" : "No") << ") | ";
        printInorderRecursive(node->right);
    }
}

void RoomsSchedule::printPreorderRecursive(RoomNode* node) {
    if (node != nullptr) {
        cout << node->roomID << " ";
        printPreorderRecursive(node->left);
        printPreorderRecursive(node->right);
    }
}

void RoomsSchedule::printPostorderRecursive(RoomNode* node) {
    if (node != nullptr) {
        printPostorderRecursive(node->left);
        printPostorderRecursive(node->right);
        cout << node->roomID << " ";
    }
}

void RoomsSchedule::searchRoomsByTypeRecursive(RoomNode* node, const string& type) {
    if (node != nullptr) {
        searchRoomsByTypeRecursive(node->left, type);
        if (node->roomType == type) {
            cout << "[MATCH] " << node->roomID << " (Floor: " << node->floorNumber << ")" << endl;
        }
        searchRoomsByTypeRecursive(node->right, type);
    }
}

void RoomsSchedule::deleteTreeRecursive(RoomNode* node) {
    if (node != nullptr) {
        deleteTreeRecursive(node->left);
        deleteTreeRecursive(node->right);
        delete node;
    }
}

void RoomsSchedule::cancelUserReservationsRecursive(RoomNode* node, const string& userName) {
    if (node != nullptr) {
        if (node->isReserved && node->reservedBy == userName) {
            node->isReserved = false;
            node->reservedBy = "None";
            cout << " Cancelled reservation for " << node->roomID << "." << endl;
        }
        cancelUserReservationsRecursive(node->left, userName);
        cancelUserReservationsRecursive(node->right, userName);
    }
}

RoomsSchedule::RoomsSchedule() {
    root = nullptr;
}

RoomsSchedule::~RoomsSchedule() {
    deleteTreeRecursive(root);
}

void RoomsSchedule::insertRoom(const string& id, int floor, const string& type) {
    root = insertRoomRecursive(root, id, floor, type);
    cout << "[SUCCESS] Room " << id << " inserted." << endl;
}

void RoomsSchedule::deleteRoom(const string& id) {
    if (searchRoom(id) == nullptr) {
        cout << "[ERROR] Room not found." << endl;
        return;
    }
    root = deleteRoomRecursive(root, id);
    cout << "[SUCCESS] Room " << id << " deleted." << endl;
}

RoomNode* RoomsSchedule::searchRoom(const string& roomID) const {
    RoomNode* current = root;
    while (current != nullptr) {
        if (roomID < current->roomID) {
            current = current->left;
        } else if (roomID > current->roomID) {
            current = current->right;
        } else {
            return current;
        }
    }
    return nullptr;
}

void RoomsSchedule::searchRoomsByType(const string& type) {
    cout << endl << "--- Searching Rooms of Type: " << type << " ---" << endl;
    searchRoomsByTypeRecursive(root, type);
}

void RoomsSchedule::cancelAllUserReservations(const string& userName) {
    cancelUserReservationsRecursive(root, userName);
}

void RoomsSchedule::printInorder() {
    cout << endl << "--- Inorder Traversal ---" << endl;
    printInorderRecursive(root);
}

void RoomsSchedule::printPreorder() {
    cout << endl << "--- Preorder Traversal ---" << endl;
    printPreorderRecursive(root);
    cout << endl;
}

void RoomsSchedule::printPostorder() {
    cout << endl << "--- Postorder Traversal ---" << endl;
    printPostorderRecursive(root);
    cout << endl;
}

bool RoomsSchedule::reserveRoom(const string& roomID, const string& userName) {
    RoomNode* room = searchRoom(roomID);
    if (room == nullptr) {
        cout << "[ERROR] Room " << roomID << " not found." << endl;
        return false;
    }
    if (room->isReserved) {
        cout << "[ERROR] Room " << roomID << " is already reserved by " << room->reservedBy << "." << endl;
        return false;
    }
    
    room->isReserved = true;
    room->reservedBy = userName;
    cout << "[SUCCESS] Room " << roomID << " reserved by " << userName << "." << endl;
    return true;
}

bool RoomsSchedule::cancelReservation(const string& roomID) {
    RoomNode* room = searchRoom(roomID);
    if (room != nullptr && room->isReserved) {
        room->isReserved = false;
        room->reservedBy = "None";
        cout << "[SUCCESS] Reservation for " << roomID << " cancelled." << endl;
        return true;
    }
    cout << "[ERROR] No reservation found for this room." << endl;
    return false;
}

ComplaintNode::ComplaintNode(int id, const string& user, const string& bldg, const string& room, const string& desc, const string& time) {
    ticketID = id;
    raisedBy = user;
    building = bldg;
    roomID = room;
    description = desc;
    timestamp = time;
    status = "Pending";
    next = nullptr;
}

ComplaintQueue::ComplaintQueue() {
    head = nullptr;
    tail = nullptr;
    count = 0;
    nextTicketID = 1001;
}

ComplaintQueue::~ComplaintQueue() {
    while (head != nullptr) {
        ComplaintNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void ComplaintQueue::enqueue(const string& user, const string& bldg, const string& room, const string& desc, const string& time) {
    ComplaintNode* newComplaint = new ComplaintNode(nextTicketID++, user, bldg, room, desc, time);

    if (tail == nullptr) {
        head = newComplaint;
        tail = newComplaint;
    } else {
        tail->next = newComplaint;
        tail = newComplaint;
    }
    count++;
    cout << "[SUCCESS] Complaint submitted. Ticket ID: " << newComplaint->ticketID << endl;
}

ComplaintNode* ComplaintQueue::dequeue() {
    if (head == nullptr) {
        cout << "[ERROR] The complaint queue is empty." << endl;
        return nullptr;
    }

    ComplaintNode* processedComplaint = head;
    head = head->next;
    
    if (head == nullptr) {
        tail = nullptr;
    }

    processedComplaint->next = nullptr; 
    processedComplaint->status = "Processed";
    count--;
    cout << "[SUCCESS] Processing Ticket ID: " << processedComplaint->ticketID << endl;
    return processedComplaint;
}

ComplaintNode* ComplaintQueue::peek() const {
    return head;
}

void ComplaintQueue::removeComplaintsByBuilding(const string& bldg) {
    ComplaintNode* current = head;
    ComplaintNode* prev = nullptr;

    while (current != nullptr) {
        if (current->building == bldg) {
            if (prev == nullptr) {
                head = current->next;
                if (head == nullptr) tail = nullptr;
            } else {
                prev->next = current->next;
                if (prev->next == nullptr) tail = prev;
            }
            ComplaintNode* temp = current;
            current = current->next;
            delete temp;
            count--;
            cout << " Removed complaint " << temp->ticketID << " from building " << bldg << endl;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void ComplaintQueue::displayQueue() const {
    if (head == nullptr) {
        cout << endl << "--- No Pending Complaints ---" << endl;
        return;
    }
    
    cout << endl << "--- Pending Complaints (" << count << ") ---" << endl;
    ComplaintNode* current = head;
    while (current != nullptr) {
        cout << "ID: " << current->ticketID << " | Raised By: " << current->raisedBy 
             << " | Building: " << current->building << " | Desc: " << current->description << endl;
        current = current->next;
    }
}

MessageNode::MessageNode(const string& from, const string& to, const string& text, const string& time) {
    fromUser = from;
    toUser = to;
    messageText = text;
    timestamp = time;
    next = nullptr;
}

MessageStack::MessageStack() {
    top = nullptr;
    count = 0;
}

MessageStack::~MessageStack() {
    while (top != nullptr) {
        MessageNode* temp = top;
        top = top->next;
        delete temp;
    }
}

bool MessageStack::isEmpty() const {
    return top == nullptr;
}

void MessageStack::push(const string& from, const string& to, const string& text, const string& time) {
    MessageNode* newMessage = new MessageNode(from, to, text, time);
    newMessage->next = top;
    top = newMessage;
    count++;
}

MessageNode* MessageStack::pop() {
    if (isEmpty()) {
        cout << "[ERROR] Stack is empty." << endl;
        return nullptr;
    }
    MessageNode* poppedMessage = top;
    top = top->next;
    poppedMessage->next = nullptr; 
    count--;
    return poppedMessage;
}

MessageNode* MessageStack::peek() const {
    return top;
}

void MessageStack::displayStack(int limit) const {
    MessageNode* current = top;
    int displayed = 0;
    
    cout << endl << "--- Notification Stack (Recent First) ---" << endl;
    while (current != nullptr && displayed < limit) {
        cout << "[" << current->timestamp << "] " << current->fromUser << " to " << current->toUser << ": " << current->messageText << endl;
        current = current->next;
        displayed++;
    }
}

void MessageStack::displayConversation(const string& user1, const string& user2) const {
    MessageNode* current = top;
    bool found = false;
    cout << endl << "--- Conversation between " << user1 << " and " << user2 << " ---" << endl;
    while (current != nullptr) {
        if ((current->fromUser == user1 && current->toUser == user2) || 
            (current->fromUser == user2 && current->toUser == user1)) {
            cout << "[" << current->timestamp << "] " << current->fromUser << ": " << current->messageText << endl;
            found = true;
        }
        current = current->next;
    }
    if (!found) cout << "No messages found." << endl;
}