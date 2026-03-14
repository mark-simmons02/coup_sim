#include <cmath>
#include <set>
#include "../lib/world.h"
#include "../lib/player.h"
#include "../lib/helper.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
// returns pointer to front of the queue and moves that player to the back of the queue

player world::get_next_player() {
    int next_player = turn_order.front();
    if (_public_state.is_playing(next_player)) {
        turn_order.pop();
        turn_order.push(next_player);
    } else {
        turn_order.pop();
        next_player = get_next_player().id;
    }

    return players[next_player];
}

void world::remove_player(int player_id) {
    // Rebuild turn_order queue without the removed player
    queue<int> new_turn_order;

    while (!turn_order.empty()) {
        int p = turn_order.front();
        turn_order.pop();
        if (p != player_id) {
            new_turn_order.push(p);
        }
    }
    _public_state.disqualify(player_id); // sets public state number of influences to 0
    players[player_id].disqualify(); //sets player influence vector to 0
    turn_order = new_turn_order;
}

void world::player_draw_influence(int player_id){
    int influence = draw_from_deck();
    players[player_id].add_influence(influence);
    _public_state.increment_influence(player_id);
}

int world::draw_from_deck(){
    int choice;
    do {
        choice = rand() % 5;
    } while (rand() % 3 < (3 - _deck[choice]));
    // choose card with monte carlo method to account for varying number of each type of influence remaining
    _deck[choice]--; //decrement chosen card from deck
    return choice;
}

void world::add_to_deck(int id){
    _deck[id]++;
}

world::world(int num_players) : players(), turn_order(), _deck() {
    public_state new_public_state = public_state();

    _deck = {3, 3, 3, 3, 3}; // Assuming 5 types of influences with 3 each
    // ambassador, assassin, captain, contessa, duke
    // randomly remove cards from remaining cards in deck to create player influences
    // player influences represented as 5 integer vector like deck
    for (int i = 0; i < num_players; i++) {

        vector<int> player_influences = {0,0,0,0,0};
        for (int j = 0; j < 2; j++) {

            int choice = draw_from_deck();
            player_influences[choice]++; //add chosen card to player's influences
        }

        players.emplace_back(player_influences, i);

    }
    cout << "Player influences after initialization:\n";
    for (const auto &p : players) {
        cout << "Player " << p.id << ": ";
        for (int infl : p.influences) {
            cout << infl << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < num_players; i++) {
        turn_order.push(players[i].id);
    }
}

bool world::is_not_finished(){
    cout << "Number of players remaining " << turn_order.size()<<endl;
    return turn_order.size() >1;
}

void world::lose_influence(int player_id, bool swap){

    if (_public_state.influences[player_id] == 2 || swap){
        int i = players[player_id].choose_lost_influence(_public_state);
        if (swap){
            _deck[i]++;
        }
        else{
            _public_state.add_card(i);
        }

        players[player_id].remove_influence(i);
        _public_state.decrement_influence(player_id);
    }
    else{
        cout<<"lost second influence"<<endl;
        //remove player from game
        cout << "Player " << player_id << " has been eliminated from the game." << endl;
        this->remove_player(player_id);
        cout<<"player removed from game succeeded"<<endl;
    }
}

void world::pay(int player_id){
    _public_state.adjust_coins(player_id, -3);
}

void world::carry_out_action(int current_player_id, int target_player_id, int action_id){
    //switch statement for different action ids 1 to 7
    switch(action_id){
        case 1: //income
            cout << "Player " << current_player_id << " takes income." << endl;
            //increase current player coins by 1
            _public_state.adjust_coins(current_player_id, 1);
            break;
        case 2: //foreign aid
            cout << "Player " << current_player_id << " takes foreign aid." << endl;
            //increase current player coins by 2
            _public_state.adjust_coins(current_player_id, 2);
            break;
        case 3: //coup
            cout << "Player " << current_player_id << " coups Player " << target_player_id << "." << endl;
            // target player loses influence
            // current player loses 7 coins
            _public_state.adjust_coins(current_player_id, -7);
            this->lose_influence(target_player_id, false);
            break;
        case 4: //assassinate
            cout << "Player " << current_player_id << " assassinates player "<< target_player_id << endl;
            // target player loses influence
            this->lose_influence(target_player_id, false);
            break;
        case 5: //steal
            //target player loses 2 coins, current player gains 2 coins
            cout << "Player " << current_player_id << " steals from Player " << target_player_id << "." << endl;
            _public_state.adjust_coins(current_player_id, 2);
            _public_state.adjust_coins(target_player_id, -2);
            
            break; 
        case 6: //tax
            //current player gains 3 coins
            cout << "Player " << current_player_id << " gains 3 coins." << endl;
            _public_state.adjust_coins(current_player_id, 3);
            break;
        case 7: //exchange
            // current player picks between 2 drawn influences and returns 2 to deck
            cout << "Player " << current_player_id << " exchanges influences" << endl;
            //TODO


            // cout<<"deck before exchange"<<endl;
            // print_vector(_deck);
            // cout<<"influences before exchange"<<endl;
            // print_vector(players[current_player_id].influences);

            // i is the index of a random card from the deck, mirroring the index of an influence
            int i = draw_from_deck();
            int j = draw_from_deck();
            vector<int> rejected_influences = players[current_player_id].exchange(i,j);

            players[current_player_id].add_influence(i);
            players[current_player_id].add_influence(j);


            cout << endl;

            players[current_player_id].remove_influence(rejected_influences[0]);
            players[current_player_id].remove_influence(rejected_influences[1]);
            _deck[rejected_influences[0]]++;
            _deck[rejected_influences[1]]++;


            // cout<<"deck after exchange"<<endl;
            // print_vector(_deck);
            // cout<<"influences after exchange"<<endl;
            // print_vector(players[current_player_id].influences);
            
            break;
    }
};
