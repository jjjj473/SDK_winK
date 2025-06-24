/* wink_sdk.c - Single-file C SDK for managing Wine-based environments on Arch Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

/* Install Mono runtime using winetricks */
int wink_install_mono(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q mono", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q mono");
    return system(command);
}

/* Install Gecko engine using winetricks */
int wink_install_gecko(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q gecko", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q gecko");
    return system(command);
}

/* Run arbitrary winetricks command */
int wink_run_winetricks(const char *prefix, const char *args)
{
    if (!args)
        return 0;
    char command[512];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks %s", prefix, args);
    else
        snprintf(command, sizeof(command), "winetricks %s", args);
    return system(command);
}

/* Import registry settings from a file */
int wink_set_registry(const char *prefix, const char *reg_file)
{
    if (!reg_file)
        return 1;
    char command[512];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s regedit %s", prefix, reg_file);
    else
        snprintf(command, sizeof(command), "regedit %s", reg_file);
    return system(command);
}

/* Fetch the current Wine version string (stored into buffer) */
int wink_get_wine_version(char *buf, size_t buflen)
{
    if (!buf || buflen == 0)
        return 1;
    FILE *fp = popen("wine --version", "r");
    if (!fp)
        return 1;
    if (!fgets(buf, buflen, fp))
    {
        pclose(fp);
        return 1;
    }
    buf[strcspn(buf, "\n")] = '\0';
    pclose(fp);
    return 0;
}

/* Check if a prefix directory exists */
int wink_prefix_exists(const char *prefix)
{
    struct stat st;
    return prefix && stat(prefix, &st) == 0 && S_ISDIR(st.st_mode);
}

/* Delete a Wine prefix directory */
int wink_delete_prefix(const char *prefix)
{
    if (!prefix || !wink_prefix_exists(prefix))
        return 1;
    char command[512];
    snprintf(command, sizeof(command), "rm -rf %s", prefix);
    return system(command);
}

/* Export a prefix to a compressed archive */
int wink_export_prefix(const char *prefix, const char *archive)
{
    if (!prefix || !archive)
        return 1;
    char command[512];
    snprintf(command, sizeof(command), "tar -czf %s -C %s .", archive, prefix);
    return system(command);
}

/* Import a prefix from an archive */
int wink_import_prefix(const char *archive, const char *prefix)
{
    if (!archive || !prefix)
        return 1;
    char command[512];
    snprintf(command, sizeof(command), "mkdir -p %s && tar -xzf %s -C %s", prefix, archive, prefix);
    return system(command);
}

/* Kill running Wine processes */
int wink_kill_processes()
{
    return system("wineserver -k");
}

/* Run DXVK setup script inside prefix */
int wink_run_dxvk_setup(const char *prefix)
{
    char command[512];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s setup_dxvk.sh install", prefix);
    else
        snprintf(command, sizeof(command), "setup_dxvk.sh install");
    return system(command);
}

/* Clean temporary files in a prefix */
int wink_cleanup_prefix(const char *prefix)
{
    char command[512];
    if (!prefix)
        return 1;
    snprintf(command, sizeof(command), "rm -rf %s/tmp/*", prefix);
    return system(command);
}

/* Run an executable with custom environment variables */
int wink_run_with_env(const char *prefix, const char *exe, const char *args, const char *env)
{
    if (!exe)
        return 1;
    char command[1024];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "%s WINEPREFIX=%s wine %s %s", env ? env : "", prefix, exe, args ? args : "");
    else
        snprintf(command, sizeof(command), "%s wine %s %s", env ? env : "", exe, args ? args : "");
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
