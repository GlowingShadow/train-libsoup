FROM mcr.microsoft.com/devcontainers/base:bookworm

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        meson \
        ninja-build \
        pkg-config \
        libsoup-3.0-dev \
        libglib2.0-dev \
        libjson-glib-dev \
        git \
        gdb \
        valgrind \
        clang-format \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app
