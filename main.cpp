//
//  main.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Videos.h"
#include "User.h"
#include "Creator.h"
#include "Database.h"
#include "Main_menu.h"


template <class T>
T login(const string &line, const bool &is_creator_) {
    
    if (is_creator_){
        
        T temp=T(line);
        return temp;
    }
    
    else {
        
        T temp=T(line);
        return temp;
    }
    
    
}

int main(int argc, const char * argv[]) {
  

    string accounts_file="users.txt";
    
    string videos_file="fake_youtube_data_updated.txt";
    
    database *d=new database(accounts_file, videos_file);
    
    
    Username_enter:
    string username;
    cout<<"Please enter your username: ";
    getline(cin,username); // if I use cin<< directly, the space symbol and stuff will be left in buffer.
    string line=d->find_line(username);
    
    if (line==""){
        
        input_incorrect_:
        string choice;
        cout<<"Username not found. Create new account? "<<endl; // if the code runs to here means the username doesn't exist
        cout<<"1. Yes, I want to register as creator."<<endl;
        cout<<"2. Yes, I want to register as a normal user."<<endl;
        cout<<"3. No, back to login."<<endl;
        cin>>choice;
        
        if (choice=="1"){
            Creator c=Creator(username, d->creator_id_generator(),0);
            d->add_account(c.compress());
            d->update_txt(1);
            
            main_menu menu=main_menu(c,*d);
            
        }
        
        else if (choice=="2"){
            
            User u=User(username, d->user_id_generator());
            d->add_account(u.compress());
            d->update_txt(1);
            
            main_menu menu=main_menu(u,*d);
            
        }
        
        else if (choice=="3"){
            cin.ignore(); // ignore the newline character for the getline to take new input
            goto Username_enter;
        }
        
        else{
            cout<<"Incorrect input!"<<endl;
            goto input_incorrect_;
        }
        
        
   }
    else{
        
        bool is_creator_=d->is_creator(line); // checks if the line represents a creator or not. The checking method I set is to see if the content after the last space in that line is a float.
        if (is_creator_){
            Creator c=login<Creator>(line,is_creator_);
            multimap<float,string>* m=d->initialise_map(c.getChannel());
            c.set_map(*m); // initialise the map in descending order of video score
            delete m;
            m=nullptr;
            main_menu menu=main_menu(c, *d);
        }
        
        else {
            User u=login<User>(line,is_creator_);
            main_menu menu=main_menu(u, *d);
        }
        
        
        
    }
    
    
    delete d;
    d=nullptr;
    
    
    
   
    
    
    return 0;
}
