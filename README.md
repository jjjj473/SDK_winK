# SDK_winK

SDK winK is a small C toolkit for managing Wine on Arch Linux. It aims to make
it easy to run Windows games (including Roblox) and packages them with simple
helpers. The entire SDK logic lives in **wink_sdk.c** with a companion
**wink_errors.cpp** providing C++ error utilities.

## Building

```sh
make
```
This produces `libwink.a` which can be linked with your projects.

## Updating

Run `scripts/update_sdk.sh` to automatically pull the latest version of the SDK
from the `codex/build-sdk-for-wink-on-arch-linux` branch of the
`jjjj473/SDK_winK` repository and rebuild the library. You can also call
`wink_self_update(NULL)` from your program to update the SDK at runtime using
the same branch.

## Example

```c
#include "wink_sdk.c" // single-file usage

int main() {
    wink_setup("/tmp/wink-prefix");
    wink_run("/tmp/wink-prefix", "notepad.exe", NULL);
    return 0;
}
```

Compile with:

```sh
gcc example.c libwink.a -o example -ldl
```

## Status
The project is experimental and demonstrates how a lightweight single-file C
SDK can wrap Wine commands. More robust features will arrive in later versions.

## Available helpers

The SDK exposes many helpers for common Wine tasks (over two hundred in total). Some examples include:

- `wink_install_mono()` and `wink_install_gecko()` install Mono and Gecko via winetricks.
- `wink_run_winetricks()` executes custom winetricks commands.
- `wink_set_registry()` imports registry files.
- `wink_get_wine_version()` returns the Wine version string.
- `wink_prefix_exists()` and `wink_delete_prefix()` manage prefix directories.
- `wink_export_prefix()` and `wink_import_prefix()` package prefixes to archives.
- `wink_kill_processes()` stops leftover Wine processes.
- `wink_run_dxvk_setup()` installs DXVK.
- `wink_cleanup_prefix()` removes temporary files.
- `wink_run_with_env()` runs programs with custom environment variables.
- `wink_install_dotnet()` installs .NET runtimes.
- `wink_install_vcrun()` installs Visual C++ runtimes.
- `wink_set_windows_version()` sets the emulated Windows version.
- `wink_backup_registry()` and `wink_restore_registry()` back up and restore registry files.
- `wink_install_gallium_nine()` enables Gallium Nine.
- `wink_setup_esync()` and `wink_setup_fsync()` toggle esync/fsync.
- `wink_install_fonts()` installs core fonts.
- `wink_enable_virtual_desktop()` and `wink_disable_virtual_desktop()` manage virtual desktop mode.
- `wink_run_uninstaller()` launches the Wine uninstaller.
- `wink_install_override_dll()` and `wink_remove_override_dll()` handle DLL overrides.
- `wink_run_config_script()` executes scripts within the prefix.
- `wink_clean_cache()` cleans temporary caches.
- `wink_extract_msi()` extracts MSI installers.
- `wink_run_regsvr()` registers DLLs.
- `wink_set_cpu_cores()` limits core usage.
- `wink_enable_pba()` and `wink_disable_pba()` toggle PBA.
- `wink_run_winecfg()` opens Wine configuration.
- `wink_run_explorer()` opens paths via Wine explorer.
- `wink_create_desktop_shortcut()` generates simple desktop shortcuts.
- `wink_update_wine()` updates the system Wine package.
- `wink_install_proton()` fetches a Proton GE build.
- `wink_install_dxvk()` installs DXVK via winetricks.
- `wink_update_dxvk()` updates DXVK inside a prefix.
- `wink_list_processes()` lists running Wine processes.
- `wink_terminate_process()` kills a given Wine process.
- `wink_install_game_msi()` installs MSI games silently.
- `wink_run_control_panel()` opens the Wine control panel.
- `wink_get_winepath()` converts Unix paths to Windows format.
- `wink_get_native_path()` converts Windows paths to Unix format.
- `wink_run_cmd()` runs commands through `cmd /c`.
- `wink_fix_wineprefix()` applies basic prefix fixes.
- `wink_enable_hud()` and `wink_disable_hud()` toggle the DXVK HUD.
- `wink_install_7zip()` installs 7-Zip.
- `wink_install_steam()` installs Steam.
- `wink_install_vlc()` installs the VLC media player.
- `wink_install_utorrent()` installs µTorrent.
- `wink_install_office2013pro()` installs Microsoft Office 2013.
- `wink_self_update()` pulls the latest SDK changes and rebuilds.
