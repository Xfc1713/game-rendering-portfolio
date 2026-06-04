#pragma once

#include <memory>
#include <string>

// RAII 与智能指针示例（渲染引擎资源管理的缩影）

class GpuBufferHandle {
public:
    explicit GpuBufferHandle(std::string name, std::size_t byteSize);
    ~GpuBufferHandle();

    GpuBufferHandle(const GpuBufferHandle&) = delete;
    GpuBufferHandle& operator=(const GpuBufferHandle&) = delete;
    GpuBufferHandle(GpuBufferHandle&& other) noexcept;
    GpuBufferHandle& operator=(GpuBufferHandle&& other) noexcept;

    const std::string& name() const { return name_; }
    std::size_t byteSize() const { return byteSize_; }

private:
    std::string name_;
    std::size_t byteSize_ = 0;
    bool valid_ = false;

    void release();
};

void run_raii_demo();
