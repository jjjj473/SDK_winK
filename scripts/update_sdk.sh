#!/bin/sh
# Update SDK to the latest version and rebuild
set -e
# Determine the repository root relative to this script
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_DIR"

if [ ! -d .git ]; then
    echo "No git repository found" >&2
    exit 1
fi
REMOTE="https://github.com/jjjj473/SDK_winK.git"
BRANCH="codex/build-sdk-for-wink-on-arch-linux"
if command -v zenity >/dev/null 2>&1; then
    zenity --question --text="Click Update to fetch the latest SDK for the best experience." --ok-label=Update --cancel-label=Skip
    [ $? -ne 0 ] && exit 0
fi
git fetch "$REMOTE" "$BRANCH"
git reset --hard FETCH_HEAD
make
