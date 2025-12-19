#include "action.h"
#include <stdexcept>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <set>
#include "../lib/world.h"
#include "../lib/player.h"

#include <stdlib.h>
using namespace std;
#include <iostream>
bool block_is_justified(int action_id, std::vector<int> influence) {
    if (action_id == 2) {
        return influence[4] != 0;
    }
    else if (action_id == 4) {
        return influence[3] != 0;
    }
    else if (action_id == 5) {
        return influence[2] != 0 || influence[0] != 0;
    }
    throw std::invalid_argument("Invalid block id");
}

bool action_is_justified(int action_id, std::vector<int> influence) {
    // if action is assassinate check for assassin influence
    if (action_id == 4)
        return influence[1] != 0;
    //if action is steal check for captain influence
    else if (action_id == 5)
        return influence[2] != 0;
    //if action is tax check for duke influence
    else if (action_id == 6)
        return influence[4] != 0;
    //if action is exchange check for ambassador influence
    else if (action_id == 7)
        return influence[0] != 0;
    throw invalid_argument("Invalid action id");
}

void print_vector(vector<int> x) {
    for (int i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << endl;
}