#include "thread_pool.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>

void test_basic_execution() {
    std::cout << "[测试 1] 基础任务执行与返回值 (std::future::get)... ";
    core::ThreadPool pool(2);
    auto f = pool.submit([](int a, int b) { return a * b; }, 6, 7);
    assert(f.get() == 42);
    std::cout << "✅ PASS\n";
}

void test_high_concurrency_stress() {
    std::cout << "[测试 2] 高并发压力测试 (1000 个任务无死锁与漏任务)... ";
    constexpr int TASK_COUNT = 1000;
    core::ThreadPool pool(8);
    std::atomic<int> counter{0};

    std::vector<std::future<void>> futures;
    futures.reserve(TASK_COUNT);

    for (int i = 0; i < TASK_COUNT; ++i) {
        futures.push_back(pool.submit([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    for (auto& fut : futures) {
        fut.get();
    }

    assert(counter.load() == TASK_COUNT);
    std::cout << "✅ PASS (成功完成 " << counter.load() << " 个并发任务)\n";
}

void test_exception_safety() {
    std::cout << "[测试 3] 异常安全传递测试 (Future 跨线程捕获异常)... ";
    core::ThreadPool pool(2);
    auto f = pool.submit([]() -> int {
        throw std::runtime_error("自定义测试异常");
    });

    bool caught = false;
    try {
        f.get();
    } catch (const std::runtime_error& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "✅ PASS\n";
}

int main() {
    std::cout << "============================================================\n";
    std::cout << "🧪 开始执行 C++ ThreadPool 自动化单元测试套件\n";
    std::cout << "============================================================\n";

    test_basic_execution();
    test_high_concurrency_stress();
    test_exception_safety();

    std::cout << "============================================================\n";
    std::cout << "🎉 全部 3 项并发与异常安全测试均已顺利通过！\n";
    std::cout << "============================================================\n";
    return 0;
}
