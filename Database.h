//
//  Database.h
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#ifndef Database_h
#define Database_h

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <vector>
#include <map>
#include <queue>
#include <deque>

#include "Videos.h"
#include "Creator.h"

using namespace std;

class database{
private:
    
    vector<string>accounts;
    vector<string>videos;
    string account_file_name;
    string video_file_name;
    
public:
    
    database(const string &accounts_filename,const string &videos_filename); // read the files and initialise the two vectors
    void update_accounts(const vector<string>&new_accounts); // update the accounts vector
    void update_videos(const vector<string>&new_videos); // update the video vector
    string get_video_by_id(const string&v_id); // returns a string stands for corresponding video by its id
//    vector<string> get_accounts_info()const; // returns the vector includes all accounts
//    vector<string> get_videos_info()const; // returns the vector includes all videos
    deque<Creator>* get_channels(); // returns the vector includes all creators (channels)
    bool is_creator(const string&line)const; // identify whether the line represents a creator account or an user account. The checking method I set doesn't matter how many spaces or no space in the name of account
    vector<video>* get_videos(const string&creator_name)const; // returns a vector containing all the videos of one specific creator
    multimap<float, string>*initialise_map(const string&channel_name); // returns a map of type string and float
    string find_line(const string&username); // returns the specific line which stands for the corresponding account of the username. Returns empty string if not found.
    queue<video>* get_keyword_videos(const string &keyword)const; // returns a queue of videos including the keyword
    int creator_id_generator(); //returns an 8 digits random unique id
    int user_id_generator(); //returns a number base on how many users plus one as the id of user, which is the pattern shows in the txt file
    void add_account(const string &info); //add an account
    void add_video(Creator&c); //add a video, update the creator, write to txt file
    void manage_video(Creator &c); // manage videos for creator
    void add_like_or_dislike_to_video(const string &v_id,const int&decision); // identify the decision of like and dislike, add like or dislike to corresponding video and update the channel score to corresponding channel. if decision==1, add a like, if decision==2, add a dislike.
    void update_txt(const int&decision); // update the info to txt files. if the decision equals to 1, update account file, if decision equals to 2, update video file.

};
#endif /* Database_h */

