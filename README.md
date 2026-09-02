# ⚡ Modern C++17 泛型异步线程池 (cpp-thread-pool)

所属专业课：《操作系统》《现代 C++ 并发编程》

---

## 📖 工程目录结构解析

```text
cpp-thread-pool/
├── include/       # 📌【头文件目录】：Header-only 泛型可变参数模板线程池实现
├── src/           # 🔨【源码目录】：演示程序主入口
├── tests/         # 🧪【单元测试】：高并发多任务调度、返回值异步获取与析构安全测试
├── build/         # 📦【编译产物】：编译生成的可执行二进制文件
├── Makefile       # ⚙️【一键编译脚本】：自动化编译指令
└── README.md      # 📘【项目文档】：并发编程核心机制剖析
```

---

## 🗂️ 本项目所有文件详细功能与角色速查

| 所在目录 | 文件名 | 承担功能与底层作用 |
| :--- | :--- | :--- |
| `include/` | [`thread_pool.hpp`](./include/thread_pool.hpp) | **核心线程池类 (ThreadPool)**：Header-only 库，基于 C++17 可变参数模板，支持任意函数/参数类型提交，返回 `std::future<ReturnType>` |
| `src/`     | [`main.cpp`](./src/main.cpp) | **实战示例程序**：演示多线程并发计算斐波那契数、批量数据处理与异步非阻塞取值 |
| `tests/`   | [`test_thread_pool.cpp`](./tests/test_thread_pool.cpp) | **自动化压力测试**：验证 100 个并发任务无死锁、无任务遗漏、优雅停止 (`stop`) |

---

## 🛠️ 构建与测试运行

```bash
make test  # 运行全套自动化并发测试
make run   # 运行多任务异步计算演示
```
