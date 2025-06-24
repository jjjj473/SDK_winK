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
