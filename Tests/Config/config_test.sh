#!/bin/bash

# Test runner for config parser
BINARY="/home/mel-houd/workspace/webserver_fork/webserv"  # Change this to your actual binary name

echo "Starting Config Parser Tests..."
echo "========================================"

# Helper function to run a test
run_test() {
    local test_name=$1
    local config_file="./$test_name.conf"
    
    # Create test config
    cat > "$config_file" <<EOF
$2
EOF

    echo "Running server with config: $config_file"
    echo "----------------------------------------"
    echo "Config content:"
    cat "$config_file"
    echo "----------------------------------------"

    # Run the server with the config
    $BINARY "$config_file" &
    SERVER_PID=$!

    echo "Server started with PID: $SERVER_PID"
    echo "Press Ctrl+C to stop the server and proceed to the next test..."
    echo "----------------------------------------"

    # Wait for user input to stop the server
    wait $SERVER_PID
    kill $SERVER_PID 2>/dev/null  # Ensure the server is stopped
    echo "Server stopped."
    echo "========================================"
}

# Valid configurations
run_test "no global params" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    
    Location / {
        Methods GET
        Index index.html
    }
}"

# Invalid configurations (for manual verification)
run_test "invalid_missing_brace" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    # Missing closing brace"

run_test "invalid_unknown_directive" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    UnknownDirective value
    
    Location / {
        Methods GET
    }
}"

run_test "invalid_listen_format" \
"Server {
    Listen not_a_number
    Host localhost
    ServerName test
    
    Location / {
        Methods GET
    }
}"

run_test "invalid_duplicate_server" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    
    Location / {
        Methods GET
    }
}

Server {
    Listen  8080
    Host localhost
    ServerName test
    
    Location / {
        Methods GET
    }
}"

run_test "invalid_empty_server" \
"Server {
}"

run_test "invalid_methods" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    
    Location / {
        Methods INVALID_METHOD
    }
}"

run_test "invalid_nesting" \
"Server {
    Listen 8080
    Host localhost
    ServerName test
    
    Location / {
        Methods GET
        Server {
            Listen 8081
        }
    }
}"

echo "All tests completed. Config files are located in the current directory."
