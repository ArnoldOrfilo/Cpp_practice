//
//  Person.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <stdio.h>

#include <string>
#include "Person.h"

using namespace std;

Person::Person(const string &name)
{   this->name = name;
}

Person::Person(){
    
}
string Person::getName() const
{   return name;
}

void Person::changeName(const string &newName)
{   name = newName;
}



