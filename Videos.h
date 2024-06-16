//
//  Videos.h
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#ifndef Videos_h
#define Videos_h
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class video{
    
private:
    string v_id;
    string name;
    string channel_name;
    int duration_minute;
    int duration_second;
    int likes;
    int dislikes;
    float rate;
    
public:
    video();
    video(const string&line); //initialise the video object by its line
    video(const string &v_id,const string &name, const string &channel_name, const int &duration_minutes,const int&duration_seconds, const int &likes, const int &dislikes);// constructor for creating new video
    string compress();// compress the information to a line ##### Haven't done it yet!
    void add_like(); // add a like to the video
    void add_dislike(); //add a dislike to the video
    string getChannel()const; //returns the channel name of the video
    float getRate()const;// returns the rate of the video
    string getName()const;//returns the name of video
    string getId()const; //returns the id of video
    void update_name(const string&new_name); // updates video name
    void reset_like_dislike(); // reset like and dislike to zero
    friend ostream &operator<<(ostream&output, const video &v); // output video info
     
    
    
};

#endif /* Videos_h */
