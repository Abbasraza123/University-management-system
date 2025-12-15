#include "ModuleF.h"

UniversitySystem::UniversitySystem() {
    userDirectory.addUser("abbas.raza", "araza1", "student", "CS", "abbas@nu.edu");
    userDirectory.addUser("wasi.kamal", "wkamal2", "student", "ME", "wasi@nu.edu");

    campusMap.addBuilding("Main_Block");
    campusMap.addBuilding("Library");
    campusMap.addBuilding("Engineering_Lab");

    campusMap.addPath("Main_Block", "Library", "walkway", "open", 150);
    campusMap.addPath("Library", "Engineering_Lab", "road", "open", 500);
    
    BuildingNode* mainBlock = campusMap.findBuilding("Main_Block");
    if (mainBlock != nullptr) {
        mainBlock->roomTree->insertRoom("MB-101", 1, "classroom");
        mainBlock->roomTree->insertRoom("MB-102", 1, "lab");
        mainBlock->roomTree->insertRoom("MB-205", 2, "classroom");
        
        mainBlock->roomTree->reserveRoom("MB-101", "abbas.raza");
        userDirectory.updateLastBookedRoom("abbas.raza", "MB-101"); 
    }
    
    BuildingNode* library = campusMap.findBuilding("Library");
    if (library != nullptr) {
        library->roomTree->insertRoom("LIB-305", 3, "seminar");
    }

    complaintSystem.enqueue("wasi.kamal", "Engineering_Lab", "NONE", "Broken_AC", "2025-12-04");
    complaintSystem.enqueue("Abbas", "Main_Block", "MB-205", "Projector_not_working", "2025-12-04");

    notificationStack.push("System", "All", "Welcome to the UMS ", "2025-12-04");
    notificationStack.push("teacher.cs", "ahme", "Check the new lab hours", "2025-12-04");
}

void UniversitySystem::run() {
    int choice;
    do {
        cout << endl << "===========================================" << endl;
        cout << "          UNIVERSITY MANAGEMENT SYSTEM" << endl;
        cout <<   "      ===========================================" << endl;
        cout << " 1. User & Authentication (Hash Table)" << endl;
        cout << " 2. Campus Map (Graph)" << endl;
        cout << " 3. Rooms & Scheduling (AVL)" << endl;
        cout << " 4. Complaints (Queue)" << endl;
        cout << " 5. Messaging (Stacks)" << endl;
        cout << " 6. Exit" << endl;
        cout << "===========================================" << endl;
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cout << "[ERROR] Invalid input. Please enter a number." << endl;
            cin.clear();
            string temp;
            if (cin.peek() == '\n') cin.get();
            else cin >> temp;
            continue;
        }

        switch (choice) {
            case 1: userMenu(); break;
            case 2: campusMapMenu(); break;
            case 3: roomsMenu(); break;
            case 4: complaintMenu(); break;
            case 5: messagingMenu(); break;
            case 6: cout << "[EXIT] System shutting down. Goodbye!" << endl; break;
            default: cout << "[ERROR] Invalid choice. Please select 1-6." << endl; break;
        }
    } while (choice != 6);
}

void UniversitySystem::userMenu() {
    int choice;
    string user, pass, role, dept, email;
    do {
        cout << endl << "--- User Directory Menu ---" << endl;
        cout << " 1. Register New User" << endl;
        cout << " 2. Login" << endl;
        cout << " 3. Update Profile" << endl; 
        cout << " 4. Delete User " << endl;
        cout << " 5. Search User" << endl;
        cout << " 6. Display All Users" << endl;
        cout << " 7. Back" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); string temp; cin >> temp; choice = -1;
        }
        
        if (choice == 1) {
            cout << "Username: "; cin >> user;
            cout << "Password: "; cin >> pass;
            cout << "Role (student/teacher/admin): "; cin >> role;
            cout << "Department: "; cin >> dept;
            cout << "Email: "; cin >> email;
            userDirectory.addUser(user, pass, role, dept, email);
        } else if (choice == 2) {
            cout << "Username: "; cin >> user;
            cout << "Password: "; cin >> pass;
            if (userDirectory.authenticateUser(user, pass)) {
                cout << "[SUCCESS] Login successful. Role: " << userDirectory.getUserRole(user) << endl;
            } else {
                cout << "[FAIL] Authentication failed." << endl;
            }
        } else if (choice == 3) { 
            cout << "Username: "; cin >> user;
            cout << "New Password: "; cin >> pass;
            cout << "New Department: "; cin >> dept;
            if (userDirectory.updateProfile(user, pass, dept)) {
                cout << "[SUCCESS] Profile updated." << endl;
            } else {
                cout << "[ERROR] User not found." << endl;
            }
        } else if (choice == 4) {
            cout << "Username to delete : "; cin >> user;
            BuildingNode* current = campusMap.getBuildingHead();
            while(current != nullptr) {
                current->roomTree->cancelAllUserReservations(user);
                current = current->next;
            }
            if (userDirectory.deleteUser(user)) {
                cout << "[SUCCESS] User " << user << " deleted." << endl;
            }
        } else if (choice == 5) {
            cout << "Username to search: "; cin >> user;
            HashEntry* u = userDirectory.getUser(user);
            if (u != nullptr) {
                cout << "Found: " << u->userName << " | Role: " << u->role << " | Last Room: " << u->lastBookedRoom << endl;
            } else {
                cout << "User not found." << endl;
            }
        } else if (choice == 6) {
            userDirectory.displayAllUsers();
        } else if (choice != 7) {
            cout << "[INFO] Option " << choice << "  invalid." << endl;
        }
        
    } while (choice != 7);
}

