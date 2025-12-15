#ifndef MODULEF_H
#define MODULEF_H

#include "ModuleA.h"
#include "ModuleB.h"
#include "ModuleC.h"
#include "ModuleD.h"
#include "ModuleE.h"

class UniversitySystem {
public:
    PeopleDirectory userDirectory;
    CampusMap campusMap;
    ComplaintQueue complaintSystem;
    MessageStack notificationStack;

    UniversitySystem();
    void run();

private:
    void userMenu();
    void campusMapMenu();
    void roomsMenu();
    void complaintMenu();
    void messagingMenu();
};

#endif