#ifndef MODULEA_H
#define MODULEA_H

#include "Setup.h"

struct HashEntry {
    string userName;       
    string password;
    string role;           
    string department;
    string email;
    string lastBookedRoom; 

    HashEntry* next; 

    HashEntry(const string& user, const string& pass, const string& r, 
              const string& dept, const string& mail);
};

class PeopleDirectory {
private:
    static const int TABLE_CAPACITY = 100; 
    HashEntry* table[TABLE_CAPACITY]; 
    int size;                         

    int hashFunction(const string& key) const;

public:
    PeopleDirectory();
    ~PeopleDirectory();

    HashEntry* getUser(const string& userName) const;
    bool addUser(const string& userName, const string& password, const string& role, 
                 const string& department, const string& email);
    bool authenticateUser(const string& userName, const string& password) const;
    void updateLastBookedRoom(const string& userName, const string& roomID);
    bool updateProfile(const string& userName, const string& newPass, const string& newDept);
    string getUserRole(const string& userName) const;
    bool deleteUser(const string& userName);
    void displayAllUsers() const;
};

#endif