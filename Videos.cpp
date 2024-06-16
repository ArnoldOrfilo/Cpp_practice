//
//  Videos.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <stdio.h>
#include <vector>
#include "Videos.h"
using namespace std;

video::video(const string &v_id,const string &name, const string &channel_name, const int &duration_minutes,const int&duration_seconds,const int &likes, const int &dislikes){
    
    this->name=name;
    this->v_id=v_id;
    this->channel_name=channel_name;
    this->duration_minute=duration_minutes;
    this->duration_second=duration_seconds;
    this->likes=likes;
    this->dislikes=dislikes;
    
    if(dislikes==0){
        this->rate=likes;
    }
    else if (this->likes==0){
        this->rate=0-dislikes;
    }
    else{
        this->rate=(float)likes/(float)dislikes;
    }
    
}


video::video(){
    // empty constructor
}


video::video(const string &line){
    

    vector <string> attributes;
    string temp="";
   
    
    for (char i:line){
        
        if (i==','){
           
            attributes.push_back(temp); // stores whatever before the first comma and between commas
            temp="";
            continue;
        }
    
        temp+=i;
        
    }
    
    attributes.push_back(temp); // the content after the last comma
    
    if (attributes.size()!=6){ // check format
        cout<<"Wrong format!"<<endl;
        
        return;
    }
    
    size_t colon=attributes[2].find(':');// locates the colon in duration
    if (colon==string::npos){ // if colon not found in duration
        cout<<"Wrong format!"<<endl;
        
        return;
    }
    
    this->duration_minute=stoi(string(attributes[2].begin(),attributes[2].begin()+colon)); // minute equals to whats before the colon in this substring
    this->duration_second=stoi(string(attributes[2].begin()+(colon+1),attributes[2].end())); // second equals to whats after the colon
    
    
    try {
        stoi(attributes[3]); // checks if like is an integer
    }
    catch(const invalid_argument &e){
        cout<<"Wrong format!"<<endl;
        
        return;
    }
    
    try {
        stoi(attributes[4]); // checks if dislike is an integer
    }
    catch(const invalid_argument &e){
       
        cout<<"Wrong format!"<<endl;
        return;
    }
    
    
    this->v_id=attributes[0];
    this->name=attributes[1];
    this->likes=stoi(attributes[3]);
    this->dislikes=stoi(attributes[4]);
    this->channel_name=attributes[5];
    
    if(this->dislikes!=0&&this->likes!=0){
        this->rate=(float)this->likes/(float)this->dislikes;
    }
    
    else if (this->likes==0){
        this->rate=0-dislikes;
    }
    else{
        this->rate=likes;
    }
    
}



void video::add_like(){
    this->likes++;
    if (this->dislikes!=0){
        this->rate=(float)this->likes/(float)this->dislikes;
    }
    else {
        this->rate=this->likes;
    }
}

void video::add_dislike(){
    this->dislikes++;
    if(this->likes!=0){
        this->rate=(float)this->likes/(float)this->dislikes;
    }
    else {
        this->rate=0-this->dislikes;
    }
}

string video::getChannel()const{
    return this->channel_name;
}

float video::getRate()const{
    return this->rate;
}

string video::getName()const{
    return this->name;
}

string video::getId()const{
    return this->v_id;
}

string video::compress(){
    return this->v_id+','+this->name+','+to_string(this->duration_minute)+':'+to_string(this->duration_second)+','+to_string(this->likes)+','+to_string(this->dislikes)+','+this->channel_name;
}

void video::update_name(const string &new_name){
    this->name=new_name;
}

void video::reset_like_dislike(){
    this->likes=0;
    this->dislikes=0;
    this->rate=0;
}

ostream&operator<<(ostream&output, const video &v){
    
    output<<"Video ID: "<<v.v_id<<endl;
    output<<"Video name: "<<v.name<<endl;
    output<<"Channel: "<<v.channel_name<<endl;
    output<<"Duration: "<<v.duration_minute<<" minute(s), "<<v.duration_second<<" second(s)."<<endl;
    output<<v.likes<<" Like(s) / "<<v.dislikes<<" Dislike(s)"<<endl;
    return output;
    
}

//video_1,Educational Insights,34:42,8960,623,Book Club


