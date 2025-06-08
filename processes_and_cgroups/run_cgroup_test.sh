#!/bin/bash
set -e

UNIT_NAME="testgroup"
MEMORY_LIMIT="1M"

echo "[INFO] Compiling memory_hog.cpp..."
g++ memory_hog.cpp -o memory_hog_cpp

echo "[INFO] Disabling swap..."
sudo swapoff -a

function cleanup {
    echo "[INFO] Re-enabling swap..."
    sudo swapon -a
}
trap cleanup EXIT

CGROUP_DIR="/sys/fs/cgroup/$UNIT_NAME"

echo "[INFO] Creating cgroup at $CGROUP_DIR..."
sudo mkdir -p "$CGROUP_DIR"

echo "[INFO] Setting memory limit to $MEMORY_LIMIT"
echo $MEMORY_LIMIT | sudo tee "$CGROUP_DIR/memory.max" > /dev/null
echo 0 | sudo tee "$CGROUP_DIR/memory.swap.max" > /dev/null

echo "[INFO] Launching memory_hog_cpp inside $UNIT_NAME cgroup..."

# Run process and catch exit code safely, hide the shell's "Killed" message
set +e
sudo bash -c "
  echo \$\$ > $CGROUP_DIR/cgroup.procs
  exec ./memory_hog_cpp
"
EXIT_STATUS=$?
set -e

if [ $EXIT_STATUS -eq 0 ]; then
  echo "[INFO] memory_hog_cpp exited normally."
else
  if [ $EXIT_STATUS -gt 128 ]; then
    SIGNAL=$((EXIT_STATUS - 128))
    echo "[WARN] memory_hog_cpp was killed by signal $SIGNAL."
  else
    echo "[WARN] memory_hog_cpp exited with code $EXIT_STATUS."
  fi
fi

# Give kernel some time to update cgroup stats
sleep 1

echo "[INFO] Cgroup memory events:"
if [ -f "$CGROUP_DIR/memory.events" ]; then
  sudo cat "$CGROUP_DIR/memory.events"
else
  echo "[WARN] Cgroup memory.events file not found."
fi


# Move all tasks out of the cgroup
sudo bash -c "echo 0 > /sys/fs/cgroup/testgroup/cgroup.procs"

# Then remove the cgroup directory
sudo rmdir /sys/fs/cgroup/testgroup
