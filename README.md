# ⚡ Modern C++17 泛型异步线程池 (cpp-thread-pool)

所属专业课：《操作系统》《现代 C++ 并发编程》

> 🌟 **【零基础自学必读】**：想知道为什么不能无限开线程？Header-only 模板为什么不能写在 `.cpp` 里？`-pthread` 编译器底层做了什么？生产者-消费者模型如何零 CPU 占用休眠？请先阅读保姆级设计手册：  
> 👉 **[📘 零基础工程架构与并发内核全景指南 (ARCHITECTURE_AND_DESIGN.md)](./ARCHITECTURE_AND_DESIGN.md)**

---

## 📖 工程目录结构

```text
cpp-thread-pool/
├── ARCHITECTURE_AND_DESIGN.md  # 🌟 零基础并发原理与手写复现指南
├── include/                    # Header-only 泛型线程池核心类 (thread_pool.hpp)
├── src/                        # 演示主入口 (main.cpp)
├── tests/                      # 高并发压力与异步返回测试
├── Makefile                    # 一键编译脚本
├── CMakeLists.txt              # 跨平台构建工程
└── README.md                   # 项目说明文档
```

---

## 🗂️ 各模块与文件功能详解

### 1. `include/` (核心线程池)
* ⚡ [`thread_pool.hpp`](./include/thread_pool.hpp)
  * **泛型线程池 (ThreadPool)**：基于 C++17 可变参数模板与 `std::future`，支持任意类型函数与参数的提交，异步返回执行结果。

### 2. `src/` (实战演示)
* 💻 [`main.cpp`](./src/main.cpp)
  * **多任务并发演示**：演示多线程异步计算斐波那契数、批量数据处理与非阻塞获取返回值。

### 3. `tests/` (自动化测试)
* 🧪 [`test_thread_pool.cpp`](./tests/test_thread_pool.cpp)
  * **并发压力测试**：验证 100 个并发任务无死锁、无任务丢失、优雅停止 (`stop`)。

---

## 🛠️ 构建与测试运行

```bash
make test  # 运行全套并发测试
make run   # 运行多任务计算演示
```
