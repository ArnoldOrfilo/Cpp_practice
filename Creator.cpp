//
//  Creator.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <stdio.h>
#include "Creator.h"


Creator::Creator(const string &channel_name, int regNo):Person(channel_name){
    this->channel_name=channel_name;
    this->regNo=regNo;
    
}

Creator::Creator(const string &channel_name, int regNo,float rate):Person(channel_name){
    this->channel_name=channel_name;
    this->regNo=regNo;
    this->like_dislike_ratio_total=rate;
}

Creator::Creator(const string &line){
    
    size_t first_space=line.find(' ');
    size_t last_space=line.rfind(' ');
    
    int reg=stoi(string(line.begin(),line.begin()+first_space));// whats before the first space is the reg.
    string name=string(line.begin()+(first_space+1),line.begin()+last_space); // for creators, the name is whatever between the first and last space.
    this->regNo=reg;
    this->name=name;
    this->channel_name=name;
    
}

Creator::iterator Creator::begin(){
    return Creator::iterator(videos_and_ratios.begin());
}

Creator::iterator Creator::end(){
    return Creator::iterator(videos_and_ratios.end());
}

void Creator::set_map(const multimap<float, string>&m){
        
    multimap<float, string>::const_iterator i=m.begin();
    while (i!=m.end()){
        
        videos_and_ratios.insert(*i); // insert the content in m to the customised multimap which stores in descending order, because I cannot use "=" to copy two multimap with different comparing logic
        i++;
    }
    
    
    if (!m.empty()){ // if this channel has videos
        float sum_rate=accumulate(videos_and_ratios.begin(),videos_and_ratios.end(),0.0f,[](float sum,const pair<float, string>&p){return sum+p.first;});// calculate the sum of ratio for all videos by the way defined in this lambda function
        this->like_dislike_ratio_total=sum_rate/(float)videos_and_ratios.size();
    }
    
    else {
        this->like_dislike_ratio_total=0;
    }
 

}


int Creator::getRegNo()const{
    return regNo;
}

float Creator::getRatio()const{
    return like_dislike_ratio_total;
}

string Creator::getChannel()const{
    return this->channel_name;
}
    
void Creator::add_new_Video(const string &v_id_and_name){
    
    videos_and_ratios.insert(make_pair(0,v_id_and_name));// a new video has 0 score
    
    if (videos_and_ratios.size()>1){ // this method is only for adding new video. if its the first video of creator, there's no need to update the channel score here because it must be zero.
        float sum_rate=accumulate(videos_and_ratios.begin(),videos_and_ratios.end(),0.0f,[](float sum,const pair<const float, string>&p){return sum+p.first;});// calculate the sum of ratio for all videos by the way defined in this lambda function
        this->like_dislike_ratio_total=sum_rate/(float)videos_and_ratios.size(); // update average score
    }
    
}

void Creator::delete_video(const string &video_id){
    
    multimap<float,string>::iterator i;
    for (i=videos_and_ratios.begin();i!=videos_and_ratios.end();i++){
        size_t finder=i->second.find('#');
        string v_id=string(i->second.begin(),i->second.begin()+finder);
        
        if (v_id==video_id){
            videos_and_ratios.erase(i); 
            break;
        }
    }
    
    float sum_rate=accumulate(videos_and_ratios.begin(),videos_and_ratios.end(),0.0f,[](float sum,const pair<const float, string>&p){return sum+p.first;});// calculate the sum of ratio for all videos by the way defined in this lambda function
    this->like_dislike_ratio_total=sum_rate/(float)videos_and_ratios.size();
    
}


void Creator::update_video_name(const string&v_id, const string&name){
    
    multimap<float,string>::iterator i;
    for (i=videos_and_ratios.begin();i!=videos_and_ratios.end();i++){
        string value=i->second;
        size_t finder=value.find('#'); // this is the symbol I use to separate id ane name in value.
        string video_id=string(value.begin(),value.begin()+finder);
        
        if (video_id==v_id){
            i->second=v_id+'#'+name;
        }
    }
}

bool Creator::have_video(const string&v_id){
    
    multimap<float,string>::iterator i;
    for (i=videos_and_ratios.begin();i!=videos_and_ratios.end();i++){
        string value=i->second;
        size_t finder=value.find('#'); // this is the symbol I use to separate id ane name in value.
        string video_id=string(value.begin(),value.begin()+finder);
        if (video_id==v_id){
            return true;
        }
    }
    
    return false;
}

void Creator::addScore(float score, string &value){
    //for this multimap, in the assignment description, it says "You need to store these statistics about the video with a mechanism of your choice.", I don't know if I understood it correctly, but I think storing video ID here instead of video name makes more sense because there are multiple videos with the same name under the same channel.
    
    // and, I use video score as key and video id+name as value, because it this case it will sort the videos by score automatically by the avl tree in map
    
    multimap<float,string>::iterator i;
    for (i=videos_and_ratios.begin();i!=videos_and_ratios.end();i++){
        if (i->second==value){
            videos_and_ratios.erase(i); // delete the old pair. Because the key is score, and key is constant value
            break;
        }
    }
    
    videos_and_ratios.insert(make_pair(score, value)); // insert the new pair with new score
    
    
    float sum=0; // sum of ratio
    int counter=0; // amount of video
    
    
    for (const auto&p:videos_and_ratios){
        counter++;
        sum+=p.first;
    }
    
    like_dislike_ratio_total=sum/(float)counter;
}

bool Creator::updateLikeDislikeRatio(){
    
    try{
        
        float sum_rate=accumulate(videos_and_ratios.begin(),videos_and_ratios.end(),0.0f,[](float sum,const pair<const float, string>&p){return sum+p.first;});// calculate the sum of ratio for all videos by the way defined in this lambda function
        this->like_dislike_ratio_total=sum_rate/(float)videos_and_ratios.size();
        
        throw NoScoreException("Creator doesn't have any video yet!");
    }
    
    catch(const NoScoreException &e){
        return false;
    }
    
    return true;
    
}

string Creator::compress(){
    return to_string(this->regNo)+' '+this->channel_name+' '+to_string(this->like_dislike_ratio_total);
}




