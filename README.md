# Columnar Storage Engine (v2)

A high-performance, kernel-bypassing data ingestion engine written in pure C using memory-mapped files and columnar (SoA) storage.

## Overview

This project is an experimental storage engine designed for fast, predictable data writes. Instead of using a traditional row-based (Array of Structures) layout, it uses a **Structure of Arrays (SoA)** approach — storing each field (name, phone, address, id) in its own separate binary file.

The goal is to achieve direct, cache-friendly access using simple pointer arithmetic and fixed strides, while completely bypassing standard `read()`/`write()` syscalls via `mmap`.

## Key Architectural Decisions

- **Zero-copy I/O**: Files are mapped directly into process memory using `mmap` with `MAP_SHARED`.
- **Columnar Storage (SoA)**: Each data field lives in its own file. This improves cache locality when scanning or operating on specific fields.
- **Fixed-width records**: Every user occupies a predictable byte offset (`user_id * stride`) in each file.
- **Persistent ID counter**: A dedicated `id.bin` file tracks the next available user ID across program restarts.
- **Manual memory management**: Full control over padding, alignment, and file growth.

## Project Structure
systems_programming/create_account_v2
├── names.bin      → Fixed 64-byte stride per user (name)
├── phone.bin      → Fixed 16-byte stride per user (phone)
├── address.bin    → Fixed 128-byte stride per user (address)
├── id.bin         → Stores the next sequential user ID (8 bytes)
└── create_accounr.c 


## Current Capabilities

- Creates and resizes four binary files on first run using `ftruncate`
- Memory-maps all files for direct access
- Reads user input and writes it to the correct offset in each column file
- Increments and persists the global user ID
- Pads unused space in each field with null bytes (`\0`)

## Building and Running

```bash
gcc -o create_account.c -O2
```
The program will prompt for name, phone number, and address, then store them in the next available slot.Inspecting the DataSince the storage is raw binary, you can inspect it with standard tools:

# View first few names
```
hexdump -C names.bin | head -n 20
```
# Check current next user ID
```
od -An -ti8 id.bin
```
Known Limitations & Next Steps 
- Currently limited to 256 users (hardcoded)
- No bounds checking — writing beyond 256 will cause memory corruption
- No concurrent access protection
- ID allocation is not yet atomic
- Reading a specific user requires loading entire columns (future optimization planned)
- File growth logic (auto-expanding when full) is not yet implemented

Lessons LearnedThis version was built without any AI-generated code. The biggest challenges were:Correct pointer arithmetic across multiple mapped regions
Managing boilerplate for open(), ftruncate(), mmap(), and munmap() on every file
Understanding how to safely pad fixed-width fields

Future iterations will focus on dynamic resizing, proper error handling, user lookup by ID, and password protection.License
MIT License

---



