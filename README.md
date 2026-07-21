# Linux Kernel Week 6 Labs

Repository này tìm hiểu hai cách đưa một chức năng vào Linux kernel:

- **Loadable kernel module**: build thành file `.ko`, có thể nạp hoặc gỡ khi
  kernel đang chạy.
- **Built-in kernel component**: liên kết trực tiếp vào kernel image và có mặt
  từ lúc hệ thống boot.

## Nội dung

| Phần | Trạng thái | Nội dung |
| --- | --- | --- |
| [Built module](built-module/README.md) | Đã có | Lý thuyết về loadable kernel module và bài thực hành |
| Built-in | Chưa triển khai | Tích hợp source vào kernel tree bằng Kconfig và Kbuild |

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
│       ├── src/
│       └── test/
├── built-in/                    # bổ sung sau
├── .clang-format
├── .gitignore
└── README.md
```

Thư mục `build/` trong mỗi phần thực hành chỉ chứa artifact được sinh tự động
và không được đưa vào Git.

## Yêu cầu môi trường

- Linux hỗ trợ loadable kernel module.
- GCC và GNU Make.
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

Đọc [tổng quan](built-module/README.md), sau đó chọn phần lý thuyết hoặc thực
hành theo hướng dẫn trong thư mục này.

## Lưu ý an toàn

Kernel module chạy trong kernel space. Lỗi trong module có thể làm treo hệ
thống hoặc gây mất dữ liệu. Nên thực hành trong máy ảo hoặc môi trường có thể
phục hồi.
