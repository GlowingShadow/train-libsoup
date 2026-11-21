#!/bin/sh
cd /workspaces/train-libsoup
find src -name '*.c' -o -name '*.h' | xargs clang-format -i
