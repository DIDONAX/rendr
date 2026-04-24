FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-14 g++-14 \
    cmake ninja-build \
    pkg-config \
    git \
    python3 python3-pip python3-jinja2 \
    nodejs npm \
    libgl1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev \
    libxcursor-dev libxi-dev \
    libwayland-dev wayland-protocols \
    libxkbcommon-dev \
 && rm -rf /var/lib/apt/lists/*
