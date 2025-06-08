# Understanding Processes and Control Groups (cgroups) in Linux

## Exploring Process Limits

To determine the maximum number of process IDs (PIDs) your system can handle, run:

```
cat /proc/sys/kernel/pid_max
```

On my Raspberry Pi running 64-bit Ubuntu, this command returns:

```
4194304
```
This value, 2^22, is set by Linux kernel developers to suit modern 64-bit systems. On a 32-bit machine, the typical value would be `32767`. As processes are created and terminated, the kernel recycles PIDs over time.

## Inspecting a Process

To view the details of a specific process (for example, with PID 21188 for VS Code), use:

```
ls /proc/21188
```

This lists the files and directories associated with that process, such as:
```
attr             cwd      loginuid    oom_adj        sessionid     task
autogroup        environ  map_files   oom_score      setgroups     timens_offsets
auxv             exe      maps        oom_score_adj  smaps         timers
cgroup           fd       mem         pagemap        smaps_rollup  timerslack_ns
clear_refs       fdinfo   mountinfo   personality    stack         uid_map
cmdline          gid_map  mounts      projid_map     stat          wchan
comm             io       mountstats  root           statm
coredump_filter  latency  net         sched          status
cpuset           limits   ns          schedstat      syscall
```
## Investigating cgroups

To see the cgroup of a process, run:

```
cat /proc/21188/cgroup
```

I am running VSCode and it's PID is 21188. The output for me looks like:

0::/user.slice/user-1002.slice/user@1002.service/app.slice/app-gnome-code-18994.scope

This string represents the cgroup hierarchy for the process. Cgroups (control groups) are organized in a directory-like structure, allowing the kernel to apply resource policies at various levels—such as for all users, a specific user, a session, or an individual application.

In this example, the hierarchy is:
- `user.slice`: Top-level group for user processes
- `user-1002.slice`: For the user with UID 1002
- `user@1002.service`: For the user's session
- `app.slice`: For user applications
- `app-gnome-code-18994.scope`: For the VS Code application instance

The generic format of a cgroup string is:

    hierarchy-ID:subsystems:cgroup-path

Here:
- `hierarchy-ID` is 0
- `subsystems` is empty (hence two consecutive colons)
- `cgroup-path` is `/user.slice/user-1002.slice/user@1002.service/app.slice/app-gnome-code-18994.scope`

`user.slice` is a systemd-created top-level group that organizes all user-initiated processes, enabling administrators to apply resource limits or policies collectively to user processes, separate from system services. This structure is defined by systemd for efficient resource management and process organization.

## Applying Resource Limits with systemd

To set resource limits for all user processes in `user.slice`, you can use the `systemctl` command or write directly to the relevant cgroup files.

For example, to limit memory usage for all user processes to 2GB:

```
sudo systemctl set-property user.slice MemoryMax=2G
```

For more details on systemd resource control properties, refer to:
https://www.freedesktop.org/software/systemd/man/latest/systemd.resource-control.html

Or if you are a nerd like me check the manual pages:

```
man 5 systemd.resource-control
```

## Useful Tools for Exploring cgroups

- `systemd-cgls`: Provides a tree-like view of the cgroup hierarchy
  https://www.freedesktop.org/software/systemd/man/latest/systemd-cgls.html
- `systemd-cgtop`: Offers an activity monitor view of cgroups
  https://www.freedesktop.org/software/systemd/man/latest/systemd-cgtop.html

# Cgroup Memory Limit Experiment

This experiment demonstrates how Linux cgroups can restrict process memory usage.

**Steps:**
1. Create a cgroup with a 1MB memory limit.
2. Run a Python program that tries to allocate ~5MB.
3. Observe the process failure (MemoryError or OOM kill).
4. Display cgroup memory events.

Run the experiment with:

`./run_cgroup_test.sh`, you will likely see output like this:

```
[INFO] Compiling memory_hog.cpp...
[INFO] Disabling swap...
[INFO] Creating cgroup at /sys/fs/cgroup/testgroup...
[INFO] Setting memory limit to 1M
[INFO] Launching memory_hog_cpp inside testgroup cgroup...
./run_cgroup_test.sh: line 35: 16046 Killed                  sudo bash -c "
  echo \$\$ > $CGROUP_DIR/cgroup.procs
  exec ./memory_hog_cpp
"
[WARN] memory_hog_cpp was killed by signal 9.
[INFO] Cgroup memory events:
low 0
high 0
max 35
oom 1
oom_kill 1
oom_group_kill 0
[INFO] Re-enabling swap...
```

This demonstrates that the process was killed by the kernel (signal 9) due to exceeding the cgroup's memory limit, and the cgroup memory events confirm an OOM (out-of-memory) kill occurred.

