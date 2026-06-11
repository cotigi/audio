#include "note.h"

#ifndef SHEET_HEADER
#define SHEET_HEADER
#define INCREMENT 2.0 * M_PI / SAMPLE_RATE

typedef struct {
    float duration;
    int num_notes;
    int *num_groups;
    Note ***notes;
} Sheet;

typedef struct {
    int file_length;
    int line_length;
    char *matrix;
} SheetCTX;
#endif

bool get_sheet(Sheet* sheet, char filename[]);
Note* process_cell(SheetCTX *ctx, Sheet *sheet, int y, int x);
Note* contains(Note **notes, int num_group, int octave, char key);
double calc_group(Note **notes, int n);
void free_sheet(Sheet *sheet);
