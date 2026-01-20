FROM alpine:3.23.2 AS builder

RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    make 
    
WORKDIR /app
COPY ../source /app/source
COPY ../main.cpp ../CMakeLists.txt /app/

RUN mkdir build && cd build && \
    cmake .. && \
    make 
    
FROM alpine:3.23.2
    
RUN apk update && apk add --no-cache \
    lighttpd \
    imagemagick
    
WORKDIR /app
COPY --from=builder ./app/build/solve-ant-problem /app
COPY ../start.sh /app/
COPY ../http-server /app/http-server/

RUN echo 'server.document-root = "/app/http-server"' > /etc/lighttpd/lighttpd.conf && \
    echo 'server.port = 8085' >> /etc/lighttpd/lighttpd.conf && \
    echo 'index-file.names = ( "index.html" )' >> /etc/lighttpd/lighttpd.conf && \
    echo 'server.modules += ( "mod_cgi" )' >> /etc/lighttpd/lighttpd.conf && \
    echo 'cgi.assign = ( ".cgi" => "/bin/sh" )' >> /etc/lighttpd/lighttpd.conf 

RUN chmod +x /app/start.sh 
RUN chmod +x /app/http-server/solve.cgi 

CMD ["/app/start.sh"]
