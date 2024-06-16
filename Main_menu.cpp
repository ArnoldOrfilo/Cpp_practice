//
//  Main_menu.cpp
//  assignment2
//
//  Created by Wy 陈 on 10/01/2024.
//

#include <stdio.h>
#include "Main_menu.h"

void printQueue(queue<video>q){ // function to print the content of queue. I directly pass the value here so what I do inside the function won't affect the actual queue object.
    int q_len=q.size(); // have to record the size of queue first because .pop will affect the length of queue
    cout<<' '<<endl;
    for (int i=0;i<q_len;i++){
        cout<<i+1<<" - "; // add the number at the beginning for the user to choose. start from 1.
        cout<<q.front().getId()<<", "<<q.front().getName()<<" by "<<q.front().getChannel()<<endl;
        
        q.pop();
    }
}

void showTopTen(list<video>video_list){
    
    
    
    if(video_list.size()>10){
        
        for (int i=0;i<10;i++){
            cout<<i+1<<" - "<<video_list.front().getId()<<", "<<video_list.front().getName()<<" Created by "<<video_list.front().getChannel()<<"."<<endl;
            video_list.pop_front();
        }
    }
    else {
        int list_len=video_list.size();
        for (int i=0;i<list_len;i++){
            cout<<i+1<<" - "<<video_list.front().getId()<<", "<<video_list.front().getName()<<" Created by "<<video_list.front().getChannel()<<"."<<endl;
            video_list.pop_front();
        }
    }
    
}

main_menu::main_menu(User&u, database&d){
    user_menu(d, u);
}


void main_menu::user_menu(database &d, User &u){
    while (true){
        incorrect_option:
        string option;
        
        cout<<"Welcome, "<<u.get_Name()<<","<<endl;
        cout<<' '<<endl;
        cout<<"1 - Get video by keyword"<<endl;
        cout<<"2 - Get top 10 video recommendations by keyword"<<endl;
        cout<<"3 - Get top 10 channels (creators) overall"<<endl;
        cout<<"4 - Exit the program"<<endl;
        cin>>option;
        
        if (option=="1"){
            
            string keyword;
            cin.ignore(); // ignore the new line charactor left in the last cin inside buffer
            cout<<"Please enter the keyword >>";
            getline(cin,keyword); // using cin directly will left the space or new line characters in buffer
            this->get_video(keyword, d);
            
        }
        
        else if (option=="2"){
            
            string keyword;
            cin.ignore();// ignore the new line charactor left in the last cin inside buffer
            cout<<"Please enter the keyword >>";
            getline(cin,keyword); // using cin directly will left the space or new line characters in buffer
            this->get_video_recommendation(keyword, d);
        }
        
        else if (option=="3"){
            this->top_channels(d);
        }
        
        else if (option=="4"){
            return;
        }
        
        else {
            cout<<"Invalid option!"<<endl;
            goto incorrect_option;
        }
    }
}

main_menu::main_menu(Creator&c, database&d){
    
    while (true){
        
        invalidOption:
        
        string choice;
        cout<<"Welcome, "<<c.getChannel()<<endl;
        cout<<"Your current channel score: "<<c.getRatio()<<endl;
        cout<<' '<<endl;
        cout<<"1 - Create a video"<<endl;
        cout<<"2 - Manage a video"<<endl;
        cout<<"3 - Exit the program"<<endl;
        cin>>choice;
        if (choice=="1"){
            d.add_video(c);
        }
        
        else if (choice=="2"){
            d.manage_video(c);
        }
        else if (choice=="3"){
            return;
        }
        
        else {
            cout<<"Invalid input!"<<endl;
            goto invalidOption;
        }
        
    }
    
}

