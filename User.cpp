//
//  User.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include"User.h"



User::User(const string &name, int regNo):Person(name){
    this->name=name;
    this->regNo=regNo;
}

User::User(const string&line){
    size_t first_space=line.find(' ');
    int reg=stoi(string(line.begin(),line.begin()+first_space));
    string name=string(line.begin()+(first_space+1),line.end()); // for users, what's before the first space is reg, and all content after the first space is username.
    this->name=name;
    this->regNo=reg;
    
}

int User::getRegNo()const{
    return this->regNo;
}

string User::get_Name()const{
    return this->name;
}


string User::compress(){
  
    string reg="";
    
    if (to_string(this->regNo).length()<4){
        
        int difference=4-to_string(this->regNo).length(); // make it in "0001" format if less than 4 digits
        
        for (int i=0;i<difference;i++){
            reg+='0';
        }
        
        reg+=to_string(this->regNo);
    }
    
    else{
        
        reg=to_string(this->regNo);
        
    }
    
    return reg+' '+this->name;

}


