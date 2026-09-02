#include "thread_pool.hpp"
#include <iostream>
#include <chrono>
#include <numeric>

// 示例 1：计算密集型任务（累加和）
long long compute_sum(int start, int end) {
    long long total = 0;
    for (int i = start; i <= end; ++i) {
        total += i;
    }
    return total;
}

// 示例 2：模拟 I/O 耗时任务
void simulated_io_task(int task_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "  [I/O 任务 " << task_id << "] 完成，执行线程 ID: " 
              << std::this_thread::get_id() << std::endl;
}

int main() {
    std::cout << "============================================================\n";
    std::cout << "🚀 Modern C++17 泛型异步线程池 (ThreadPool) 演示程序\n";
    std::cout << "============================================================\n";

    // 1. 创建包含 4 个工作线程的线程池
    core::ThreadPool pool(4);
    std::cout << "✅ 线程池初始化成功，常驻工作线程数: " << pool.get_thread_count() << "\n\n";

    // 2. 演示多任务异步提交与 std::future 返回值获取
    std::cout << "[场景 1] 异步并发分块计算 1 到 1000000 的总和...\n";
    auto f1 = pool.submit(compute_sum, 1, 250000);
    auto f2 = pool.submit(compute_sum, 250001, 500000);
    auto f3 = pool.submit(compute_sum, 500001, 750000);
    auto f4 = pool.submit(compute_sum, 750001, 1000000);

    // 阻塞等待异步结果返回并汇总
    long long total_sum = f1.get() + f2.get() + f3.get() + f4.get();
    std::cout << "  -> 4 个线程分块并发计算完毕，总结果 = " << total_sum << "\n\n";

    // 3. 演示 Lambda 表达式与自定义无返回值任务
    std::cout << "[场景 2] 并发提交 8 个模拟 I/O 任务...\n";
    for (int i = 1; i <= 8; ++i) {
        pool.submit(simulated_io_task, i);
    }

    // 4. 演示支持泛型 Lambda 返回字符串
    auto f_str = pool.submit([](const std::string& name) {
        return "Hello, " + name + "! (From ThreadPool Worker)";
    }, "Quanzai");

    std::cout << "\n[场景 3] Lambda 异步结果: " << f_str.get() << "\n";

    std::cout << "\n正在优雅关闭线程池...\n";
    pool.shutdown();
    std::cout << "🎉 线程池已完全停机，所有任务安全退出！\n";
    return 0;
}
