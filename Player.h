//
// Created by kang on 18-1-21.
//

#ifndef WORDWINNER_PLAYER_H
#define WORDWINNER_PLAYER_H

#include <vector>
#include <string>


class Player {
public:
    //debug
    Player(std::string name);
    void print();
    //debug
    Player();
    ~Player(){}

    int refreshPlayerInformation();
    int addFriend();
    int deleteFriend();
    int battleWithFriend();
    int battleWithStranger();

private:
    int player_id_;
    std::string player_name_;
    int score_;
    int rank_;
    int state_;
    std::vector<Player> friends_;


};


#endif //WORDWINNER_PLAYER_H
