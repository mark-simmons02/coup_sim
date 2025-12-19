#include <cmath>
#include <set>
#include "../lib/action.h"
#include "../lib/world.h"
#include <stdlib.h>
using namespace std;

std::vector<int> public_state::get_influences() const {
    return influences;
}

std::vector<int> public_state::get_public_deck() const {
    return public_deck;
}

void public_state::adjust_coins(int player_id, int amount) {
    int x = coins[player_id];
    coins[player_id] = x +amount;
}

public_state::public_state() : coins(vector<int>()), influences(vector<int>()), public_deck(vector<int>()) {
    // Initialize coins and influences for players
    // Assuming a default of 0 coins and 2 influences per player for 4 players
    for (int i = 0; i < 4; i++) {
        coins.push_back(2);
        influences.push_back(2);
    }
    public_deck = {0,0,0,0,0};

}

void public_state::add_card(int i){
    public_deck[i]++;
}

int public_state::get_coins(int player_id) const{
    return coins[player_id];
}

void public_state::decrement_influence(int player_id) {

    influences[player_id]--;
}

void public_state::increment_influence(int player_id) {

    influences[player_id]++;
}

bool public_state::is_playing(int player_id) const {
    return (get_influences()[player_id] > 0);
}

void public_state::disqualify(int player_id){
    influences[player_id] = 0;
    coins[player_id] = 0;
};