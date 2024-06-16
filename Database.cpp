//
//  Database.cpp
//  assignment2
//
//  Created by Wy 陈 on 01/01/2024.
//

#include <stdio.h>
#include "Database.h"


database::database(const string &accounts_filename,const string &videos_filename){
    
    this->account_file_name=accounts_filename;
    this->video_file_name=videos_filename;
    ifstream accounts_(this->account_file_name);
    if (!accounts_){
        cout<<"Fail to access accounts information!"<<endl;
        return;
    }
    
    string line;
    while (getline(accounts_,line)){
        
        line.erase(remove(line.begin(), line.end(), '\n'), line.end()); // windows and ios use different new line character, have to remove them first
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.size()>2){ // prevents other unknown and invisible symbols at the end of the file
            this->accounts.push_back(line);
        }
    }
    accounts_.close();
    
    line="";
    
    
    ifstream videos_(this->video_file_name);
    if (!videos_){
        cout<<"Fail to access video information!"<<endl;
        return;
    }
    while (getline(videos_,line)){
        
        line.erase(remove(line.begin(), line.end(), '\n'), line.end()); // windows and ios use different new line character, have to remove them first
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if(line.size()>2){// prevents other unknown and invisible symbols at the end of the file
            this->videos.push_back(line);
        }
        
    }
    videos_.close();

}



void database::update_accounts(const vector<string>&new_accounts){
    this->accounts=new_accounts;
}

void database::update_videos(const vector<string>&new_videos){
    this->videos=new_videos;
}

//vector<string> database::get_videos_info()const{
//    return videos;
//}
//
//vector<string> database::get_accounts_info()const{
//    return accounts;
//}

string database::get_video_by_id(const string &v_id){
    
    string result="";
    for (const string&i:videos){
        size_t finder=i.find(v_id);
        if (finder!=string::npos){
            result=i;
        }
    }
    return result;
    
}


deque<Creator>* database::get_channels(){
    
    deque<Creator>*all= new deque<Creator>;
    multimap<float, string>* m=nullptr;
    
    for (const string &i:accounts){ // traverse each line in accounts
        if (is_creator(i)){ // identify whether it stands for a creator account or not
            Creator temp=Creator(i);
            m=this->initialise_map(temp.getChannel());
            temp.set_map(*m); // initialise the map in order to calculate the score
            delete m;
            m=nullptr;
            all->push_back(temp);
        }
    }
   
    
    sort(all->begin(),all->end(),[](const Creator&a, const Creator&b){return a.getRatio()>b.getRatio();}); // sort the deque by descending order of the ratio of creators inside.
    
    
    deque<Creator>*result=new deque<Creator>;
    if (all->size()>10){
        for(int i=0;i<10;i++){
            result->push_back(all->at(i));
        } // copy the first ten element to the result
    }
    
    else {
        *result=*all;
    }
    
    delete all;
    all=nullptr;
    
    return result;
    
}

bool database::is_creator(const string &line)const{
    
    size_t last_space=line.rfind(' '); // locates the last space
    
    if (last_space==string::npos){ // if no space in line
        cout<<"Incorrect format of line!"<<endl;
        throw invalid_argument("Incorrect format of line!");
    }
    
    string substring=string(line.begin()+(last_space+1),line.end());// retrieves the content after the last space
    
    try{
        stof(substring); // see if the content after the last space is float
    }
    catch(const invalid_argument &e){
        return false; // The account that this line represents doesn't have likes and dislikes ratio, means that this account isn't a creator account
    }
    
    return true;
}


vector<video>* database::get_videos(const string &creator_name)const{
    
    
    vector<video>*result=new vector<video>;
    for (const string &i:videos){
        
        size_t last_comma=i.rfind(','); // locates the last comma
        string channel_name=string(i.begin()+(last_comma+1),i.end());
    
        if (channel_name==creator_name){
            video temp_v=video(i); // initialise the video object by the line
            result->push_back(temp_v);
        }
    }

    
    return result;
}

multimap<float, string>* database::initialise_map(const string&channel_name){
    
    vector<video>*video_of_the_channel=this->get_videos(channel_name); // get all videos of the channel by the get_videos method of class database
    multimap<float, string> *result=new multimap<float, string>;
    
    if (!video_of_the_channel->empty()){
        
        for (const video &i:*video_of_the_channel){
            
            string value=i.getId()+'#'+i.getName();
            result->insert(make_pair(i.getRate(),value));
            //for this multimap, in the assignment description, it says "You need to store these statistics about the video with a mechanism of your choice.", I don't know if I understood it correctly, but I think storing both video ID and name here instead of video name alone makes more sense because there are multiple videos with the same name under the same channel.
            // and, for the challenge, I reverse the key and the value, which video score is the key, and video id+name is the value, to let the avl tree in the map sort the videos, and I have defined a function "compare" in Creator.h to customize the sorting order.
        }
        
    }
    
    delete video_of_the_channel;
    video_of_the_channel=nullptr;
    
    return result;
    
}


