#!/bin/bash

# Function to send curl request and check for non-200 response
send_request() {
    local response=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/)
    if [ "$response" -ne 200 ]; then
        echo "Non-200 response detected: HTTP $response"
        exit 1  
        # Exit the script if non-200 response is encountered
    fi
}

# Loop to send multiple concurrent requests
count=0
while true; do
    for ((i=1; i<=500; i++)); do
        send_request &  # Send request in background to make it concurrent
        ((count++))
        echo "Sent request $count"
    done

    # Wait for all background processes to finish
    wait

    # Optionally, introduce a delay before sending the next batch of requests
    # Adjust the sleep time (in seconds) based on your test requirements
    sleep 1
done
