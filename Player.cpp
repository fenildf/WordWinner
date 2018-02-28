//
// Created by kang on 18-1-21.
//

#include <iostream>
#include "Player.h"
Player::Player(std::string name):player_name_(name)
{

}
void Player::print()
{
    std::cout << this->player_name_ << std::endl;
}