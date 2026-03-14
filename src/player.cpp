#include <cstdlib>          // for rand()
#include "../lib/player.h"
#include "../lib/public_state.h"
#include "../lib/helper.h"
class action;
class public_state;
using namespace std;
#include <iostream>

player::player(const std::vector<int>& influences, int id): _influences(influences), _id(id) {}

// takes public state as input, returns encoding of action taken by player as target, action_id, confidence
std::vector<int> player::choose_taken_action(const public_state& state, bool forced_coup) const{
    int act_id;
    int target;
    target = rand() % 4;
    if (forced_coup){
         act_id = 3;
    }
    else{
    act_id = rand()%7+1;
    }
    return std::vector<int> {target, act_id};   
}

// takes target player id, action id, and public state as input, returns 1 to challenge, 0 to not challenge
int player::decide_challenge(int current_player_id, int target_player_id, int action_id, const public_state& state) {
    //randomly choose 1 or 0
    // if(rand() % 2 == 0) {
    //     return 1;
    // } else {
    //     return 0;
    // }
    return 0;
}

// takes target player id, action id, and public state as input, returns 1 to block, 0 to not block
int player::decide_block(int current_player_id, int target_player_id, int action_id, const public_state& state){
    //randomly choose 1 or 0
    if(rand() % 2 == 0) {
        return 1;
    } else {
        return 0;
    }
};
// takes blocker id, action id, and public state as input, returns 1 to challenge block, 0 to not challenge
int player::decide_challenge_block(int current_player_id, int blocker_id,  int action_id, const public_state& state){
    //randomly choose 1 or 0
    if(rand() % 2 == 0) {
        return 1;
    } else {
        return 0;
    }
};

int player::choose_lost_influence(const public_state& state) {
    // randomly choose one of the influences in _influences to lose influences represented as a vector of 0s and 1s where 1 means influence is present
    std::vector<int> present_influences = get_influence_indexes();

    //returns index of influence chosen to be lost
    return present_influences[rand()%present_influences.size()];
}

void player::remove_influence(int i){
    _influences[i]--;
};

void player::add_influence(int i){
    _influences[i]++;
}

void player::disqualify(){
    _influences = {0,0,0,0,0};
}

// takes 2 influence indexes, allows player to choose to swap any number of influences
// by returning indexes of rejected influences

vector<int> player::exchange(int i, int j) {
    vector<int> all_influences = get_influence_indexes();
    all_influences.push_back(i);
    all_influences.push_back(j);

    int a = rand() % all_influences.size();
    int b;
    do {
        b = rand() % all_influences.size();
    } while (a == b);

    return {all_influences[a], all_influences[b]};
}