string database::find_line(const string &username){
    
    
    for (const string &i:accounts){
    
        if (is_creator(i)){
            size_t first_space=i.find(' '); // locates first space
            size_t last_space=i.rfind(' '); // locates last space
            string name=string(i.begin()+(first_space+1),i.begin()+last_space); // for creators, the name is whatever between the first and last space.
           
            if (name==username){
                return i;
            }
        }
        
        else {
            size_t first_space=i.find(' '); // locates first space
            string name=string(i.begin()+(first_space+1),i.end()); // for users, what's before the first space is reg, and all content after the first space is username.
            if (name==username){
                return i;
            }
        }
    }
    
    return ""; // if not found, returns an empty string
}

queue<video>* database::get_keyword_videos(const string &keyword)const{
    
    queue<video>*result=new queue<video>;
    
    for (const string &i:videos){ //traverse in all lines of video
        
        video temp=video(i);// initialises the video object by the information of the line
        string name=temp.getName();
        string channel=temp.getChannel();
        string v_id=temp.getId();
        // see if the keyword is in the video id, name or channel name.
        // the reason why I don't directly check the entire line of info is because I don't want to include the number of duration of video, like or dislike number of the video. I consider they are irrelevant to searching. Otherwise for example if user inputs '38' the system will pop out with videos have duration 38 seconds, its weird.
        size_t finder_name=name.find(keyword);
        size_t finder_channel=channel.find(keyword);
        size_t finder_id=v_id.find(keyword);
        if (finder_name!=string::npos||finder_channel!=string::npos||finder_id!=string::npos){
            result->push(temp); // add to result
        }
        
        
    }
    
    return result;
}

int database::creator_id_generator(){
    
    string result=""; // string is easier to operate in this case
    srand((unsigned) time(NULL)); // random seed (initial value) should be placed here. It generates an initial value using the time based on the second count from Unix Epoch (1/1/1970, 00:00:00 UTC) to the moment this code runs. If placed in the loop might generate repeated number as the runtime is too fast.
    
    for (int i=0;i<8;i++){
        int random_num=rand()%10; // Generates a number between 0-9
        result+=to_string(random_num);
    }
    
    return stoi(result);
    
    

}

int database::user_id_generator(){
    int counter=0;
    for(const string &i:accounts){
        if (!is_creator(i)){
            counter++; // counts how many users are there
        }
    }
    
    return counter+1; // return the id for this one
}

void database::add_video(Creator&c){
    
    invalid_name:
    cin.ignore();
    string line;
    string temp;
    cout<<"Enter the name of video >>";
    getline(cin,temp);

    size_t finder = temp.find('#'); // I use this symbol to separate video name and id in the multimap of creator class, if video name contains this symbol might cause bugs

    if (finder!=string::npos){ // which means the string contains the symbol
        cout<<"Invalid name!"<<endl;
        goto invalid_name;
    }

    string name=temp; // store the name and use it later when updating the multimap in creator class
    line+=temp; // line will be "name"
    line+=','; // line will be "name,"

    Invalid_minutes_input:
    temp="";
    cout<<"How many minutes does the video have >>";
    cin>>temp;

    try{
        stoi(temp);
    }
    catch(const invalid_argument &e){
        cout<<"Invalid input for minutes."<<endl;
        goto Invalid_minutes_input;
    }
    
    if (stoi(temp)<0){
        goto Invalid_minutes_input;
    }

    line+=temp;// line will be "name,minutes"
    line+=':';// line will be "name,minutes:"

    Invalid_seconds_input:
    temp="";
    cout<<"How many seconds does the video have >>";
    cin>>temp;

    try{
        stoi(temp);
    }
    catch(const invalid_argument &e){
        cout<<"Invalid input for seconds."<<endl;
        goto Invalid_seconds_input;
    }
    
    if (stoi(temp)>59||stoi(temp)<0){
        goto Invalid_seconds_input;
    }

    line+=temp;// line will be "name,minutes:seconds"
    line+=",0,0,"; //line will be "name,minutes:seconds,0,0," because a new video has 0 like and 0 dislike
    line+=c.getChannel(); //line will be "name,minutes:seconds,0,0,channel name"
    
    int video_counter=videos.size()+1;
    
    // the existance of this part below is to make sure the uniqueness of video id. Imagine this case: if a creator added two videos, and the id will be video_1001 and video_1002, and if he deletes video_1001, now the amount of videos will be 1001 again, and, if he or another creator adds a new video, the id of that video will be video_1002 again. Without this part, there will be 2 videos with same id, or even more.
    
    generate_id:
    string video_id="video_"+to_string(video_counter);
    for (const string& i: videos){
        size_t comma=i.find(',');
        string sub=string(i.begin(),i.begin()+comma);
        if (sub==video_id){
            video_counter++;
            goto generate_id;
        }
    }
    
    this->videos.push_back(video_id+','+line);
    string multimap_value=video_id+'#'+name;
    c.add_new_Video(multimap_value); // this method updates the multimap and average score
    
    for (string &i:accounts){
        if (this->is_creator(i)){
            size_t first_space=i.find(' ');
            size_t last_space=i.rfind(' ');
            string name=string(i.begin()+(first_space+1), i.begin()+last_space);
            if (name==c.getChannel()){
                i=c.compress(); // update the new info of c
                break;
            }
        }
    }
    
    this->update_txt(1);// this method takes one integer argument, if input '1', it updates the accounts to account file, if input '2', it updates the videos to the video file.
    
    this->update_txt(2);  // this method takes one integer argument, if input '1', it updates the accounts to account file, if input '2', it updates the videos to the video file.
    
    cout<<' '<<endl;
    cout<<"Video added!"<<endl;
    cout<<' '<<endl;
}

