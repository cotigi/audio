#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "../../include/modules/sheet.h"
#include "../../include/modules/file.h"

bool get_sheet(Sheet* sheet, char filename[]) {
    SheetCTX ctx;
    if (!read_sheet(&ctx, filename))
    {
        return false;
    }
    sheet->duration = 0.0;
    sheet->num_notes = ctx.file_length-2;
    sheet->num_groups = malloc(sizeof(int)*(ctx.file_length-2));
    sheet->notes = malloc(sizeof(Note**)*(ctx.file_length-2));

    for (int y = 2; y < ctx.file_length; y++) {
        Note **notes = malloc(sizeof(Note*));
        int size = 0;

        for (int x = 0; x < ctx.line_length; x++) {
            Note *note = process_cell(&ctx, sheet, y, x);
            if (note != NULL) {
                sheet->duration += duration(note->duration);
                size++;
                notes = realloc(notes, size * sizeof(Note*));
                notes[size-1] = note;
            }
        }

        sheet->num_groups[y-2] = size;
        if (size) {
            sheet->notes[y-2] = notes;
        } else {
            sheet->notes[y-2] = NULL;
        }
    }

    sheet->duration = duration(EIGHT)*sheet->num_notes;

    free(ctx.matrix);

    return true;
}

Note* process_cell(SheetCTX *ctx, Sheet *sheet, int y, int x) {
    int ch = ctx->matrix[y * ctx->line_length + x];

    if (ch != 'X' && ch != '.') { return NULL; }

    Note *note = malloc(sizeof(Note));

    // Get the octave indicator from the first line
    int octave = ctx->matrix[x] - '0';
    // Get the key indicator from the second line
    char key = ctx->matrix[ctx->line_length + x];

    note->octave = octave;
    note->key = key;
    note->phase = 0.0;
    note->freq = freq(octave, key);
    note->duration = QUARTER;

    switch (ch) {
        case 'X':
            note->origin = NULL;
            break;
        case '.':
            note->origin = contains(
                sheet->notes[y-2],
                sheet->num_groups[y-2],
                octave,
                key
            );
            break;
    }

    return note;
}

Note* contains(Note **notes, int num_group, int octave, char key) {
    for (int x = 0; x < num_group; x++) {
        if (notes[x]->octave == octave && notes[x]->key == key) {
            return notes[x];
        }
    }

    return NULL;
}

double calc_group(Note **notes, int n) {
    double amp = 0.0;

    for (int i = 0; i < n; i++) {
        Note *origin = (notes[i]->origin) ? notes[i]->origin : notes[i];

        amp += sin(origin->phase);

        origin->phase += INCREMENT * origin->freq;
    }

    return amp/n;
}

void free_sheet(Sheet *sheet) {
    for (int y = 0; y < sheet->num_notes; y++) {
        for (int x = 0; x < sheet->num_groups[y]; x++) {
            if (sheet->notes[y][x]->origin) { 
                free(sheet->notes[y][x]->origin);
            }

            free(sheet->notes[y][x]);
        }

        free(sheet->notes[y]);
    }

    free(sheet->notes);
    free(sheet->num_groups);
    //free(sheet);
}
