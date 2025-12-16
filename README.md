# Simple TCP File Server

## Description
This is a **simple TCP HTTP-style file server** implemented in C++ that handles `GET` requests from clients. The server:  
- Serves files and directories from a specified root directory  
- Normalizes and sanitizes paths (`..` paths blocked, duplicate slashes removed)  
- Skips hidden files (files starting with `.`)  
- Sends appropriate error messages for invalid requests or missing files  

**Note:** Dynamic memory is allocated using `malloc`/`free`. In production code, modern C++ would use `std::vector` or `std::string`.

---

## Features
- Handles simple `GET` requests  
- Directory listing if `index.html` is not found  
- Works with `telnet` or `nc` (netcat) on the specified port  
- Basic security: prevents directory traversal (`..`) and blocks unsafe paths  

---

## Requirements
- Linux / UNIX environment  
- C++ compiler (`g++`)  
- `telnet` or `nc` (netcat) for testing  
- Optional: `make` (for Makefile build)  

---

## Build

### Option 1: Compile manually with g++
```bash
g++ -o tcp src/main.cpp src/server.cpp src/request.cpp src/utils.cpp
```
## Option 2: Compile with Makefile
```bash
make
```

## Usage
```bash
./tcp <port> <root-directory>
```

**Example**
```bash
./tcp 9000 ./webroot
```

## Running with Shell Script (Optional)
- You can also use the included run_server.sh script for convenience:
- Default port 9000
- Default root directory: current directory
```bash
./run_server.sh 9000 ./webroot
```

## Testing with telnet/nc
```bash
telnet localhost 9000
```

or
```bash
nc localhost 9000
```

## Docker Usage
- Build and run the server in a container:
```bash
# Build the docker image
docker build -t tcp-server .

# Run the container and map to port 9000
docker run -p 9000:9000 tcp-server
```
-You can still access via telnet or netcat just like above. This demonstrates basic containerization.

## Example session:
```text
GET /index.html
Client requested: GET /index.html
<contents of index.html printed here>
Connection closed by foreign host.
```

## For directories without index.html:
```text
GET /files
Client requested: GET /files
file1.txt file2.txt subfolder
Connection closed by foreign host.
```

## Security/Notes
- Blocks requests containing .. to prevent directory traversal
- Hidden files (starting with .) are skipped
- Only handles GET requests; other HTTP methods return an error
- Memory management is done manually (malloc/free)

## Academic Use

This project is my original work. Copying for school assignments or other academic submissions
without permission is prohibited and may violate academic honesty policies.
