FROM ubuntu:22.04

WORKDIR /app

RUN apt-get update && \
    apt-get install -y g++ make netcat && \
    rm -rf /var/lib/apt/lists/*

COPY src ./src
COPY webroot ./webroot

RUN g++ -Wall -std=c++17 -o tcp src/main.cpp src/server.cpp src/request.cpp src/utils.cpp

EXPOSE 9000

CMD ["./tcp", "9000", "./webroot"]

