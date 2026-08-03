# syntax=docker/dockerfile:1.7
# BFA-HavenCore - multi-stage build (EXPERIMENTAL Linux port, see docker/README.md)
# BFA-HavenCore - build multi-stage (puerto EXPERIMENTAL a Linux, ver docker/README.md)

FROM ubuntu:24.04 AS build
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates gpg wget \
    && rm -rf /var/lib/apt/lists/*

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
    | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' \
    | tee /etc/apt/sources.list.d/kitware.list >/dev/null

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential clang cmake ninja-build git ca-certificates pkg-config ccache \
    libboost-all-dev libmysqlclient-dev libssl-dev libbz2-dev libreadline-dev zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
# EN: copy only what cmake actually needs to compile. Copying the whole
# repo (COPY . /src) would invalidate this layer -- and every expensive
# step after it, since layer caching is content-hash based -- whenever an
# unrelated file changes (docs, docker/ scripts, sql/ dumps, etc).
# ES: copiar solo lo que cmake realmente necesita para compilar. Copiar
# todo el repo (COPY . /src) invalidaria esta capa -- y todos los pasos
# caros que vienen despues, porque el cache de capas es por hash de
# contenido -- cada vez que cambia un archivo sin relacion (docs, scripts
# de docker/, dumps de sql/, etc).
COPY CMakeLists.txt PreLoad.cmake revision_data.h.in.cmake /src/
COPY cmake /src/cmake
COPY dep /src/dep
COPY src /src/src

# EN: ccache is content/flag-hash based, so it survives even if the ninja
# build-cache mount below gets evicted (has happened after long gaps
# between builds / Docker Desktop restarts): a "cold" /build dir still hits
# the ccache mount per-file instead of recompiling from scratch.
# ES: ccache trabaja por hash de contenido/flags, asi que sobrevive aunque
# se pierda el cache mount de ninja de abajo (paso despues de mucho tiempo
# entre builds / reinicios de Docker Desktop): un /build "frio" igual
# aprovecha el mount de ccache archivo por archivo en vez de recompilar
# todo de cero.
ENV CCACHE_DIR=/ccache
ENV CCACHE_MAXSIZE=15G
# EN: recommended sloppiness for GCC precompiled headers (USE_COREPCH/
# USE_SCRIPTPCH are on by default in this project) so ccache doesn't miss
# on PCH-related timestamps.
# ES: sloppiness recomendada para precompiled headers de GCC (USE_COREPCH/
# USE_SCRIPTPCH estan on por defecto en este proyecto) para que ccache no
# falle el cache por timestamps relacionados al PCH.
ENV CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime

ARG BUILD_JOBS=0
RUN --mount=type=cache,target=/build \
    --mount=type=cache,target=/ccache \
    JOBS=${BUILD_JOBS}; \
    if [ "$JOBS" = "0" ]; then JOBS=$(nproc); fi; \
    cmake -S /src -B /build -G Ninja \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DTOOLS=0 \
      -DSCRIPTS=static \
      -DWITHOUT_GIT=1 \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
      -DCMAKE_INSTALL_PREFIX=/opt/bfacore \
      -DCMAKE_CXX_FLAGS="-D__cdecl=" \
    && cmake --build /build -j"$JOBS" \
    && cmake --build /build --target install \
    && ccache --show-stats

# ---------------------------------------------------------------------------

FROM ubuntu:24.04 AS runtime
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libmysqlclient21 \
    libssl3t64 \
    libreadline8t64 \
    zlib1g \
    libbz2-1.0 \
    libboost-system1.83.0 \
    libboost-filesystem1.83.0 \
    libboost-thread1.83.0 \
    libboost-program-options1.83.0 \
    libboost-iostreams1.83.0 \
    libboost-regex1.83.0 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/bfacore /opt/bfacore
COPY docker/entrypoint.sh /opt/bfacore/entrypoint.sh
RUN chmod +x /opt/bfacore/entrypoint.sh

WORKDIR /opt/bfacore/bin
ENTRYPOINT ["/opt/bfacore/entrypoint.sh"]