void main_menu::get_video(const string &keyword, database &d){
    
    queue<video>*result=d.get_keyword_videos(keyword);// This method of class database returns a queue of videos which containing the keyword.

   
    if (result->size()==0){ // No matches, returns to menu
        cout<<"No matched result."<<endl;
        return;
    }
    
    else if (result->size()<=20){ // if only have less/equals than 20 results, show all.
        printQueue(*result); // output the result. this function is defined at the top of this file
        
    }
    
    else { // when result length larger than 20, need to randomly pick
        
        vector<video>*v=new vector<video>; // need a vector to do the shuffle as a queue has limited accessing method
        
        int queue_length=result->size();//have to record the size of queue first because .pop will affect the length of queue
        
        for (int i=0;i<queue_length;i++){ // copy the videos into vector
            v->push_back(result->front());
            result->pop();
        }
      
        // now result is empty
    
        random_device random_generator; // A generator that generates random value as initial value (seed) based on hardware behaviour
        mt19937 m(random_generator()); // generates random numbers based on Mersenne Twister algorithm and the seed value
        
        // the reason why using random_device is, if mt19937 generates serials of numbers based on the same initial value, these serials of numbers are going to be the same, thats why it should based on a random_device object that generates real random initial value.
        
        shuffle(v->begin(),v->end(),m); // shuffle the vector from begin to end using Fisher Yates algorithm based on the random numbers that "m" generates as index
        
        for (int i=0;i<20;i++){ // push the first 20 element of the shuffled vector into result
            result->push((*v)[i]);
        }
        
        delete v;
        v=nullptr;
        
        printQueue(*result); // prints the result to user, this function is defined at the top of this file
        
    }
    
    InvalidChoice: // If user inputted an invalid option

    string choice;
    
    cout<<' '<<endl;
    cout<<"To watch a video, please enter the number of video (NOT video ID), for example enter 1 for the first video in the list above, enter 2 for the second video)."<<endl;
    cout<<"To go back to main menu, please press M. "<<endl;
    cin>>choice;

    if (choice=="M"){
        return;
    }

    else {
        
        try{
            stoi(choice);
        }
        catch( const invalid_argument &e){
            cout<<"Invalid input!!"<<endl;
            goto InvalidChoice;// If user inputted an invalid option
        }
         
        int number_of_video=stoi(choice)-1; // the printQueue function prints from 1, so I have to minus 1 to get its true index.
        
        if(number_of_video>20||number_of_video<0){ // if the input out of range
            cout<<"Video number not found!"<<endl;
            goto InvalidChoice;
        }
        
        for (int i=0;i<number_of_video;i++){ // pops the video(s) before the one user chose
            result->pop();
        }
        
        video chosen_video=result->front();
        delete result;
        result=nullptr;
        this->ask_like_dislike(chosen_video, d);
    }
}

void main_menu::get_video_recommendation(const string &keyword, database &d){
    
    queue<video>*result=d.get_keyword_videos(keyword);
    list<video>*video_list=new list<video>;
    
    if (result->empty()){
        cout<<"No match videos. "<<endl;
        return;
    }
    
    int queueSize=result->size(); // have to define the size here because the pop() will affect the for loop condition
    
    for (int i=0;i<queueSize;i++){
        video_list->push_back(result->front());
        result->pop();
    }
    
    delete result;
    result=nullptr;
    
    if (video_list->size()>1){
        video_list->sort([](const video &a,const video &b){return a.getRate() > b.getRate();}); // sort the list by this lambda function, which in descending order by the video score
    }
    
    showTopTen(*video_list);
    
    Invalid_Choice: // If user inputted an invalid option

    string choice;
    cout<<' '<<endl;
    cout<<"To watch a video, please enter the number of video (NOT video ID, for example enter 1 for the first video in the list above, enter 2 for the second video)."<<endl;
    cout<<"To go back to main menu, please press M. "<<endl;
    cin>>choice;

    if (choice=="M"){
        return;
    }

    else {
        
        try{
            stoi(choice);
        }
        catch( const invalid_argument &e){
            cout<<"Invalid input!!"<<endl;
            goto Invalid_Choice;// If user inputted an invalid option
        }
        
        int numChoice=stoi(choice)-1; // Because when showing the list, I start from 1 to show the videos to user
        
        if (numChoice>10||numChoice<0){
            cout<<"Input out of range!"<<endl;
            goto Invalid_Choice;
        }
        
        for (int i=0;i<numChoice;i++){
            video_list->pop_front(); //pops the videos before the one use chose
        }
        
        video chosen=video_list->front();
        delete video_list;
        video_list=nullptr;
        
        this->ask_like_dislike(chosen, d);
    }
}

