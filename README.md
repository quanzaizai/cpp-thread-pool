# ⚡ Modern C++17 泛型异步线程池 (cpp-thread-pool)

所属专业课：《操作系统》《多线程与高并发系统编程》

## 💡 为什么需要线程池？
在多线程并发场景下，频繁通过操作系统 API（如 `pthread_create`）创建和销毁线程会带来高昂的内核态上下文切换、内存页表分配和初始化开销。
本项目实现了**现代 C++17 工业级泛型异步线程池**：
* **任务队列解耦**：生产者线程通过完美转发提交任务，消费者（Worker 线程）自旋挂起与唤醒。
* **零开销异步结果获取**：深度结合 `std::future` 与 `std::packaged_task`，支持同步等待并提取任意类型返回值。
* **异常跨线程传递**：任务内部抛出的异常可安全经由 `future.get()` 在主线程中捕获。
* **优雅停机 (Graceful Shutdown)**：保证在析构或停机时，排队中的任务不会被意外丢弃。

## 🛠️ 构建与测试

```bash
# 使用 Makefile
make test  # 运行自动化单元测试
make run   # 运行综合功能演示

# 或使用 CMake
mkdir build && cd build
cmake ..
cmake --build .
./test_thread_pool
./thread_pool_demo
```
