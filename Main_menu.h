//
//  Main_menu.h
//  assignment2
//
//  Created by Wy 陈 on 10/01/2024.
//

#ifndef Main_menu_h
#define Main_menu_h

#include <algorithm>
#include <iterator>
#include <queue>
#include <random>
#include <vector>
#include <list>
#include <map>
#include "Database.h"
#include "User.h"

class main_menu{
public:
    
    main_menu(User&u, database&d); // constructor which shows user the menu for users
    main_menu(Creator&c, database&d); // constructor which shows creator the menu for creators
    void user_menu(database&d, User&u);
    void get_video(const string &keyword, database &d); // display 20 videos related to the keyword randomly (if there are more than 20), and user can display, likes or dislikes the video. The d here cannot be const because I have to update the like/dislike data to corresponding video and channel.
    void get_video_recommendation(const string&keyword, database&d);
    void top_channels(database&d);
    void ask_like_dislike(video&chosen_video,database&d);
    
};
#endif /* Main_menu_h */
