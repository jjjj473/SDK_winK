/* wink_sdk.c - Single-file C SDK for managing Wine-based environments on Arch Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* Helper macro to create simple winetricks wrappers */
#define WINK_SIMPLE_VERB(name, verb)                          \
int wink_##name(const char *prefix)                          \
{                                                            \
    char cmd[256];                                           \
    if (prefix && strlen(prefix) > 0)                        \
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s winetricks -q %s", prefix, verb); \
    else                                                     \
        snprintf(cmd, sizeof(cmd), "winetricks -q %s", verb); \
    return system(cmd);                                      \
}

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

/* Install .NET runtime using winetricks */
int wink_install_dotnet(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q dotnet48", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q dotnet48");
    return system(command);
}

/* Install Visual C++ runtimes */
int wink_install_vcrun(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q vcrun2019", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q vcrun2019");
    return system(command);
}

/* Set the Windows version for the prefix */
int wink_set_windows_version(const char *prefix, const char *version)
{
    char command[256];
    if (!version)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q win%s", prefix, version);
    else
        snprintf(command, sizeof(command), "winetricks -q win%s", version);
    return system(command);
}

/* Backup registry to a file */
int wink_backup_registry(const char *prefix, const char *output)
{
    char command[512];
    if (!output)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s regedit /E %s", prefix, output);
    else
        snprintf(command, sizeof(command), "regedit /E %s", output);
    return system(command);
}

/* Restore registry from a file */
int wink_restore_registry(const char *prefix, const char *input)
{
    char command[512];
    if (!input)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s regedit %s", prefix, input);
    else
        snprintf(command, sizeof(command), "regedit %s", input);
    return system(command);
}

/* Install Gallium Nine for improved D3D9 performance */
int wink_install_gallium_nine(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q galliumnine", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q galliumnine");
    return system(command);
}

/* Enable esync in the prefix */
int wink_setup_esync(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q settings esync=enabled", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q settings esync=enabled");
    return system(command);
}

/* Enable fsync in the prefix */
int wink_setup_fsync(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q settings fsync=enabled", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q settings fsync=enabled");
    return system(command);
}

/* Install common fonts */
int wink_install_fonts(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q corefonts", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q corefonts");
    return system(command);
}

/* Enable virtual desktop mode */
int wink_enable_virtual_desktop(const char *prefix, int width, int height)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q vd=%dx%d", prefix, width, height);
    else
        snprintf(command, sizeof(command), "winetricks -q vd=%dx%d", width, height);
    return system(command);
}

/* Disable virtual desktop mode */
int wink_disable_virtual_desktop(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q vd=off", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q vd=off");
    return system(command);
}

/* Run Wine uninstaller */
int wink_run_uninstaller(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s wine uninstaller", prefix);
    else
        snprintf(command, sizeof(command), "wine uninstaller");
    return system(command);
}

/* Set a DLL override to native */
int wink_install_override_dll(const char *prefix, const char *dll)
{
    char command[512];
    if (!dll)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q dlloverride=%s=n", prefix, dll);
    else
        snprintf(command, sizeof(command), "winetricks -q dlloverride=%s=n", dll);
    return system(command);
}

/* Remove a DLL override */
int wink_remove_override_dll(const char *prefix, const char *dll)
{
    char command[512];
    if (!dll)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q dlloverride=%s=", prefix, dll);
    else
        snprintf(command, sizeof(command), "winetricks -q dlloverride=%s=", dll);
    return system(command);
}

/* Execute a configuration script inside the prefix */
int wink_run_config_script(const char *prefix, const char *script)
{
    char command[512];
    if (!script)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s bash %s", prefix, script);
    else
        snprintf(command, sizeof(command), "bash %s", script);
    return system(command);
}

/* Clean cached files in the prefix */
int wink_clean_cache(const char *prefix)
{
    char command[512];
    if (!prefix)
        return 1;
    snprintf(command, sizeof(command), "rm -rf %s/drive_c/windows/temp/*", prefix);
    return system(command);
}

/* Extract an MSI installer */
int wink_extract_msi(const char *prefix, const char *msi)
{
    char command[512];
    if (!msi)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s msiexec /a %s /qb TARGETDIR=%s/msi_extract", prefix, msi, prefix);
    else
        snprintf(command, sizeof(command), "msiexec /a %s /qb TARGETDIR=./msi_extract", msi);
    return system(command);
}

/* Register a DLL with regsvr32 */
int wink_run_regsvr(const char *prefix, const char *dll)
{
    char command[512];
    if (!dll)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s regsvr32 %s", prefix, dll);
    else
        snprintf(command, sizeof(command), "regsvr32 %s", dll);
    return system(command);
}

