cd /workspaces/train-libsoup
meson setup build
meson compile -C build
./build/api-server
cd -