void database::add_account(const string &info){
    this->accounts.push_back(info);
    this->update_txt(1); // this method takes one integer argument, if input '1', it updates the accounts to account file, if input '2', it updates the videos to the video file.
}

void database::manage_video(Creator &c){
    
    cout<<"Below are all your videos:"<<endl;
    cout<<' '<<endl;
    
    // The creator class is iterable and all the videos inside has already sorted by descending order depends on video score
    
    
    Creator::iterator it=c.begin(); // the iterator defined in creator class header file
    
    for (;it!=c.end();++it){ // traverse through creator class, and videos in the creator class are already sorted by descending order of their score
        
        size_t finder=(*it).second.find('#'); // i use this symbol to separate video ID and name in the multimap key
        string v_id=string((*it).second.begin(), (*it).second.begin()+finder); // video id is the content before '#'
        string name=string ((*it).second.begin()+(finder+1), (*it).second.end()); // video name is the content after '#'
        cout<<v_id<<", "<<name<<", "<<"score: "<<(*it).first<<". "<<endl;
        
    }
    
   
    cout<<' '<<endl;
    cout<<"Now you can:"<<endl;
    
    Invalid_input:
    string choice;
    cout<<"a) Rename a video"<<endl;
    cout<<"b) Reset likes/dislikes of a video"<<endl;
    cout<<"c) Delete a video"<<endl;
    cout<<"d) Exit to main menu"<<endl;
    cin>>choice;
    
    if (choice=="a"){
        
        string v_id;
        cout<<"Please enter the video ID of the video >>";
        cin>>v_id;
       
        if (!c.have_video(v_id)){
            cout<<"Video not found under your channel!"<<endl;
            goto Invalid_input;
        }
        
        Invalid_name:
        cin.ignore();
        string new_name;
        cout<<"Enter the new name of video, it cannot contain '#' and '_'. >>";
        getline(cin,new_name);
        
        size_t symbol_finder=new_name.find('#');
        if (symbol_finder!=string::npos){
            cout<<"Name containing illegal symbol!"<<endl; // I use '#' to separate video id and name in multimap of class creator
            goto Invalid_name;
        }
        
        symbol_finder=new_name.find('_');
        if (symbol_finder!=string::npos){
            cout<<"Name containing illegal symbol!"<<endl; // prevent creator give a name like "video_1"
            goto Invalid_name;
        }
        
        for (string &i:videos){
            size_t first_comma=i.find(',');
            string video_id=string(i.begin(),i.begin()+first_comma);
            if (video_id==v_id){
                video temp=video(i);
                temp.update_name(new_name);
                i=temp.compress();
                break;
            }
        }
        
        c.update_video_name(v_id, new_name);
        
        this->update_txt(1);//update account info to txt file
        this->update_txt(2);// update video info to txt file
        cout<<' '<<endl;
        cout<<"Video name changed!"<<endl;
        cout<<' '<<endl;
    }
    
    else if (choice=="b"){
        
        video_not_found:
        string v_id;
        cout<<"Please enter video id of the video >>";
        cin>>v_id;
        
        if (!c.have_video(v_id)){
            cout<<"Video not found under your channel!"<<endl;
            goto video_not_found;
        }
        
        string value=v_id;
        value+='#';
        
        for (string&i:videos){
            
            size_t first_comma=i.find(',');
            string video_id=string(i.begin(),i.begin()+first_comma);
            
            if(video_id==v_id){
                video temp=video(i);
                temp.reset_like_dislike();
                value+=temp.getName();
                i=temp.compress();
                break;
            }
            
        }
        
        c.addScore(0, value);
        
        for (string &i:accounts){
            if (this->is_creator(i)){
                size_t first_space=i.find(' ');
                size_t last_space=i.rfind(' ');
                string name=string(i.begin()+(first_space+1), i.begin()+last_space);
                if (name==c.getChannel()){
                    i=c.compress(); // update the new info of c
                    break;
                }
            }
        }
        
        
        
        this->update_txt(1);
        this->update_txt(2);
        cout<<' '<<endl;
        cout<<"Reset done!"<<endl;
        cout<<' '<<endl;
  
    }
    
    else if (choice=="c"){
        
        Invalid_video_id:
        
        string v_id;
        cout<<"Please enter the video id of the video >>";
        cin>>v_id;
        
        if (!c.have_video(v_id)){
            cout<<"Video not found under your channel!"<<endl;
            goto Invalid_video_id;
        }
        
        int counter=0;
        
        for (string&i:videos){
            
            size_t first_comma=i.find(',');
            string video_id=string(i.begin(),i.begin()+first_comma); 
            // Just in case of the situation that if a new creator named "video_1" or somthing similar and created some videos, so I check after extracting video id from the line.
            if (video_id==v_id){
                break;
            }
            
            counter++;
        }
        
        videos.erase(videos.begin()+counter);
        c.delete_video(v_id);
        
        for (string &i:accounts){
            if (this->is_creator(i)){
                size_t first_space=i.find(' ');
                size_t last_space=i.rfind(' ');
                string name=string(i.begin()+(first_space+1), i.begin()+last_space);
                if (name==c.getChannel()){
                    i=c.compress(); // update the new info of corresponding creator
                    break;
                }
            }
        }
        
        this->update_txt(1);
        this->update_txt(2);
        
        cout<<' '<<endl;
        cout<<"Video deleted!"<<endl;
        cout<<' '<<endl;
    }
    
    else if (choice=="d"){
        return;
    }
    
    else{
        cout<<"Incorrect input!"<<endl;
        goto Invalid_input;
    }
}