/* Limit CPU cores for Wine processes */
int wink_set_cpu_cores(const char *prefix, int cores)
{
    char command[512];
    if (cores <= 0)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "taskset -c 0-%d WINEPREFIX=%s wineserver -w", cores - 1, prefix);
    else
        snprintf(command, sizeof(command), "taskset -c 0-%d wineserver -w", cores - 1);
    return system(command);
}

/* Enable PBA */
int wink_enable_pba(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q settings pba=enabled", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q settings pba=enabled");
    return system(command);
}

/* Disable PBA */
int wink_disable_pba(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winetricks -q settings pba=disabled", prefix);
    else
        snprintf(command, sizeof(command), "winetricks -q settings pba=disabled");
    return system(command);
}

/* Launch winecfg for the prefix */
int wink_run_winecfg(const char *prefix)
{
    char command[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s winecfg", prefix);
    else
        snprintf(command, sizeof(command), "winecfg");
    return system(command);
}

/* Open a directory in Wine explorer */
int wink_run_explorer(const char *prefix, const char *path)
{
    char command[512];
    if (!path)
        return 1;
    if (prefix && strlen(prefix) > 0)
        snprintf(command, sizeof(command), "WINEPREFIX=%s wine explorer %s", prefix, path);
    else
        snprintf(command, sizeof(command), "wine explorer %s", path);
    return system(command);
}

/* Create a simple desktop shortcut */
int wink_create_desktop_shortcut(const char *prefix, const char *name, const char *exe)
{
    if (!name || !exe)
        return 1;
    FILE *fp = fopen(name, "w");
    if (!fp)
        return 1;
    fprintf(fp, "[Desktop Entry]\n");
    fprintf(fp, "Type=Application\n");
    fprintf(fp, "Name=%s\n", name);
    if (prefix && strlen(prefix) > 0)
        fprintf(fp, "Exec=env WINEPREFIX=%s wine %s\n", prefix, exe);
    else
        fprintf(fp, "Exec=wine %s\n", exe);
    fclose(fp);
    return 0;
}

/* ------ Additional helper wrappers (60 total new tools) ------ */
/* winetricks verbs */
WINK_SIMPLE_VERB(install_dxvk, "dxvk")
WINK_SIMPLE_VERB(install_openal, "openal")
WINK_SIMPLE_VERB(install_quartz, "quartz")
WINK_SIMPLE_VERB(install_msxml6, "msxml6")
WINK_SIMPLE_VERB(install_physx, "physx")
WINK_SIMPLE_VERB(install_xact, "xact")
WINK_SIMPLE_VERB(install_directmusic, "dmusic")
WINK_SIMPLE_VERB(install_d3dx9, "d3dx9")
WINK_SIMPLE_VERB(install_d3dcompiler, "d3dcompiler_47")
WINK_SIMPLE_VERB(install_wmp, "wmp9")
WINK_SIMPLE_VERB(install_gstreamer, "gstreamer")
WINK_SIMPLE_VERB(install_vkd3d, "vkd3d")
WINK_SIMPLE_VERB(install_ffmpeg, "ffmpeg")
WINK_SIMPLE_VERB(install_winhttp, "winhttp")
WINK_SIMPLE_VERB(install_dotnet35, "dotnet35")
WINK_SIMPLE_VERB(install_mfc, "mfc42")
WINK_SIMPLE_VERB(install_gdiplus, "gdiplus")
WINK_SIMPLE_VERB(install_msxml3, "msxml3")
WINK_SIMPLE_VERB(install_msxml4, "msxml4")
WINK_SIMPLE_VERB(install_vcrun6, "vcrun6")
WINK_SIMPLE_VERB(install_vcrun2005, "vcrun2005")
WINK_SIMPLE_VERB(install_vcrun2008, "vcrun2008")
WINK_SIMPLE_VERB(install_vcrun2010, "vcrun2010")
WINK_SIMPLE_VERB(install_vcrun2012, "vcrun2012")
WINK_SIMPLE_VERB(install_vcrun2013, "vcrun2013")
WINK_SIMPLE_VERB(install_vcrun2015, "vcrun2015")
WINK_SIMPLE_VERB(install_directplay, "directplay")
WINK_SIMPLE_VERB(install_dsound, "dsound")
WINK_SIMPLE_VERB(install_devenum, "devenum")
WINK_SIMPLE_VERB(install_shell32, "shell32")
WINK_SIMPLE_VERB(install_control_panel, "controlpanel")
WINK_SIMPLE_VERB(install_wininet, "wininet")
WINK_SIMPLE_VERB(install_comctl32, "comctl32")
WINK_SIMPLE_VERB(install_riched20, "riched20")
WINK_SIMPLE_VERB(install_riched30, "riched30")
WINK_SIMPLE_VERB(install_secur32, "secur32")
WINK_SIMPLE_VERB(install_shlwapi, "shlwapi")
WINK_SIMPLE_VERB(install_urlmon, "urlmon")
WINK_SIMPLE_VERB(install_advapi32, "advapi32")
WINK_SIMPLE_VERB(install_winspool, "winspool")
WINK_SIMPLE_VERB(install_adobe_air, "adobeair")
WINK_SIMPLE_VERB(install_flash, "flash")
WINK_SIMPLE_VERB(install_chrome_frame, "chromeframe")
WINK_SIMPLE_VERB(install_d3d11, "d3d11")
WINK_SIMPLE_VERB(install_dotnet452, "dotnet452")
WINK_SIMPLE_VERB(install_directx9, "directx9")
WINK_SIMPLE_VERB(install_directx10, "directx10")
WINK_SIMPLE_VERB(install_directx11, "directx11")
WINK_SIMPLE_VERB(install_physx_legacy, "physxlegacy")

/* manual wrappers */
int wink_update_wine()
{
    return system("sudo pacman -Syu --noconfirm wine");
}

int wink_install_proton()
{
    return system("protonup -d ~/.steam/root/compatibilitytools.d -p GE-Proton");
}

int wink_update_dxvk(const char *prefix)
{
    char cmd[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s setup_dxvk.sh install --symlink", prefix);
    else
        snprintf(cmd, sizeof(cmd), "setup_dxvk.sh install --symlink");
    return system(cmd);
}

int wink_list_processes()
{
    return system("ps -ef | grep wine");
}

int wink_terminate_process(const char *name)
{
    if (!name)
        return 1;
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "pkill -f %s", name);
    return system(cmd);
}

int wink_install_game_msi(const char *prefix, const char *msi)
{
    if (!msi)
        return 1;
    char cmd[512];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s msiexec /i %s /qb", prefix, msi);
    else
        snprintf(cmd, sizeof(cmd), "msiexec /i %s /qb", msi);
    return system(cmd);
}

int wink_run_control_panel(const char *prefix)
{
    char cmd[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s wine control", prefix);
    else
        snprintf(cmd, sizeof(cmd), "wine control");
    return system(cmd);
}

int wink_get_winepath(const char *unix_path, char *out, size_t len)
{
    if (!unix_path || !out || len == 0)
        return 1;
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "winepath '%s'", unix_path);
    FILE *fp = popen(cmd, "r");
    if (!fp)
        return 1;
    if (!fgets(out, len, fp)) {
        pclose(fp);
        return 1;
    }
    out[strcspn(out, "\n")] = '\0';
    pclose(fp);
    return 0;
}

int wink_get_native_path(const char *win_path, char *out, size_t len)
{
    if (!win_path || !out || len == 0)
        return 1;
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "winepath -u '%s'", win_path);
    FILE *fp = popen(cmd, "r");
    if (!fp)
        return 1;
    if (!fgets(out, len, fp)) {
        pclose(fp);
        return 1;
    }
    out[strcspn(out, "\n")] = '\0';
    pclose(fp);
    return 0;
}

int wink_run_cmd(const char *prefix, const char *command)
{
    if (!command)
        return 1;
    char cmd[512];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s wine cmd /c %s", prefix, command);
    else
        snprintf(cmd, sizeof(cmd), "wine cmd /c %s", command);
    return system(cmd);
}

int wink_fix_wineprefix(const char *prefix)
{
    char cmd[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s winetricks --force setupapi", prefix);
    else
        snprintf(cmd, sizeof(cmd), "winetricks --force setupapi");
    return system(cmd);
}

int wink_enable_hud(const char *prefix)
{
    char cmd[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s DXVK_HUD=1 winecfg", prefix);
    else
        snprintf(cmd, sizeof(cmd), "DXVK_HUD=1 winecfg");
    return system(cmd);
}

int wink_disable_hud(const char *prefix)
{
    char cmd[256];
    if (prefix && strlen(prefix) > 0)
        snprintf(cmd, sizeof(cmd), "WINEPREFIX=%s DXVK_HUD= winecfg", prefix);
    else
        snprintf(cmd, sizeof(cmd), "DXVK_HUD= winecfg");
    return system(cmd);
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
