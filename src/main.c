#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "../include/modules/note.h"
#include "../include/modules/sheet.h"
#include "../include/modules/file.h"

#define PCM_FILE "sine.pcm"

int main() {
    Sheet sheet;
	if (!get_sheet(&sheet, "test.sheet")){
		return EXIT_FAILURE;
	}

    FILE* file = write_header(&sheet, PCM_FILE);
    if (!file) {
        return 1;
    }

    int index = 0;

    for (int n = 0; n < sheet.length; n++) {
        int num_group = sheet.num_groups[n];

        for (int _ = 0; _ < BASE; _++) {
            double amp = calc_group(sheet.notes, index, num_group);

            int16_t sample = (int16_t)(0.25 * AMPLITUDE * amp);

            fwrite(&sample, sizeof(int16_t), 1, file);
        }

        index += num_group;
    }

    free_sheet(&sheet);

    fclose(file);

    char *command;
    //asprintf(&command, "ffplay -showmode 1 %s > /dev/null", PCM_FILE);
    asprintf(&command, "ffplay %s > /dev/null", PCM_FILE);

    system(command);
    free(command);

    printf("\nPCM file created successfully.\n");

    return 0;
}

