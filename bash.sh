#!/bin/bash

mkdir -p responses
# Number of simultaneous connections to simulate
num_connections=500

# URL of the server to test
server_url="http://localhost:80"

# Function to make a single HTTP request using curl
make_request() {
    response_file="responses/response_$1.txt"
    curl -s -o "$response_file" "$2" &
}

# Loop to make multiple simultaneous HTTP requests
for ((i = 1; i <= num_connections; i++)); do
    make_request "$i" "$server_url"
done

# Wait for all requests to complete
wait

echo "All requests completed."
