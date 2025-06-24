/* wink_sdk.c - Single-file C SDK for managing Wine-based environments on Arch Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initialize a Wine prefix. Returns 0 on success, non-zero on error. */
int wink_setup(const char *prefix)
{
    if (!prefix || strlen(prefix) == 0) {
        fprintf(stderr, "Invalid prefix path\n");
        return 1;
    }
    char command[512];
    snprintf(command, sizeof(command), "WINEPREFIX=%s wineboot -u", prefix);
    return system(command);
}

/* Run a Windows executable through Wine */
int wink_run(const char *prefix, const char *exe, const char *args)
{
    if (!exe) {
        fprintf(stderr, "No executable specified\n");
        return 1;
    }
    char command[1024];
    if (prefix && strlen(prefix) > 0) {
        snprintf(command, sizeof(command), "WINEPREFIX=%s wine %s %s", prefix, exe, args ? args : "");
    } else {
        snprintf(command, sizeof(command), "wine %s %s", exe, args ? args : "");
    }
    return system(command);
}

/* Package a game directory into a simple archive */
int wink_package(const char *dir, const char *output)
{
    if (!dir || !output) {
        fprintf(stderr, "Invalid arguments to wink_package\n");
        return 1;
    }
    char command[1024];
    snprintf(command, sizeof(command), "tar -czf %s -C %s .", output, dir);
    return system(command);
}

/* Example main demonstrating usage */
#ifdef WINK_SDK_DEMO
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <exe> [args]\n", argv[0]);
        return 1;
    }
    const char *prefix = getenv("WINK_PREFIX");
    return wink_run(prefix, argv[1], argc > 2 ? argv[2] : NULL);
}
#endif
