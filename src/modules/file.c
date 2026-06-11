#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/modules/sheet.h"
#include "../../include/modules/note.h"
#include "../../include/modules/file.h"

FILE* write_header(Sheet* sheet, char filename[]) {
    PCMHeader header;

    memcpy(header.chunkID, "RIFF", 4);
    memcpy(header.format, "WAVE", 4);
    memcpy(header.subChunk1ID, "fmt ", 4);

    header.subChunk1Size = 16; // PCM format size
    header.audioFormat = 1;    // PCM = 1
    header.numChannels = 1;    // Mono audio
    header.sampleRate = SAMPLE_RATE;
    header.bitsPerSample = 16;
    header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    memcpy(header.subChunk2ID, "data", 4);
    header.subChunk2Size = SAMPLE_RATE * sheet->duration * header.numChannels * header.bitsPerSample / 8;
    header.chunkSize = 36 + header.subChunk2Size;

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Could not open file: %s", filename);
        return NULL;
    }

    fwrite(&header, sizeof(PCMHeader), 1, file);
    return file;
}

bool read_sheet(SheetCTX* ctx, char filename[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s", filename);
        return false;
    }

    *ctx = read_lines(file);
    rewind(file);

    int y = 0;
    int x = 0;
    for (int ch; (ch = getc(file)) != EOF; ) {
        if (ch == '\n') {
            ++y;
            x = 0;
        } else {
            const size_t pos = ctx->line_length * y + x;
            ctx->matrix[pos] = ch;
            ++x;
        }
    }

    fclose(file);

    return true;
}

SheetCTX read_lines(FILE* file) {
    int file_length = 0;
    int max_line_length = 0;
    int line_length = 0;

    for (int ch; (ch = getc(file)) != EOF; ) {
        if (ch == '\n') {
            ++file_length;
            if (max_line_length < line_length) {
                max_line_length = line_length;
            }
            line_length = 0;
        } else {
            ++line_length;
        }
    }

    SheetCTX ctx = {
        .file_length = file_length,
        .line_length = max_line_length,
        .matrix      = malloc(sizeof(char)*file_length*max_line_length)
    };

    return ctx;
}
