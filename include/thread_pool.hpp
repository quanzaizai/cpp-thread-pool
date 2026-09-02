#pragma once

/**
 * ==============================================================================
 * 💡【知识点】现代 C++17 工业级泛型异步线程池 (Modern C++ Thread Pool)
 * 所属专业课：《操作系统》《多线程与高并发系统编程》
 * 
 * 🎓【核心考点与体系结构理论】：
 * 1. 为什么需要线程池（Thread Pool）？
 *    - 频繁创建/销毁 OS 线程开销极大（包含内核栈分配、页表建立、上下文切换）。
 *    - 线程池通过“预先创建一组工作线程（Worker Threads）+ 一个线程安全的任务队列（Task Queue）”，
 *      实现线程资源的复用，将系统开销从 O(N) 降低到 O(1)。
 * 
 * 2. 核心并发原语：
 *    - std::mutex & std::unique_lock: 保护任务队列在多线程并发读写下的原子性与临界区安全。
 *    - std::condition_variable: 实现“生产者-消费者”模式下的高效休眠与唤醒，避免 CPU 忙等待（Busy-polling）。
 *    - std::future & std::packaged_task: 实现“异步计算结果获取（Asynchronous Result Retrieval）”。
 *    - std::forward / 可变参数模板 (Variadic Templates): 实现完美转发，接收任意签名可调用对象。
 * ==============================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>
#include <memory>
#include <utility>

namespace core {

class ThreadPool {
public:
    /**
     * @brief 构造函数：初始化指定数量的工作线程
     * @param thread_count 线程池中常驻工作线程数（默认为硬件并发核心数）
     */
    explicit ThreadPool(size_t thread_count = std::thread::hardware_concurrency())
        : stop_(false), active_tasks_(0) {
        
        if (thread_count == 0) {
            thread_count = 1;
        }

        workers_.reserve(thread_count);
        for (size_t i = 0; i < thread_count; ++i) {
            workers_.emplace_back([this, i]() {
                this->worker_loop(i);
            });
        }
    }

    /**
     * @brief 析构函数：优雅关闭线程池，等待所有正在执行的任务完成
     */
    ~ThreadPool() {
        shutdown();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /**
     * @brief 提交任意可调用任务（函数、Lambda、成员函数），返回代表异步结果的 std::future
     */
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) 
        -> std::future<typename std::invoke_result<F, Args...>::type> {
        
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            
            if (stop_.load()) {
                throw std::runtime_error("❌ [ThreadPool Error] 线程池已关闭，无法提交新任务！");
            }

            tasks_.emplace([task]() {
                (*task)();
            });
        }

        cv_.notify_one();
        return res;
    }

    /**
     * @brief 优雅关闭线程池（等待已排队任务全部执行完毕）
     */
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (stop_.load()) {
                return;
            }
            stop_.store(true);
        }

        cv_.notify_all();

        for (std::thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    size_t get_thread_count() const {
        return workers_.size();
    }

    size_t get_queue_size() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }

private:
    void worker_loop(size_t worker_id) {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(this->queue_mutex_);

                this->cv_.wait(lock, [this]() {
                    return this->stop_.load() || !this->tasks_.empty();
                });

                if (this->stop_.load() && this->tasks_.empty()) {
                    return;
                }

                task = std::move(this->tasks_.front());
                this->tasks_.pop();
                active_tasks_++;
            }

            try {
                task();
            } catch (const std::exception& e) {
                std::cerr << "⚠️ [Worker " << worker_id << " 异常]: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "⚠️ [Worker " << worker_id << " 未知异常]" << std::endl;
            }

            active_tasks_--;
        }
    }

private:
    std::vector<std::thread>          workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex                        queue_mutex_;
    std::condition_variable           cv_;
    std::atomic<bool>                 stop_;
    std::atomic<size_t>               active_tasks_;
};

} // namespace core