void UniversitySystem::campusMapMenu() {
    int choice;
    string src, dest, rel, stat;
    int weight;

    do {
        cout << endl << "--- Campus Map Menu ---" << endl;
        cout << " 1. Add Building" << endl;
        cout << " 2. Remove Building " << endl;
        cout << " 3. Add Path between buildings" << endl;
        cout << " 4. Remove Path" << endl;
        cout << " 5. Find Shortest Path (Dijkstra)" << endl;
        cout << " 6. BFS from Building" << endl;
        cout << " 7. DFS from Building" << endl;
        cout << " 8. Display Adjacency List" << endl;
        cout << " 9. Back" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); string temp; cin >> temp; choice = -1;
        }

        if (choice == 1) {
            cout << "Building Name: "; cin >> src;
            campusMap.addBuilding(src);
        } else if (choice == 2) {
            cout << "Building Name to remove : "; cin >> src;
            BuildingNode* nodeToRemove = campusMap.removeBuilding(src);
            if (nodeToRemove != nullptr) {
                complaintSystem.removeComplaintsByBuilding(src);
                delete nodeToRemove; 
                cout << "[SUCCESS] Building " << src << " and associated data cleaned up." << endl;
            }
        } else if (choice == 3) {
            cout << "Source Building: "; cin >> src;
            cout << "Destination Building: "; cin >> dest;
            cout << "Relation (road/walkway): "; cin >> rel;
            cout << "Status (open/closed): "; cin >> stat;
            cout << "Distance (weight): "; cin >> weight;
            campusMap.addPath(src, dest, rel, stat, weight);
        } else if (choice == 4) {
            cout << "Source Building: "; cin >> src;
            cout << "Destination Building: "; cin >> dest;
            campusMap.removePath(src, dest);
        } else if (choice == 5) {
            cout << "Start Building: "; cin >> src;
            cout << "End Building: "; cin >> dest;
            campusMap.shortestPath(src, dest);
        } else if (choice == 6) {
            cout << "Start Building: "; cin >> src;
            campusMap.BFS(src);
        } else if (choice == 7) {
            cout << "Start Building: "; cin >> src;
            campusMap.DFS(src);
        } else if (choice == 8) {
            campusMap.displayMap();
        } else if (choice != 9) {
            cout << "[INFO] Option " << choice << "  invalid." << endl;
        }
        
    } while (choice != 9);
}

void UniversitySystem::roomsMenu() {
    int choice;
    string bldgName, id, type, user;
    int floor;
    
    cout << endl << "--- Rooms Menu ---" << endl;
    cout << "Enter Building Name to manage rooms: ";
    cin >> bldgName;
    
    BuildingNode* bldg = campusMap.findBuilding(bldgName);
    if (bldg == nullptr) {
        cout << "[ERROR] Building not found." << endl;
        return;
    }
    
    RoomsSchedule* roomMgr = bldg->roomTree;

    do {
        cout << endl << "--- Building: " << bldgName << " Rooms Menu ---" << endl;
        cout << " 1. Insert Room" << endl;
        cout << " 2. Delete Room" << endl;
        cout << " 3. Search Room" << endl;
        cout << " 4. Search Rooms by Type" << endl;
        cout << " 5. Reserve Room" << endl;
        cout << " 6. Cancel Reservation" << endl;
        cout << " 7. Print Inorder Traversal" << endl;
        cout << " 8. Print Preorder Traversal" << endl;
        cout << " 9. Print Postorder Traversal" << endl;
        cout << " 10. Back" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); string temp; cin >> temp; choice = -1;
        }
        
        if (choice == 1) {
            cout << "Room ID (e.g., MB-201): "; cin >> id;
            cout << "Floor Number: "; cin >> floor;
            cout << "Room Type: "; cin >> type;
            roomMgr->insertRoom(id, floor, type);
        } else if (choice == 2) {
            cout << "Room ID to delete: "; cin >> id;
            roomMgr->deleteRoom(id);
        } else if (choice == 3) {
            cout << "Room ID to search: "; cin >> id;
            RoomNode* r = roomMgr->searchRoom(id);
            if (r) cout << "Found Room: " << r->roomID << " Type: " << r->roomType << endl;
            else cout << "Room not found." << endl;
        } else if (choice == 4) {
            cout << "Room Type: "; cin >> type;
            roomMgr->searchRoomsByType(type);
        } else if (choice == 5) {
            cout << "Room ID to reserve: "; cin >> id;
            cout << "Your Username: "; cin >> user;
            if (userDirectory.getUser(user) == nullptr) { 
                cout << "[ERROR] User not found. Cannot reserve." << endl;
                continue;
            }
            if (roomMgr->reserveRoom(id, user)) {
                userDirectory.updateLastBookedRoom(user, id); 
                notificationStack.push("System", user, "Room " + id + " reserved successfully.", "now");
            }
        } else if (choice == 6) {
            cout << "Room ID to cancel: "; cin >> id;
            roomMgr->cancelReservation(id);
        } else if (choice == 7) {
            roomMgr->printInorder();
        } else if (choice == 8) {
            roomMgr->printPreorder();
        } else if (choice == 9) {
            roomMgr->printPostorder();
        } else if (choice != 10) {
            cout << "[INFO] Invalid option." << endl;
        }
        
    } while (choice != 10);
}

