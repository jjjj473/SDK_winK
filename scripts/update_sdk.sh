#!/bin/sh
# Update SDK to the latest version and rebuild
set -e
if [ ! -d .git ]; then
    echo "No git repository found" >&2
    exit 1
fi
REMOTE="https://github.com/jjjj473/SDK_winK.git"
BRANCH="codex/build-sdk-for-wink-on-arch-linux"
git fetch "$REMOTE" "$BRANCH"
git reset --hard FETCH_HEAD
make
