#include <stdio.h>
#include <stdbool.h>

#define TOTAL_PLAYERS 32
#define TOTAL_GROUPS 8
#define MAX_WEEKS 10
#define PLAYERS_PER_GROUP 4

void binary_string(char *str, unsigned int *data, int len)
{
	unsigned i, e = len-1;
	for (i = 1 << (len-1); i > 0; i = i / 2)
		str[e--] = (*data & i) ? '1' : '0';
	str[len] = '\0';
}

void print_players_constrains(unsigned int *constrains)
{
	for(int i = 0; i < TOTAL_PLAYERS; i++)
	{
		char str[TOTAL_PLAYERS+1] = "";
		binary_string(str, &constrains[i], TOTAL_PLAYERS);
		printf("Player %d: %s\n", i, str);
	}
}

void print_groups(unsigned int *groups)
{
	for(int i = 0; i < TOTAL_GROUPS; i++)
	{
		char str[TOTAL_PLAYERS+1] = "";
		binary_string(str, &groups[i], TOTAL_PLAYERS);
		printf("Group %d: %s\n", i, str);
	}
}

bool solve_group(unsigned int *groups, int group_index)
{
	//unsigned int group = groups[group_index];

        char str[TOTAL_PLAYERS+1] = "";
        //binary_string(str, &group, TOTAL_PLAYERS);
        //printf("Group to solve: %s\n", str);

	groups[group_index] = 0; // Reset current group

	// Now we need to get all the available players that can join this group in the current state.	
	// Rule 1) Use only available players within the current week.

	// A NOR operation of all groups will give us all the current available players in the current week
	unsigned int available_players_mask = 0;
	for(int i=0; i<TOTAL_GROUPS; i++)
		available_players_mask = available_players_mask | groups[i]; // OR
	available_players_mask = ~available_players_mask; // NOT

	

	binary_string(str, &available_players_mask, TOTAL_PLAYERS);
	printf("Available players mask: %s\n", str);

	

	// Rule 2) Players that are not in the same group in previous weeks.

	

	return false;
}

void solve_week(unsigned int *weeks, int week_index)
{
	unsigned int *week_groups = &weeks[week_index];
	solve_group(week_groups, 0); // Solve the first group of the current week

/*


	while(total_solved_groups<TOTAL_GROUPS)
	{
		solve_group(groups, g, week_index))
		{
			// Group cannot be solved. Return to previous week 
			return;
		}
	}

	solve_week(groups, week_index+1);
*/
}

int main(int argc, char *argv[])
{
	printf("Social Golfer Problem solver\n\n");

	// Groups definition
	// unsigned int groups[TOTAL_GROUPS] = {0};

	// Weeks definition
	unsigned int weeks[MAX_WEEKS][TOTAL_GROUPS] = {0};

	// Definition of pairs of players taken
	unsigned int constrains[TOTAL_PLAYERS] = {0};

	// Setup group 0 of week 0 just for testing
	weeks[0][0] |= 1UL << 0;
	weeks[0][0] |= 1UL << 3;

	// Initial configuration per week 0
/*	weeks[0][0] = 0xF; // 0,1,2,3
	weeks[0][1] = 0xF0; // 4,5,6,7
	weeks[0][2] = 0xF00; // 8,9,10,11
	weeks[0][3] = 0xF000; // 12,13,14,15
	weeks[0][4] = 0xF0000; // 16,17,18,19
	weeks[0][5] = 0xF00000; // 20,21,22,23
	weeks[0][6] = 0xF000000; // 24,25,26,27
	weeks[0][7] = 0xF0000000; // 28,29,30,31

	// Initial configuration of pairs of players constrains
	constrains[0] = 0xF; // 0,1,2,3
	constrains[1] = 0xF; // 0,1,2,3
	constrains[2] = 0xF; // 0,1,2,3
	constrains[3] = 0xF; // 0,1,2,3
	constrains[4] = 0xF0; // 4,5,6,7
	constrains[5] = 0xF0; // 4,5,6,7
	constrains[6] = 0xF0; // 4,5,6,7
	constrains[7] = 0xF0; // 4,5,6,7
	constrains[8] = 0xF00; // 8,9,10,11
	constrains[9] = 0xF00; // 8,9,10,11
	constrains[10] = 0xF00; // 8,9,10,11
	constrains[11] = 0xF00; // 8,9,10,11
	constrains[12] = 0xF000; // 12,13,14,15
	constrains[13] = 0xF000; // 12,13,14,15
	constrains[14] = 0xF000; // 12,13,14,15
	constrains[15] = 0xF000; // 12,13,14,15
	constrains[16] = 0xF0000; // 16,17,18,19
	constrains[17] = 0xF0000; // 16,17,18,19
	constrains[18] = 0xF0000; // 16,17,18,19
	constrains[19] = 0xF0000; // 16,17,18,19
	constrains[20] = 0xF00000; // 20,21,22,23
	constrains[21] = 0xF00000; // 20,21,22,23
	constrains[22] = 0xF00000; // 20,21,22,23
	constrains[23] = 0xF00000; // 20,21,22,23
	constrains[24] = 0xF000000; // 24,25,26,27
	constrains[25] = 0xF000000; // 24,25,26,27
	constrains[26] = 0xF000000; // 24,25,26,27
	constrains[27] = 0xF000000; // 24,25,26,27
	constrains[28] = 0xF0000000; // 28,29,30,31
	constrains[29] = 0xF0000000; // 28,29,30,31
	constrains[30] = 0xF0000000; // 28,29,30,31
	constrains[31] = 0xF0000000; // 28,29,30,31
*/
	// Print groups of the current week
	print_groups(weeks[0]);

	// Print player constrains
	print_players_constrains(constrains);

	// Start solving second week
	solve_week(*weeks, 0);
}

