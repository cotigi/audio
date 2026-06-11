#include "note.h"
#include <stdbool.h>

#ifndef SHEET_HEADER
#define SHEET_HEADER
#define INCREMENT 2.0 * M_PI / SAMPLE_RATE

typedef struct {
    float duration;
    int length;
    int *num_groups;
    Note *notes;
} Sheet;

typedef struct {
    int file_length;
    int line_length;
    int note_count;
    char *matrix;
} SheetCTX;
#endif

bool get_sheet(Sheet* sheet, char filename[]);
void process_note(Note *note, SheetCTX *ctx, Sheet *sheet, int y, int x, int pos);
Note* contains(Note *notes, int num_group, int octave, char key, int pos);
double calc_group(Note *notes, int pos, int count);
void free_sheet(Sheet *sheet);
