#debian:13.3-slim amd64
ARG IMAGE=debian:13.3-slim

FROM ${IMAGE} AS cpp-builder

RUN apt update && apt install -y --no-install-recommends \
    g++ \
    cmake \ 
    make 

COPY ./source/solver/ /build/
WORKDIR /build/
    
RUN mkdir build && cd build && \
    cmake .. && \
    make 
    
RUN mkdir /output && mkdir /output/lib/
RUN mv ./build/libsolver.so /output/lib/

FROM ${IMAGE} AS python-builder

RUN apt update && apt install -y --no-install-recommends \
    python3.13 \
    python3.13-venv \
    python3-pip \
    libpython3.13 \
    binutils

COPY ./source/flask/ /build/
WORKDIR /build/

RUN python3 -m venv ./venv && \
    ./venv/bin/pip3 install -r requirements.txt

RUN ./venv/bin/pyinstaller --onefile \
    --name="automaton-web" \
    --add-data="templates:templates" \
    main.py

RUN mkdir /output
RUN mv ./dist/automaton-web /output/

FROM ${IMAGE} AS runtime

RUN apt update && \
    apt install -y --no-install-recommends libstdc++6 && \ 
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=cpp-builder /output/. .
COPY --from=python-builder /output/. .

CMD ["./automaton-web"]