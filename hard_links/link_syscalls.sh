#!/bin/bash
set -e

BASE_DIR="/tmp/link_experiment"
DATA_DIR="$BASE_DIR/data"
OUTPUT_DIR="/experiment/output"
STRACE_LOG="$OUTPUT_DIR/strace_output.txt"

echo "Setting up environment..."

# Clean previous experiment
rm -rf "$BASE_DIR"
rm -f "$OUTPUT_DIR"/*

mkdir -p "$DATA_DIR"
mkdir -p "$OUTPUT_DIR"
cd "$DATA_DIR"

# Create initial file
echo "Hello World" > file1.txt

echo "Initial files created:"
ls -li

echo
echo "Starting focused strace logging (hardlink creation and destruction)..."
echo

# Helper function to run a command and log strace output
run_with_strace() {
  local description="$1"
  shift
  local cmd="$*"

  echo "========= $description =========" >> "$STRACE_LOG"
  echo "\$ $cmd" >> "$STRACE_LOG"
  echo >> "$STRACE_LOG"

  strace -f -e trace=file -o /tmp/temp_strace "$@"
  cat /tmp/temp_strace >> "$STRACE_LOG"
  echo -e "\n\n" >> "$STRACE_LOG"
}

# Start clean strace output
: > "$STRACE_LOG"

# Focused operations:
run_with_strace "CREATE HARD LINK" ln file1.txt file1_hardlink.txt
run_with_strace "REMOVE HARD LINK" rm file1_hardlink.txt
run_with_strace "CAT ORIGINAL FILE" cat file1.txt

# Cleanup temp file
rm /tmp/temp_strace

echo
echo "Strace logging completed."
echo "Strace output saved to: $STRACE_LOG"

echo
echo "Final filesystem state:"
ls -li
