# Bare Metal Programming on QEMU MPS2-an505 from Scratch

## Introduction

NO CMSIS, NO SDK

## Usage

### Run with QEMU

```bash
$ make run DIR=TARGET_DIR
```

### Debug with GDB

Run the following commands in different console:

```bash
$ make debug DIR=TARGET_DIR
$ make gdb DIR=TARGET_DIR
```

## References

- Arm® Cortex®-M33 Devices Generic User Guide
- <https://github.com/qemu/qemu/blob/master/hw/arm/mps2-tz.c>
- <https://embeddedsecurity.io/sec-c>
- <https://github.com/Introduction-To-System-On-Chip/QEMU_an505>
- <https://github.com/jserv/mini-arm-os>