void UniversitySystem::complaintMenu() {
    int choice;
    string user, bldg, room, desc, time;
    do {
        cout << endl << "--- Complaint Ticketing Menu ---" << endl;
        cout << " 1. Submit New Complaint " << endl;
        cout << " 2. Process Next Complaint" << endl;
        cout << " 3. View Next Complaint (Peek)" << endl;
        cout << " 4. Display All Pending" << endl;
        cout << " 5. Back" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); string temp; cin >> temp; choice = -1;
        }
        
        if (choice == 1) {
            cout << "Your Username: "; cin >> user;
            cout << "Building: "; cin >> bldg;
            cout << "Room ID (if applicable, type NONE if not): "; cin >> room;
            cout << "Description (single line, no spaces): "; cin >> desc;
            
            if (userDirectory.getUser(user) == nullptr) { 
                cout << "[ERROR] User does not exist. Complaint rejected." << endl;
            } else if (campusMap.findBuilding(bldg) == nullptr) { 
                cout << "[ERROR] Building does not exist. Complaint rejected." << endl;
            } else {
                time = "2025-12-04_1000"; 
                complaintSystem.enqueue(user, bldg, room, desc, time);
            }

        } else if (choice == 2) {
            ComplaintNode* c = complaintSystem.dequeue();
            if (c != nullptr) {
                notificationStack.push("System", c->raisedBy, "Your complaint (" + c->roomID + ") was processed.", "now");
                delete c; 
            }
        } else if (choice == 3) {
            ComplaintNode* c = complaintSystem.peek();
            if (c != nullptr) {
                cout << "Next Ticket: " << c->ticketID << " (" << c->description << ")" << endl;
            } else {
                cout << "No pending complaints." << endl;
            }
        } else if (choice == 4) {
            complaintSystem.displayQueue();
        } else if (choice != 5) {
            cout << "[INFO] Invalid option." << endl;
        }
        
    } while (choice != 5);
}

void UniversitySystem::messagingMenu() {
    int choice;
    string from, to, msg;
    do {
        cout << endl << "--- Messaging & Notifications Menu ---" << endl;
        cout << " 1. Send Notification (Push to Stack)" << endl;
        cout << " 2. View Latest Notification (Peek)" << endl;
        cout << " 3. Pop Latest Notification" << endl;
        cout << " 4. Display Recent Notifications" << endl;
        cout << " 5. Display Conversation" << endl;
        cout << " 6. Back" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); string temp; cin >> temp; choice = -1;
        }
        
        if (choice == 1) {
            cout << "From User: "; cin >> from;
            cout << "To User/General: "; cin >> to;
            cout << "Message: "; cin >> msg;
            if (userDirectory.getUser(from) == nullptr) { 
                cout << "[ERROR] Sender not found. Message rejected." << endl;
                continue;
            }
            notificationStack.push(from, to, msg, "2025-12-04_1005");
            cout << "[SUCCESS] Notification sent." << endl;
        } else if (choice == 2) {
             MessageNode* m = notificationStack.peek();
             if (m) cout << "Latest: " << m->messageText << endl;
             else cout << "Stack empty." << endl;
        } else if (choice == 3) {
            MessageNode* m = notificationStack.pop();
            if (m != nullptr) {
                cout << "[POPPED] Message cleared: " << m->messageText << endl;
                delete m; 
            }
        } else if (choice == 4) {
            notificationStack.displayStack(5);
        } else if (choice == 5) {
            cout << "User 1: "; cin >> from;
            cout << "User 2: "; cin >> to;
            notificationStack.displayConversation(from, to);
        } else if (choice != 6) {
            cout << "[INFO] Invalid option." << endl;
        }
        
    } while (choice != 6);
}

int main() {
    UniversitySystem sys;
    sys.run();
    return 0;
}