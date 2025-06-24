#!/bin/sh
# Update SDK to the latest version and rebuild
set -e
if [ ! -d .git ]; then
    echo "No git repository found" >&2
    exit 1
fi
git pull --ff-only
make
