FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential cmake ninja-build \
    libgl1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev \
    libxcursor-dev libxi-dev \
    git \
    && rm -rf /var/lib/apt/lists/*
