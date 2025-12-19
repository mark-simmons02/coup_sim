#include <cmath>
#include <set> 
#include "lib/action.h"
#include "lib/world.h"   
#include "lib/player.h"
#include <stdlib.h>
using namespace std;
#include <iostream>

int main() {
    srand((unsigned)time(0));
    cout<<"Coup simulation started."<<endl;
    int target_player_id;
    int action_id;
    int challenger_id;
    int blocker_id;
    int loser_id;
    int winner_id;
    bool action_justified;
    bool forced_coup;
    vector<int> current_player_action;
    vector<int> challenger_ids;
    vector<int> blocker_ids;
    vector<player>::iterator it;

    world new_world = world(4);

    cout<<"World with 4 players created."<<endl;

    while(new_world.is_not_finished()){
        cout << endl;    
        cout <<"Start of turn"<<endl;
        cout << endl;
        int current_player = new_world.get_next_player().get_id();
        cout << "Start of player " << current_player<<"'s turn." << endl;

    //current player makes decision
        if (new_world.get_public_state().get_coins(current_player) < 10){
            forced_coup = false;
        }
        else{
            forced_coup = true;
        }
        current_player_action = new_world.get_players()[current_player].choose_taken_action(new_world.get_public_state(), forced_coup);
        action_justified = true;

            if(current_player_action[1]<3 ||
            current_player_action[1]>5){
                cout << "Current Player Action ";

                cout <<current_player_action[1]<<" ";
            }
            else{
                cout << "Current Player Target and Action ";

                for (int val : current_player_action) {
                    cout << val << " ";
                }
            }
        cout << endl;

        target_player_id = current_player_action[0], action_id = current_player_action[1];
    //if action requires coins check coin count
        if ( (action_id == 3 && new_world.get_public_state().get_coins(current_player)<7) || 
        (action_id == 4 && new_world.get_public_state().get_coins(current_player)<3) ||
        (action_id == 5 && new_world.get_public_state().get_coins(target_player_id)<2) ||
         !new_world.get_public_state().is_playing(target_player_id) ){ 
            
            cout << "Invalid action, either not enough coins or targeting player already out."<< 
            "Current player is disqualified"<<endl;
            // invalid action, not enough coins, disqualify player.
            action_justified = false;
            new_world.remove_player(current_player);
        }
        // assassninate requires payment even if action fails.
        else if(action_id ==4){
            new_world.pay(current_player);
        }
    //if action requires influence

        if (action_id > 3 && action_justified) {
            challenger_ids = {};

    // for each other player in turn order queue        
            for ( it = new_world.get_players().begin(); it!= new_world.get_players().end(); ++it) {

                if (it->get_id() != current_player && new_world.get_public_state().is_playing(it->get_id())) {
    // make decision
                    if (it -> decide_challenge(current_player, target_player_id, action_id, new_world.get_public_state()) == 1){
                        challenger_ids.push_back( it->get_id());    
                    }
                }      
            }
    //if someone has challenged, process challenge
            if (challenger_ids.size() > 0){
    // challenger_ids is a vector of ids of players who challenge. choose one at random
                int challenger_id = challenger_ids[rand() % challenger_ids.size()];
                cout << "Player number " << challenger_id <<" challenges player number "<< current_player<<"'s action."<< endl;
                if (action_is_justified(action_id, new_world.get_players()[current_player].get_influences())){
                    loser_id = challenger_id;
                    winner_id = current_player;
                    cout << "Action is justified. Player "<<loser_id<<" loses influence, challenged player "<<winner_id<<" changes influence." << endl;
                    //TODO add implementation of function to let challenger choose new influence
                }
                else{loser_id = current_player;
                    winner_id = challenger_id;
                    cout << "Action is not justified. Current player loses influence." << endl;
                    action_justified = false;
                }
                // remove influence from challenger, current player swaps cards
                //search for loser

                new_world.lose_influence(loser_id, false);

                new_world.lose_influence(winner_id, true);
                new_world.player_draw_influence(winner_id);
                
            // end of loser of challenge losing inflluence                
            }// end of if there is at least one challenger
            else{
                cout<<"No challenges"<<endl;
            }
        } // end of challenge processing
        // if action is still valid
        if (action_justified){
            //if action has an id which relates to a blockable action
            //different collection method of blocker ids to collect from everyone when action is 2 and only from challenger when id is 4,5
            blocker_ids = {};
            //if foreign aid check all players for blocks
            if(action_id == 2){
        // for each other player in turn order queue        
                for ( it = new_world.get_players().begin(); it!= new_world.get_players().end(); ++it) {

                    if (it->get_id() != current_player && new_world.get_public_state().is_playing(it->get_id())) {
        // make decision
                        if (it -> decide_block(current_player, target_player_id, action_id, new_world.get_public_state()) == 1){
                            blocker_ids.push_back(it->get_id());    
                        }
                    }      
                }   
            }
            //else if assassinate or steal ony check if target wants to block
            else if(action_id == 4 || action_id == 5){
                //search for target player
                if (new_world.get_players()[target_player_id].decide_block(current_player, target_player_id, action_id, new_world.get_public_state()) == 1){
        // challenger_ids is a vector of ids of players who block. choose one at random
                    blocker_ids.push_back(target_player_id);    
                }
            }
                    //check if block is challenged
            challenger_ids = {};
            //check for anyone who wants to challenge block
            if (blocker_ids.size() > 0){
                //choose blocker from blockers
                blocker_id = blocker_ids[rand() % blocker_ids.size()];
                    cout << "Blocker ID: " << blocker_id << endl;

                for (it = new_world.get_players().begin(); it!= new_world.get_players().end(); ++it) {
                    if (it->get_id() != blocker_id && new_world.get_public_state().is_playing(it->get_id())) {
            // make decision    
                        if (it ->decide_challenge_block(current_player, blocker_id, action_id, new_world.get_public_state()) == 1){
                            
                            challenger_ids.push_back( it->get_id());    
                        }
                    }
                }
            // if at least one person challenges
                if (challenger_ids.size() > 0){
            // challenger_ids is a vector of ids of players who challenge. choose one at random
                    int challenger_id = challenger_ids[rand() % challenger_ids.size()];
                    cout << "Challenger to block ID: " << challenger_id << endl;
                    if (block_is_justified(action_id, new_world.get_players()[target_player_id].get_influences())){
                        loser_id = challenger_id;
                        winner_id = blocker_id;
                        cout << "Block is justified. Challenger loses influence, blocker shuffles influence." << endl;
                        action_justified = false;
                        // TODO allow current player to swap out influence

                    }
                    else{loser_id = blocker_id;
                        winner_id = challenger_id;
                        cout << "Block is not justified. Blocker player "<< blocker_id<<" loses influence." << endl;
                    }
                    // remove influence from loser
                    new_world.lose_influence(loser_id, false);
                    new_world.lose_influence(winner_id, true);
                    cout<<"three"<<endl;
                    new_world.player_draw_influence(winner_id);
                }// end of if at least one person challenges
                        // if noone challenges then block succeeds
                else{action_justified = false;
                    cout<<"No challenges to block"<<endl;
                }
            }// end of if player blocks
            else{
                cout<<"No blocks"<<endl;
            }
        }// end of if action can be blocked

        if (action_justified){
            cout << "Action is carried out." << endl;
            if (new_world.get_public_state().is_playing(target_player_id) || action_id < 3 || action_id >5){ 
                new_world.carry_out_action(current_player, target_player_id, action_id);
            }
        }
    cout << endl;    
    cout <<"End of turn"<<endl;
    cout << endl;

    cout << "Coins of players"<< endl;
    for (int i = 0; i < new_world.get_players().size(); ++i){
        cout << new_world.get_public_state().get_coins(i) << ' ';}
    cout << endl;

    cout << "influence of players" << endl;
    for (int i = 0; i < new_world.get_players().size(); ++i){
        cout << new_world.get_public_state().get_influences()[i] << ' ';}
    cout << endl;
    
    cout << "Player influences after turn:\n";
    for (const auto &p : new_world.get_players()) {
        cout << "Player " << p.get_id() << ": ";
        for (int infl : p.get_influences()) {
            cout << infl << " ";
        }
        cout << endl;
    }
    }
        // end of action resolution
    cout<<"Coup simulation ended."<<endl;
    cout<<"Player "<< new_world.get_next_player().get_id()<< " wins!" << endl;


    // cout all of player information on all players
    return 0;
}