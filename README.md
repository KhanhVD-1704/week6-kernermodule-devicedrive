# Linux Kernel Week 6 Labs — Loadable Module

Repository này tìm hiểu cách xây dựng **loadable kernel module** thành file
`.ko`, có thể nạp hoặc gỡ khi kernel đang chạy.

## Nội dung

- [Cơ sở lý thuyết](built-module/theory/README.md): kernel space, external
  loadable module, Kbuild, vòng đời, character device, ioctl, đồng bộ, quản lý
  tài nguyên và chẩn đoán lỗi.
- [Thực hành character device](built-module/practice/README.md): xây dựng, nạp,
  kiểm tra và gỡ một character-device module có hỗ trợ ioctl.

## Cấu trúc repository

```text
kernel-week6-labs/
├── built-module/
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

Nếu mới làm quen với kernel module, hãy bắt đầu từ
[phần lý thuyết](built-module/theory/README.md). Phần này độc lập với bài tập và
trình bày các cơ chế nền tảng.

Để build và chạy bài thực hành:

```bash
cd built-module/practice
make
make check
```

Quy trình chi tiết, kết quả mong đợi và cách xử lý lỗi nằm trong
[README thực hành](built-module/practice/README.md).

## Lưu ý an toàn

Kernel module chạy trong kernel space. Lỗi trong module có thể làm treo hệ
thống hoặc gây mất dữ liệu. Nên thực hành trong máy ảo hoặc môi trường có thể
phục hồi.
