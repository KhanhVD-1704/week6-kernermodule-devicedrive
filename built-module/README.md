# Loadable Kernel Module

Project này tập trung vào **loadable kernel module** (module có thể nạp và gỡ
khi kernel đang chạy). Nội dung được tách thành hai phần độc lập:

- [Cơ sở lý thuyết](theory/README.md): kiến thức chung về loadable module,
  Kbuild, vòng đời, dependency, symbol, tham số và cách chẩn đoán lỗi.
- [Thực hành character device](practice/README.md): một ứng dụng cụ thể của
  loadable module, kèm toàn bộ mã nguồn và hướng dẫn kiểm thử riêng.

Phần lý thuyết không diễn giải lại từng bước của bài character device. Người
đọc có thể học cơ chế module trước, sau đó dùng bài thực hành để quan sát các cơ
chế đó trong một driver chạy được.

## Cấu trúc

```text
built-module/
├── README.md
├── theory/
│   └── README.md             # toàn bộ cơ sở lý thuyết
└── practice/
    ├── README.md
    ├── Makefile
    ├── include/
    ├── src/
    └── test/
```

## Bắt đầu

Đọc [phần lý thuyết](theory/README.md), sau đó build bài thực hành:

```bash
cd practice
make
make check
make unload
```
