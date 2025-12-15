#ifndef MODULED_H
#define MODULED_H

#include "Setup.h"

struct ComplaintNode {
    int ticketID;
    string raisedBy;
    string building;
    string roomID;
    string description;
    string timestamp;
    string status; 

    ComplaintNode* next;

    ComplaintNode(int id, const string& user, const string& bldg, const string& room, const string& desc, const string& time);
};

class ComplaintQueue {
private:
    ComplaintNode* head; 
    ComplaintNode* tail; 
    int count;
    int nextTicketID;

public:
    ComplaintQueue();
    ~ComplaintQueue();

    void enqueue(const string& user, const string& bldg, const string& room, const string& desc, const string& time);
    ComplaintNode* dequeue();
    ComplaintNode* peek() const;
    void removeComplaintsByBuilding(const string& bldg);
    void displayQueue() const;
};

#endif