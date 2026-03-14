#include <iostream>
#include <cmath>
#include <set>
#include <stdlib.h>
#include "lib/helper.h"
#include "lib/world.h"
#include "lib/player.h"

using namespace std;

// Helper function to get coin count
int get_coins(const world& w, int player_id) {
    return w._public_state.coins[player_id];
}

// Helper function to validate action
bool is_action_valid(const world& w, int current_player, int target_player_id, int action_id) {
    return !((action_id == 3 && get_coins(w, current_player) < 7) ||
             (action_id == 4 && get_coins(w, current_player) < 3) ||
             (action_id == 5 && get_coins(w, target_player_id) < 2) ||
             !w._public_state.is_playing(target_player_id));
}

// Helper function to get challengers
vector<int> get_challengers(const world& w, int current_player, int target_player_id,
                             int action_id, vector<player>& players) {
    vector<int> challenger_ids;
    for (auto& player : players) {
        if (player.id != current_player && w._public_state.is_playing(player.id)) {
            if (player.decide_challenge(current_player, target_player_id, action_id, 
                                       w._public_state)) {
                challenger_ids.push_back(player.id);
            }
        }
    }
    return challenger_ids;
}

// Helper function to process challenge
bool process_challenge(world& w, int challenger_id, int current_player, int& loser_id, int& winner_id) {
    if (action_is_justified(0, w.players[current_player].influences)) {
        loser_id = challenger_id;
        winner_id = current_player;
        cout << "Action justified. Player " << loser_id << " loses influence.\n";
    } else {
        loser_id = current_player;
        winner_id = challenger_id;
        cout << "Action not justified. Current player loses influence.\n";
        return false;
    }
    w.lose_influence(loser_id, false);
    w.lose_influence(winner_id, true);
    w.player_draw_influence(winner_id);
    return true;
}

// Helper function to get blockers
vector<int> get_blockers(const world& w, int current_player, int target_player_id, 
                         int action_id, vector<player>& players) {
    vector<int> blocker_ids;
    if (action_id == 2) {
        for (auto& player : players) {
            if (player.id != current_player && w._public_state.is_playing(player.id)) {
                if (player.decide_block(current_player, target_player_id, action_id, 
                                       w._public_state)) {
                    blocker_ids.push_back(player.id);
                }
            }
        }
    } else if (action_id == 4 || action_id == 5) {
        if (players[target_player_id].decide_block(current_player, target_player_id, 
                                                   action_id, w._public_state)) {
            blocker_ids.push_back(target_player_id);
        }
    }
    return blocker_ids;
}

int main() {
    srand((unsigned)time(0));
    cout << "Coup simulation started.\n";

    world new_world(4);
    cout << "World with 4 players created.\n";

    while (new_world.is_not_finished()) {
        cout << "\n=== Start of turn ===\n";
        int current_player = new_world.get_next_player().id;
        cout << "Player " << current_player << "'s turn.\n";

        // Determine if coup is forced
        bool forced_coup = (get_coins(new_world, current_player) >= 10);
        vector<int> action = new_world.players[current_player].choose_taken_action(
            new_world._public_state, forced_coup);

        int target_player_id = action[0];
        int action_id = action[1];
        bool action_justified = true;

        // Display action
        if (action_id < 3 || action_id > 5) {
            cout << "Action: " << action[1] << "\n";
        } else {
            cout << "Action: " << action_id << " targeting player " << target_player_id << "\n";
        }

        // Validate action
        if (!is_action_valid(new_world, current_player, target_player_id, action_id)) {
            cout << "Invalid action. Player disqualified.\n";
            action_justified = false;
            new_world.remove_player(current_player);
        } else if (action_id == 4) {
            new_world.pay(current_player);
        }

        // Handle challenges for influence-requiring actions
        if (action_id > 3 && action_justified) {
            vector<int> challengers = get_challengers(new_world, current_player, target_player_id,
                                                      action_id, new_world.players);
            if (!challengers.empty()) {
                int challenger_id = challengers[rand() % challengers.size()];
                cout << "Player " << challenger_id << " challenges!\n";
                int loser_id, winner_id;
                action_justified = process_challenge(new_world, challenger_id, current_player, loser_id, winner_id);
            } else {
                cout << "No challenges.\n";
            }
        }

        // Handle blocks
        if (action_justified) {
            vector<int> blockers = get_blockers(new_world, current_player, target_player_id, 
                                               action_id, new_world.players);
            
            if (!blockers.empty()) {
                int blocker_id = blockers[rand() % blockers.size()];
                cout << "Blocker: " << blocker_id << "\n";
                
                vector<int> challengers = get_challengers(new_world, current_player, blocker_id,
                                                          action_id, new_world.players);
                if (!challengers.empty()) {
                    int challenger_id = challengers[rand() % challengers.size()];
                    cout << "Block challenged by " << challenger_id << "\n";
                    
                    if (block_is_justified(action_id, new_world.players[target_player_id].influences)) {
                        cout << "Block justified. Challenger loses influence.\n";
                        new_world.lose_influence(challenger_id, false);
                        new_world.lose_influence(blocker_id, true);
                        action_justified = false;
                    } else {
                        cout << "Block not justified. Blocker loses influence.\n";
                        new_world.lose_influence(blocker_id, false);
                        new_world.lose_influence(challenger_id, true);
                    }
                    new_world.player_draw_influence(blocker_id);
                } else {
                    cout << "Block succeeds.\n";
                    action_justified = false;
                }
            } else {
                cout << "No blocks.\n";
            }
        }

        // Execute action
        if (action_justified) {
            cout << "Action executed.\n";
            if (new_world._public_state.is_playing(target_player_id) || action_id < 3 || action_id > 5) {
                new_world.carry_out_action(current_player, target_player_id, action_id);
            }
        }

        // Display end-of-turn state
        cout << "\n=== End of turn ===\n";
        cout << "Coins: ";
        for (int i = 0; i < new_world.players.size(); ++i) {
            cout << new_world._public_state.coins[i] << " ";
        }
        cout << "\n";
    }
    cout << "Coup simulation ended.\n";
    cout << "Winner: Player " << new_world.get_next_player().id << "\n";
    system("pause");
    return 0;
}