void database::add_like_or_dislike_to_video(const string &v_id,const int&decision){
    
    
    video v_to_process;
    
    for(string &i:videos){ // use normal reference here because I want to modify the actual value in videos
        
        
        size_t first_comma=i.find(','); //locates the first comma in the line
        string video_id=string(i.begin(),i.begin()+first_comma); // whats before the first comma is video name
        
        if (video_id==v_id){ // if their id match. no need to check for exception here because I already check before the method been called
            
            video temp=video(i); // this constructor initialise a video object by the line of information
            
            if(decision==1){
                temp.add_like(); // this is a method of class video which adds a like to the video object and update the score (ratio) of video
            }
            
            else {
                temp.add_dislike(); // this is a method of class video which adds a dislike to the video object and update the score (ratio) of video
            }
            
      
            
            v_to_process=temp;
            i=temp.compress(); // this is a method of class video which compresses the updated video object to a line of information. I use reference to define "i" so it updates the actual value in videos.
            break; // no need to check the rest of videos as the id is unique
        }
        
        
    }
    
    for (string &j:accounts){
        
        if (!this->is_creator(j)){ // this method identify if this line represents a creator.
            continue; // if this line doesn't represent a creator, skip.
        }
        
        size_t first_space=j.find(' '); //locates first space in the line
        size_t last_space=j.rfind(' '); // locates second space in the line
        string channel_name=string(j.begin()+(first_space+1),j.begin()+last_space); // name of creator is whatever between the first and last space
        
        
        if (channel_name==v_to_process.getChannel()){
            
            Creator temp_c=Creator(j);// initialise a creator object by the line of information. this is the new constructor I defined of class creator.
            temp_c.set_map(*(this->initialise_map(temp_c.getChannel()))); // set up the map in temp_c to re-calculate average score based on new ratio of video
            
            j=temp_c.compress(); // compress() is a method of class creator which converts the updated info to a line. I use reference to define "j" so it updates the actual value
        }
    }
    
    this->update_txt(1); //update account info to txt file
    this->update_txt(2); // update video info to txt file
 
}





void database::update_txt(const int&decision){
    
    if (decision==1){
        
        ofstream write(this->account_file_name);
        if (!write){
            cout<<"Fail to write to file!"<<endl;
            return;
        }
        
        for (string i:accounts){
            write<<i<<endl;
        }
        
        write.close();
    }
    
    else{
        ofstream write(this->video_file_name);
        
        if (!write){
            cout<<"Fail to write to file!"<<endl;
            return;
        }
        
        for (string i:videos){
            write<<i<<endl;
        }
        
        write.close();
    }
}




