#ifndef PROVERKI_OPERACII_H
#define PROVERKI_OPERACII_H


void clear_screen();
void delay_seconds(int seconds);
void err(int a);
void pred(int a);
int ost(int a, int b);
struct kmp napr(const char *line);
struct field komanda(struct field Field, char line[][51], int line_number);
int get_number(const char *line, int line_number);
void print_field(struct field Field);
int count_commands(const char *filename, int *max_undo_out);
void undo_tuda(struct field *undo_field, int max_undo, struct field new_state);
struct field create_empty_field();
void undo_obr(struct field *undo_field, int max_undo, struct field new_state);
struct field rav(struct field Field, struct field Field_new);


#endif //PROVERKI_OPERACII_H
