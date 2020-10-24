#include <stdio.h>

#define TOTAL_PLAYERS 32
#define TOTAL_GROUPS 8
#define PLAYERS_PER_GROUP 4

void print_group(unsigned int *groups, unsigned int group_index)
{
	char str[TOTAL_PLAYERS+1] = "";
	unsigned i, e = TOTAL_PLAYERS-1;
	for (i = 1 << (TOTAL_PLAYERS-1); i > 0; i = i / 2) 
		str[e--] = (groups[group_index] & i) ? '1' : '0'; 
	str[TOTAL_PLAYERS] = '\0';
	printf("Group %d: %s\n", group_index, str);
}

int main(int argc, char *argv[]) {
	printf("Social Golfer Problem solver\n\n");

	// Initilize groups setup
	unsigned int groups[TOTAL_GROUPS] = {0};

	// Setup group 0 for testing
	groups[0] |= 1UL << 0;
	groups[0] |= 1UL << 3;

	// Print groups of the current week
	for(int i = 0; i < TOTAL_GROUPS; i++)
		print_group(groups, i);
}

