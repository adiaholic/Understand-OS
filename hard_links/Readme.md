# 📜 **Understanding System Calls for ln, rm, cat**

---

# 🔥 Purpose

This experiment traces and documents **which Linux system calls** are invoked when executing simple file operations:

* **Creating a hard link** (`ln file1.txt file1_hardlink.txt`)
* **Deleting a hard link** (`rm file1_hardlink.txt`)
* **Reading a file** (`cat file1.txt`)

The goal is to understand **how shell commands map to low-level kernel system calls**.

All system behavior was traced using **strace** with `-f -e trace=file` options.

---

# 🛠️ Flow of Events and System Call Analysis

---

## 1. **Creating a Hard Link (`ln`)**

**Command:**

```bash
ln file1.txt file1_hardlink.txt
```

**System Calls Observed:**

|                       Syscall                      | Meaning                                                  |
| :------------------------------------------------: | :------------------------------------------------------- |
|           `execve("/usr/bin/ln", [...])`           | Starts the `ln` executable                               |
|                     `faccessat`                    | Checks access permissions for preload libraries (normal) |
|       `openat("/etc/ld.so.cache", O_RDONLY)`       | Loads library cache                                      |
|                    `newfstatat`                    | Reads metadata of library files                          |
|      `openat("/lib/.../libc.so.6", O_RDONLY)`      | Loads libc dynamically                                   |
|                    `newfstatat`                    | Reads metadata of libc                                   |
| **`linkat("file1.txt", "file1_hardlink.txt", 0)`** | **Creates a hard link (key operation)**                  |
|                       `exit`                       | `ln` exits successfully                                  |

**Main filesystem effect**:

* **`linkat()`** creates a new directory entry `file1_hardlink.txt` pointing to the **same inode** as `file1.txt`.
* No data copying occurs.

---

## 2. **Deleting the Hard Link (`rm`)**

**Command:**

```bash
rm file1_hardlink.txt
```

**System Calls Observed:**

|                  Syscall                 | Meaning                                         |
| :--------------------------------------: | :---------------------------------------------- |
|      `execve("/usr/bin/rm", [...])`      | Starts the `rm` executable                      |
|                `faccessat`               | Checks preload libraries                        |
|  `openat("/etc/ld.so.cache", O_RDONLY)`  | Loads library cache                             |
|               `newfstatat`               | Reads library metadata                          |
| `openat("/lib/.../libc.so.6", O_RDONLY)` | Loads libc dynamically                          |
|               `newfstatat`               | Reads metadata of libc                          |
|  `newfstatat("file1_hardlink.txt", ...)` | Checks metadata of target file                  |
|  **`unlinkat("file1_hardlink.txt", 0)`** | **Removes the directory entry (key operation)** |
|                  `exit`                  | `rm` exits successfully                         |

**Main filesystem effect**:

* **`unlinkat()`** removes the `file1_hardlink.txt` **directory entry**.
* If no other links point to the inode, the inode and data would be deleted.
* However, in this case, the original `file1.txt` still exists.

---

## 3. **Reading the Original File (`cat`)**

**Command:**

```bash
cat file1.txt
```

**System Calls Observed:**

|                  Syscall                 | Meaning                                        |
| :--------------------------------------: | :--------------------------------------------- |
|      `execve("/usr/bin/cat", [...])`     | Starts the `cat` executable                    |
|                `faccessat`               | Checks access permissions                      |
|  `openat("/etc/ld.so.cache", O_RDONLY)`  | Loads library cache                            |
|               `newfstatat`               | Reads metadata of library                      |
| `openat("/lib/.../libc.so.6", O_RDONLY)` | Loads libc                                     |
|               `newfstatat`               | Reads libc metadata                            |
|      `openat("file1.txt", O_RDONLY)`     | **Opens the file for reading (key operation)** |
|               `newfstatat`               | Reads metadata of the opened file              |
|                  `exit`                  | `cat` exits successfully                       |

**Main filesystem effect**:

* **`openat()`** opens the file descriptor for `file1.txt`.
* `cat` reads and outputs the file content to stdout.

---

# 🧠 Key Takeaways

|              Command              | Critical Syscall |               Action               |
| :-------------------------------: | :--------------: | :--------------------------------: |
| `ln file1.txt file1_hardlink.txt` |     `linkat`     |          Create hard link          |
|      `rm file1_hardlink.txt`      |    `unlinkat`    | Remove hard link (directory entry) |
|          `cat file1.txt`          |     `openat`     |        Open file for reading       |

All other syscalls (`execve`, `openat`, `newfstatat`, etc.) during setup are **dynamic linking overhead**.
Only `linkat`, `unlinkat`, and `openat` are **actual filesystem operations** in these experiments.

---
