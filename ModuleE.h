#ifndef MODULEE_H
#define MODULEE_H

#include "Setup.h"

struct MessageNode {
    string fromUser;
    string toUser;
    string messageText;
    string timestamp;

    MessageNode* next; 

    MessageNode(const string& from, const string& to, const string& text, const string& time);
};

class MessageStack {
private:
    MessageNode* top;
    int count;

public:
    MessageStack();
    ~MessageStack();

    bool isEmpty() const;
    void push(const string& from, const string& to, const string& text, const string& time);
    MessageNode* pop();
    MessageNode* peek() const;
    void displayStack(int limit) const;
    void displayConversation(const string& user1, const string& user2) const;
};

#endif