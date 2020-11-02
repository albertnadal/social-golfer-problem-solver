#include <stdlib.h>
#include <stdio.h>

#define MAX_PLAYERS 32
#define MAX_GROUPS 8
#define MAX_WEEKS 10
#define PLAYERS_PER_GROUP 4
#define MAX_GROUP_COMBINATIONS 35960 // Max possible player combinations in a group without repetition 32!/(4!*(32-4)!) = 35960

void binary_string(char *str, const unsigned int *data, unsigned int len) {
    unsigned int i, e = len - 1;
    for (i = 1 << (len - 1); i > 0; i = i / 2)
        str[e--] = (*data & i) ? '1' : '0';
    str[len] = '\0';
}

void print_players_constrains(unsigned int *constrains) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        char str[MAX_PLAYERS + 1] = "";
        binary_string(str, &constrains[i], MAX_PLAYERS);
        printf("Player %d: %s\n", i, str);
    }
}

void print_groups(unsigned int *groups) {
    for (int i = 0; i < MAX_GROUPS; i++) {
        char str[MAX_PLAYERS + 1] = "";
        binary_string(str, &groups[i], MAX_PLAYERS);
        printf("Group %d: %s\n", i, str);
    }
}

unsigned int generate_group_combinations(unsigned int position, int offset, unsigned int *combinations,
                                         unsigned int combinations_count, unsigned int available_players_mask,
                                         unsigned int *constrains) {
    unsigned int current_combination = combinations[combinations_count];

    // Trivial case:
    if (position >= PLAYERS_PER_GROUP) {
        // New valid combination found. Update and return the total amount of combinations found
        combinations_count++;
        combinations[combinations_count] = current_combination;
        return combinations_count;
    }

    // Recursive case:
    unsigned int player = 0;
    player |= 1UL << offset;

    // Search next available player for current combination
    for (int i = offset; i < MAX_PLAYERS; i++) {
        // Check if the player is available in the current week. And check if player is not present in the current combination. And check if player did not joined any group with any of the players of the current combination.
        unsigned int player_constrains = constrains[i]; // Get player constrains
        if ((player & available_players_mask) && !(player & current_combination) && !(player_constrains & current_combination)) {
            current_combination = player | current_combination; // Add current player into the current group combination
            combinations[combinations_count] = current_combination;
            available_players_mask &= ~(1UL << i); // Remove the current player from the available players mask
            constrains[i] |= current_combination; // Add constrains to the current player
            // TODO: Actualitzar les constrains a cada un dels players de la combinació actual amb el player actual

            combinations_count = generate_group_combinations(position + 1, i + 1, combinations, combinations_count, available_players_mask, constrains);

            // Uncomment the following line if the algorithm is not working properly
            //combinations[combinations_count] = current_combination;

            //constrains[i] // TODO: Fer un NAND per tornar a restablir les constrains del player
            // TODO: Restaurar les constrains de cada un dels players de la combinació actual amb el player actual
            available_players_mask |= 1UL << i; // Add the current player to the available players mask again
            current_combination &= ~(1UL << i); // Remove the current player from the current combination
        }

        // Uncomment the following line if the algorithm is not working properly
        //combinations[combinations_count] = current_combination;
        player *= 2; // Iterate to the next player just by shifting one bit
    }

    return combinations_count;
}

void solve_group(unsigned int *weeks, unsigned int *constrains, int week_index, int group_index) {

    if (week_index >= MAX_WEEKS) {
        // Solved the Social Golfer Problem by completing the 10 weeks!
        printf("Solved the Social Golfer Problem by completing the 10 weeks!\n");
        exit(0);
    }

    unsigned int *week_groups = &weeks[week_index];

    if (group_index >= MAX_GROUPS) {
        // We solved the Social Golfer Problem up to the current week!
        printf(">>> Solved the SGP up to week number %d\n", week_index + 1);

        // Continue solving the next week
        unsigned int *week_groups = &weeks[week_index + 1];
        solve_group(weeks, constrains, week_index + 1, 0);
    }

    week_groups[group_index] = 0; // Reset current group

    // Now we need to get all the available players that can join this group in the current state.
    // Rule 1) Use only available players within the current week.

    // Apply a NOR operation to all the groups will give us the available players in the current week
    unsigned int available_players_mask = 0;
    for (int i = 0; i < group_index; i++)
        available_players_mask = available_players_mask | week_groups[i]; // OR
    available_players_mask = ~available_players_mask; // NOT

    // Now we will generate all non-repetitive players combinations with the available players in the current group
    // Rule 2) Combinations cannot contain players that are in the same group in previous weeks.

    // 143840 bytes of memory needed to store all the possible combinations. MAX_GROUP_COMBINATIONS * sizeof(unsigned int)
    unsigned int group_combinations[MAX_GROUP_COMBINATIONS] = {0};

    // Generate non-repetitive combinations of 4 players using all current available players (32 at max)
    unsigned int combinations_count = generate_group_combinations(0, 0, group_combinations, 0, available_players_mask, constrains);
    printf("%d COMBINATIONS FOUND FOR GROUP %d\n", combinations_count, group_index);

    for (int c = 0; c < combinations_count; c++) {
        week_groups[group_index] = group_combinations[c];
/*        char str[MAX_PLAYERS + 1] = "";
        binary_string(str, &week_groups[group_index], MAX_PLAYERS);
        printf("Current combination for group %d: %s\n", group_index, str);*/
        solve_group(weeks, constrains, week_index, group_index + 1);
    }
}

int main(int argc, char *argv[]) {
    printf("Social Golfer Problem solver\n\n");

    // Weeks definition
    unsigned int weeks[MAX_WEEKS][MAX_GROUPS] = {0};

    // Definition of pairs of players taken
    unsigned int constrains[MAX_PLAYERS] = {0};

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
    //print_groups(weeks[0]);

    // Print player constrains
    //print_players_constrains(constrains);

    // Start solving from group 0 of week 0
    solve_group(weeks, constrains, 0, 0);
}
