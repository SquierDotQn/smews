#define FIELD_SIZE 8
#define FIELD_GEN 15
#define FAKE 3

extern char field[];
void evolve(const char *field, char *t, int size);
void game(int nb_gens, char[] in_field);
