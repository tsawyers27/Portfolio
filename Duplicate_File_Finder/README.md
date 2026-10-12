# Duplicate File Finder

A Python command-line utility that recursively scans a directory, identifies files with matching contents, and reports duplicate groups alongside an estimate of the disk space that could be reclaimed by retaining one copy from each group.

This project was built to practice filesystem operations, hashing, data grouping, exception handling, and designing a complete small utility independently.

## Features

- Recursively scans a user-selected directory and its subdirectories.
- Computes a SHA-256 digest for each readable file, regardless of filename or extension.
- Groups files with matching hashes and displays their names, paths, and sizes in formatted tables.
- Estimates reclaimable disk space in bytes, assuming one file is kept from each duplicate group.
- Prints a message when access to a file or directory is denied. Files that cannot be hashed due to a permission error are omitted from the results.
- **Read-only:** never deletes or changes the files it scans.

## Requirements

- **Python 3.11 or newer** (`hashlib.file_digest` is used)
- Third-party packages:
  - `pandas`
  - `tabulate`

The other imported modules (`os`, `pathlib`, and `hashlib`) are included in Python's standard library.

## Installation

Clone the repository or download the script, then install the dependencies:

```bash
python -m pip install pandas tabulate
```

## Usage

From a terminal, run the script:

```bash
python dupe_finder.py
```

When prompted, enter the directory you want to scan:

```text
Please enter a directory to search: /path/to/directory
```

The program will print each duplicate group and the estimated reclaimable space. For example, a scan containing three identical 32-byte files and two identical 11-byte files would yield two duplicate groups and:

```text
Reclaimable Disk Space: 75 Bytes
```

The script accepts a path through the interactive prompt; it does not currently provide command-line flags.

## How It Works

1. Walk through the specified directory tree using `os.scandir()`.
2. Open each file in binary mode and calculate its SHA-256 digest.
3. Collect each successfully hashed file's name, path, size, and hash.
4. Use `pandas` to select hashes associated with more than one file and group those files together.
5. Print each group using `tabulate`.
6. Sum the sizes of the extra copies in each group to estimate reclaimable space.

**Reclaimable space** is an estimate, not space actually freed: the application does not delete files. Matching SHA-256 hashes are treated as evidence of duplicate content; the program does not do a byte-for-byte confirmation.

## Current Limitations

- Files are fully read to compute hashes; there is no preliminary size-based filtering or hashing cache.
- Permission errors are handled, but other filesystem errors (such as missing paths or files disappearing during a scan) may stop execution.
- Scanning a directory with no successfully processed files may cause an error rather than producing an empty report.
- Directory links and unusual filesystem structures are not specially handled.
- Results are printed to the terminal; there is no file export or automatic cleanup.

## Project Scope

This is a deliberately small, standalone portfolio project focused on core Python and software-engineering fundamentals, rather than a production-ready disk-cleanup tool.
