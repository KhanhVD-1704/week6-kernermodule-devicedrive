# Linux Kernel Week 6 Labs — Loadable Module

Repository này tìm hiểu cách xây dựng **loadable kernel module** thành file
`.ko`, có thể nạp hoặc gỡ khi kernel đang chạy.

## Nội dung

- [Cơ sở lý thuyết](built-module/theory/README.md): khái niệm, Kbuild, vòng đời,
  dependency, symbol, bảo mật và cách chẩn đoán lỗi kernel module.
- [Thực hành character device](built-module/practice/README.md): xây dựng,
  nạp, kiểm thử và gỡ một character-device module có hỗ trợ ioctl.

## Cấu trúc repository

```text
kernel-week6-labs/
├── built-module/
│   ├── README.md
│   ├── theory/
│   │   └── README.md
│   └── practice/
│       ├── README.md
│       ├── Makefile
│       ├── include/
│       │   └── char_buffer_ioctl.h
│       ├── src/
│       │   ├── Kbuild
│       │   └── char_device_module.c
│       └── test/
│           └── char_buffer_ctl.c
├── .clang-format
├── .gitignore
└── README.md
```

Thư mục `build/` trong mỗi phần thực hành chỉ chứa artifact được sinh tự động
và không được đưa vào Git.

## Yêu cầu môi trường

- Linux hỗ trợ loadable kernel module.
- GCC, GNU Make và udev.
- Kernel headers phù hợp với kernel đang chạy.
- Quyền quản trị để nạp và gỡ module.

Trên Ubuntu hoặc Debian:

```bash
sudo apt install build-essential linux-headers-$(uname -r)
```

Kiểm tra kernel headers:

```bash
test -d /lib/modules/$(uname -r)/build && echo "kernel headers: OK"
```

## Bắt đầu

```bash
cd built-module
```

Đọc [tổng quan](built-module/README.md), sau đó đọc phần lý thuyết hoặc chuyển
vào `practice/` để build và kiểm thử module.

## Lưu ý an toàn

Kernel module chạy trong kernel space. Lỗi trong module có thể làm treo hệ
thống hoặc gây mất dữ liệu. Nên thực hành trong máy ảo hoặc môi trường có thể
phục hồi.
