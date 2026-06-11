#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "../../include/modules/sheet.h"
#include "../../include/modules/file.h"

bool get_sheet(Sheet* sheet, char filename[]) {
    SheetCTX ctx;
	if (!read_sheet(&ctx, sheet, filename)) {
		return false;
	}

    sheet->duration = 0.0;
    sheet->length = ctx.file_length-2;
    sheet->notes = malloc(sizeof(Note)*ctx.note_count);

    int count = 0;

    for (int y = 2; y < ctx.file_length; y++) {
        for (int x = 0; x < ctx.line_length; x++) {
            int cell = ctx.matrix[y * ctx.line_length + x];

            if (cell == 'X' || cell == '.') {
                process_note(&sheet->notes[count], &ctx, sheet, y, x, count);
                count++;
            }
        }
    }

    sheet->duration = duration(EIGHT)*sheet->length;

    free(ctx.matrix);

    return true;
}

void process_note(
        Note *note,
        SheetCTX *ctx,
        Sheet *sheet,
        int y,
        int x,
        int pos
    ) {
    int cell = ctx->matrix[y * ctx->line_length + x];
    // Get the octave indicator from the first line
    int octave = ctx->matrix[x] - '0';
    // Get the key indicator from the second line
    char key = ctx->matrix[ctx->line_length + x];

    note->octave     = octave;
    note->key        = key;
    note->phase      = 0.0;
    note->freq       = freq(octave, key);
    note->duration   = QUARTER;
    note->origin     = NULL;

    if (cell == '.' && y-2 > 0) {
        note->origin = contains(
            sheet->notes,
            sheet->num_groups[y-3],
            octave,
            key,
            pos
        );
    }
}

Note* contains(Note *notes, int count, int octave, char key, int pos) {
    for (int x = 1; x <= count; x++) {
        Note *note = &notes[pos-x];

        if (note->octave == octave && note->key == key) {
            return (note->origin) ? note->origin : note;
        }
    }

    return NULL;
}

double calc_group(Note *notes, int pos, int count) {
    float amp = 0.0;

    for (int i = 0; i < count; i++) {
        Note *note = &notes[pos+i];
        Note *origin = (note->origin) ? note->origin : note;

        amp += sin(origin->phase);

        origin->phase += INCREMENT * origin->freq;
    }

    return amp/count;
}

void free_sheet(Sheet *sheet) {
    free(sheet->notes);
    free(sheet->num_groups);
    //free(sheet);
}
