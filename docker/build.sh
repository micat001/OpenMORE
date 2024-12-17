#!/bin/bash
cd ../../ && \
    DOCKER_BUILDKIT=1 docker build --progress=plain . \
        --file OpenMORE/docker/Dockerfile \
        --target base \
        --tag  openmore:latest
