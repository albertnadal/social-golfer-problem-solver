#include <stdio.h>

#define TOTAL_PLAYERS 32
#define TOTAL_GROUPS 8
#define PLAYERS_PER_GROUP 4

void binary_string(char *str, unsigned int *data, int len)
{
	unsigned i, e = len-1;
	for (i = 1 << (len-1); i > 0; i = i / 2)
		str[e--] = (*data & i) ? '1' : '0';
	str[len] = '\0';
}

void print_player_constrains(unsigned int *pairs, unsigned int player_index)
{
        char str[TOTAL_PLAYERS+1] = "";
	binary_string(str, &pairs[player_index], TOTAL_PLAYERS);
        printf("Player %d: %s\n", player_index, str);
}

void print_group(unsigned int *groups, unsigned int group_index)
{
	char str[TOTAL_PLAYERS+1] = "";
	binary_string(str, &groups[group_index], TOTAL_PLAYERS);
	printf("Group %d: %s\n", group_index, str);
}

void solve_group(unsigned int *groups, int group_index, int week_number)
{
	// TODO: Implement solve group
}

void solve_week(unsigned int *groups, int week_index)
{
	// TODO: Implement solve week
	solve_group(groups, 0, week_index);
}

int main(int argc, char *argv[])
{
	printf("Social Golfer Problem solver\n\n");

	// Groups definition
	unsigned int groups[TOTAL_GROUPS] = {0};

	// Definition of pairs of players taken
	unsigned int pairs[TOTAL_PLAYERS] = {0};

	// Setup group 0 for testing
	groups[0] |= 1UL << 0;
	groups[0] |= 1UL << 3;

	// Initial configuration per week 0
	groups[0] = 0xF; // 0,1,2,3
	groups[1] = 0xF0; // 4,5,6,7
	groups[2] = 0xF00; // 8,9,10,11
	groups[3] = 0xF000; // 12,13,14,15
	groups[4] = 0xF0000; // 16,17,18,19
	groups[5] = 0xF00000; // 20,21,22,23
	groups[6] = 0xF000000; // 24,25,26,27
	groups[7] = 0xF0000000; // 28,29,30,31

	// Initial configuration of pairs of players constrains
	pairs[0] = 0xF; // 0,1,2,3
	pairs[1] = 0xF; // 0,1,2,3
	pairs[2] = 0xF; // 0,1,2,3
	pairs[3] = 0xF; // 0,1,2,3
	pairs[4] = 0xF0; // 4,5,6,7
	pairs[5] = 0xF0; // 4,5,6,7
	pairs[6] = 0xF0; // 4,5,6,7
	pairs[7] = 0xF0; // 4,5,6,7
	pairs[8] = 0xF00; // 8,9,10,11
	pairs[9] = 0xF00; // 8,9,10,11
	pairs[10] = 0xF00; // 8,9,10,11
	pairs[11] = 0xF00; // 8,9,10,11
	pairs[12] = 0xF000; // 12,13,14,15
	pairs[13] = 0xF000; // 12,13,14,15
	pairs[14] = 0xF000; // 12,13,14,15
	pairs[15] = 0xF000; // 12,13,14,15
	pairs[16] = 0xF0000; // 16,17,18,19
	pairs[17] = 0xF0000; // 16,17,18,19
	pairs[18] = 0xF0000; // 16,17,18,19
	pairs[19] = 0xF0000; // 16,17,18,19
	pairs[20] = 0xF00000; // 20,21,22,23
	pairs[21] = 0xF00000; // 20,21,22,23
	pairs[22] = 0xF00000; // 20,21,22,23
	pairs[23] = 0xF00000; // 20,21,22,23
	pairs[24] = 0xF000000; // 24,25,26,27
	pairs[25] = 0xF000000; // 24,25,26,27
	pairs[26] = 0xF000000; // 24,25,26,27
	pairs[27] = 0xF000000; // 24,25,26,27
	pairs[28] = 0xF0000000; // 28,29,30,31
	pairs[29] = 0xF0000000; // 28,29,30,31
	pairs[30] = 0xF0000000; // 28,29,30,31
	pairs[31] = 0xF0000000; // 28,29,30,31

	// Print groups of the current week
	for(int i = 0; i < TOTAL_GROUPS; i++)
		print_group(groups, i);

	// Print player constrains
	for(int i = 0; i < TOTAL_PLAYERS; i++)
		print_player_constrains(pairs, i);

	solve_week(groups, 1);
}

