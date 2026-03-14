#ifndef PLAYER_H
#define PLAYER_H
#include <vector>

class public_state;

class player {
public:
    //players have influences stored as a one hot vector of which influences they have,
    // influecnes are in the order ambassador, assassin, captain, contessa, duke
    std::vector<int> influences;
    // player id is an integer to track player order and identity
    int id;


// constructor which takes influence vector and id
    player(const std::vector<int>& influences, int id);
    // choose action to take at the beginning of turn
    std::vector<int> choose_taken_action(const public_state& state, bool forced_coup) const;
    //decide whether to challenge another player's action    
    int decide_challenge(int current_player_id, int target_player_id, int action_id, const public_state& state);
    // decide whether to block another player's action by claiming an influence
    // takes as input the id of the blocker, the action id being blocked
    int decide_block(int current_player_id, int target_player_id, int action_id, const public_state& state);
    // choose which influence to lose when losing influence
    int choose_lost_influence(const public_state& state);
    // checks if player is still playing
    int decide_challenge_block(int current_player_id, int blocker_id,  int action_id, const public_state& state);
    void remove_influence(int i);
    void disqualify();
    void add_influence(int i);
    std::vector<int> exchange(int i, int j);
    std::vector<int> get_influence_indexes();
};

#endif // PLAYER_H