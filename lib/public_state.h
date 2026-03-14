#ifndef PUBLIC_STATE_H
#define PUBLIC_STATE_H

#include <math.h>
#include <vector>
#include <queue>
#include <set>



class public_state{
    public:
        std::vector<int> coins;
        std::vector<int> influences;
        std::vector<int> public_deck;
        public_state();
        void decrement_influence(int player_id);
        bool is_playing(int player_id) const;
        void adjust_coins(int player_id, int amount);
        void disqualify(int player_id);
        void increment_influence(int player_id);        
        void add_card(int i);
    };
#endif // PUBLIC_STATE_H