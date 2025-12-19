#ifndef ACTION_H
#define ACTION_H

#include <vector>

bool block_is_justified(int action_id, std::vector<int> influence);
bool action_is_justified(int action_id, std::vector<int> influence);
void print_vector(std::vector<int>);
#endif
