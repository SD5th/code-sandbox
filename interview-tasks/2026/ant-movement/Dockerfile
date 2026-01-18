FROM alpine:latest AS builder

RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    make

COPY . ./app
WORKDIR /app

RUN mkdir build && cd build && \
    cmake .. && \
    make

FROM alpine:latest

COPY --from=builder /app/build/ant-movement /app/ant-movement

WORKDIR /app

CMD ["./ant-movement"]