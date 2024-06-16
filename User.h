//
//  User.h
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#ifndef User_h
#define User_h

#include <string>
#include <stdexcept>
#include <algorithm>
#include "Person.h"


using namespace std;

class User: public Person
{   public:
    // constructor should initialise name and registration number using arguments
    // and initialise score map to be empty
    User(const string &name, int regNo);
    User(const string &line); //initialise an User by a line of info, and initialise the map with creators info
    // method to return registration number
    int getRegNo() const;
    string get_Name() const;
    string compress(); // return a string in the txt file format
    
    
   
    private:
    
    int regNo;
    string name;
    
};


#endif /* User_h */
