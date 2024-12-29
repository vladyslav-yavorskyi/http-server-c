# Simple HTTP Server in C

This project implements a basic HTTP server in C capable of serving static files from a `public/` directory. The server handles HTTP GET requests and provides appropriate responses, including `200 OK` for successful requests and `404 Not Found` for missing files.

## Features
- Serves static files from the `public/` directory.
- Sends appropriate HTTP headers for each response.
- Handles multiple connections using `fork()`.
- Provides simple error handling (e.g., 404 responses).

## How It Works
1. The server listens for incoming TCP connections on a specified port.
2. For each request:
   - Parses the HTTP request to identify the requested file path.
   - Maps the file path to the `public/` directory.
   - Sends the file's contents with the appropriate HTTP headers.
3. If the file doesn't exist, the server returns a 404 response with an error page.

## Configuration
Modify `config.h` to customize server settings:
#define PORT 8080             // Port number
#define MAX_CONNECTIONS 10    // Maximum simultaneous connections

## Known Limitations
- The server only supports HTTP/1.1.
- No support for HTTPS or advanced HTTP features like POST or PUT.
- Limited error handling.

## Future Improvements
- Add support for HTTPS.
- Handle more HTTP methods (e.g., POST, DELETE).
- Implement multithreading instead of `fork()` for better performance.
- Add logging and request metrics.

