//
//  Person.h
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#ifndef Person_h
#define Person_h
#include <string>

using namespace std;

class Person
{   public:
    Person(const string &name);
    Person();
    string getName() const;
    void changeName(const string &newName);
    protected:
       string name;
};

#endif /* Person_h */

