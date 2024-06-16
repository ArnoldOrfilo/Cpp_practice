//
//  Creator.h
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#ifndef Creator_h
#define Creator_h

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "Person.h"
#include "Videos.h"



using namespace std;

class NoScoreException: public exception
{
public:
    NoScoreException(const string &msg){
        this->msg=msg;
    }
    
    const char* what()const noexcept override{ // without noexcept, the compiler shows "Exception specification of overriding function is more lax than base version"
        return msg.c_str(); // convert to char array
    }
private:
    string msg;
};

class Creator: public Person{

public:
    
    class iterator{
        
    public:
        
        explicit iterator(multimap<float, string>::iterator it){ // differenciate this iterator and iterator of multimap
            this->it=it;
        }
        
        iterator& operator++() { // define ++ operator
            ++it;
            return *this;
        }

               
        pair<const float, string>& operator*() const { // define * operator
            return *it;
        }
        
        bool operator!=(const iterator& other) const { // define != operator
            return it != other.it;
        }

        
    private:
        multimap<float, string>::iterator it;
        
    };
    
    struct compare { // comparing logic for the multimap 
        bool operator()(const float& a, const float& b) const {
            return a > b; // compares in descending order
        }
    };
    
    
    // constructor should initialise name and registration number using arguments
    Creator(const string &channel_name, int regNo);
    Creator(const string &channel_name, int regNo,float rate);
    Creator(const string &line); // initialise a video by a line of info
    
    Creator::iterator begin(); // returns an iterator points to the beginning of the multimap
    Creator::iterator end(); // returns an iterator points to the end of the multimap
    
    void creator_menu();

    void set_map(const multimap<float, string>&m); //initialises the map. The map input here has already sorted by video score in descending order, in the "initialise_map" method of class database.

    // method to return registration number
    int getRegNo() const;
    
    float getRatio()const; //method to return the ratio
    string getChannel()const;
    
    void add_new_Video(const string& v_id_and_name);
    // method to add the score to the map
    
    void delete_video(const string&video_id);
    
    
    void update_video_name(const string&v_id, const string&name); // updates corresponding video name in the multimap.
    bool have_video(const string&v_id); // identify if the creator has corresponding video or not
    // this method should be handy when updating the multimap
    void addScore(float score,string &value); // updates the score of a video.

    // method to update the accumulative score map
    // should throw NoRatioException if creator has no videos yet
    bool updateLikeDislikeRatio();
    
    string compress(); // compress the creator information into txt file format
    

private:
    int regNo;
    string channel_name;
    
    multimap<string,float> video_and_ratio; // the original multimap
    
    multimap<float,string,compare> videos_and_ratios;
    // for this multimap, I think storing both video ID and name as key here instead of video name alone makes more sense because there are multiple videos with the same name under the same channel. And, for the challenge, I reverse the key and the value, which video score is the key, and video id+name is the value, to let the avl tree in the map sort the videos by the compare function I written. 
    
    
    float like_dislike_ratio_total; // overall channel score

    // friend function to output details of creator to stream
    // output to be defined and formated by you
    // if the creator has no videos "has no videos" should be output instead of displaying the videos
    friend ostream& operator<<(ostream &str, const Creator &s);
};


#endif /* Creator_h */
