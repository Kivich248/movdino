#ifndef FUNCTIONS_H
#define FUNCTIONS_H


struct field move(struct field Field, char line[][51], int line_number);
struct field paint(struct field Field, char line[][51], int line_number);
struct field dig(struct field Field, char line[][51], int line_number);
struct field mound(struct field Field, char line[][51], int line_number);
struct field jump(struct field Field, char line[][51], int line_number);
struct field grow(struct field Field, char line[][51], int line_number);
struct field cut(struct field Field, char line[][51], int line_number);
struct field make(struct field Field, char line[][51], int line_number);
struct field push(struct field Field, char line[][51], int line_number);
struct field handle_if(struct field Field, char line[][51], char subline[50][51], int line_number);
struct field undo(const struct field Field, char line[][51], int line_number);

#endif //FUNCTIONS_H