void main_menu::top_channels(database &d){
    
    deque<Creator>*result=d.get_channels();
    cout<<"Top ten channels with best average score:"<<endl;
    for (int i=0;i<result->size();i++){
        cout<<i+1<<" - "<<(*result)[i].getChannel()<<endl;
    }
    
    wrong_input:
    string choice;
    cout<<' '<<endl;
    cout<<"To explore a channel, please enter the ranked number (For example enter 1 for the first channel in the list above, enter 2 for the second channel) :"<<endl;
    cout<<"To go back to menu, please press M. "<<endl;
    cin>>choice;
    
    if (choice=="M"){
        return;
    }
    
    try{
        stoi(choice);
    }
    catch (const invalid_argument &e){
        cout<<"Invalid input!"<<endl;
        goto wrong_input;
    }
    
    if (stoi(choice)>10||stoi(choice)<1){
        cout<<"Invalid input!"<<endl;
        goto wrong_input;
    }
    
    int number_chosen=stoi(choice)-1; // The number in the list starts from 1, minus one to get the true index
    Creator temp=result->at(number_chosen);
    
    delete result;
    result=nullptr;
    
    Creator::iterator it=temp.begin(); // the iterator defined in creator class header file
    
    
    int counter=1; // to display the number of each video to user in order to let user to choose which one to watch
    
    for (;it!=temp.end();++it){ // traverse through creator class, and videos in the creator class are already sorted by descending order of their score
        
        size_t finder=(*it).second.find('#'); // i use this symbol to separate video ID and name in the multimap key
        string v_id=string((*it).second.begin(), (*it).second.begin()+finder); // video id is the content before '#' in the value of the pair
        string name=string ((*it).second.begin()+(finder+1), (*it).second.end()); // video name is the content after '#' in the value of the pair
        cout<<counter<<" - "<<v_id<<", "<<name<<", "<<"score: "<<(*it).first<<". "<<endl;
        counter++;
    }
    
   
    
    Incorrect:
    
    choice="";
    cout<<' '<<endl;
    cout<<"To watch a video, please enter the number of video (NOT video ID, for example enter 1 for the first video in the list above, enter 2 for the second video)."<<endl;
    cout<<"To go back to main menu, please press M. "<<endl;
    cin>>choice;
    
    if (choice=="M"){
        return;
    }
    
    try {
        stoi(choice);
    }
    
    catch(const invalid_argument &e){
        cout<<"Invalid option!"<<endl;
        goto Incorrect;
    }
    
    if (stoi(choice)<1||stoi(choice)>counter){
        cout<<"Input out of range!"<<endl;
        goto Incorrect;
    }
    
    number_chosen=stoi(choice)-1;
    
    it=temp.begin();
    for (int i=0;i<number_chosen;i++){
        ++it;
    }
    string video_key=(*it).second;
    size_t finder=video_key.find('#'); // i use this symbol to separate video ID and name in the multimap key
    string v_id=string(video_key.begin(), video_key.begin()+finder); // video id is the content before '#'
    string info=d.get_video_by_id(v_id); // this method returns the line of info for the corresponding id
    video temp_v=video(info);
    ask_like_dislike(temp_v, d);
    
}

void main_menu::ask_like_dislike(video&chosen_video,database&d){
    
    InvalidChoice2: // if user chooses an invalid option in the menu below

    string choice;
    cout<<"Playing: "<<endl;
    cout<<chosen_video<<endl;
    cout<<' '<<endl;
    
    
    cout<<"1 - Like /  2 - Dislike  /  3 - Go back to main menu"<<endl;
    cin>>choice;

    if (choice=="1"){
        d.add_like_or_dislike_to_video(chosen_video.getId(), 1); // its a method in class database that take a video id and a decision code as arguments to update the score of corresponding video and corresponding channel. Decision code 1 means like and 2 means dislike.
        cout<<"Video liked."<<endl;
    }

    else if (choice=="2"){
        d.add_like_or_dislike_to_video(chosen_video.getId(), 2); // its a method in class database that take a video id and a decision code as arguments to update the score of corresponding video and corresponding channel. Decision code 1 means like and 2 means dislike.
        cout<<"Video disliked."<<endl;
    }

    else if (choice=="3"){
        return;
    }

    else {
        cout<<"Invalid input!"<<endl;
        goto InvalidChoice2;
    }
}


