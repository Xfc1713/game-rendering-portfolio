#include "raii_demo.h"

#include <iostream>
#include <utility>

GpuBufferHandle::GpuBufferHandle(std::string name, std::size_t byteSize)
    : name_(std::move(name)), byteSize_(byteSize), valid_(true) {
    std::cout << "[RAII] 分配 GPU 缓冲: " << name_ << " (" << byteSize_ << " bytes)\n";
}

GpuBufferHandle::~GpuBufferHandle() {
    release();
}

GpuBufferHandle::GpuBufferHandle(GpuBufferHandle&& other) noexcept
    : name_(std::move(other.name_)),
      byteSize_(other.byteSize_),
      valid_(other.valid_) {
    other.valid_ = false;
    other.byteSize_ = 0;
}

GpuBufferHandle& GpuBufferHandle::operator=(GpuBufferHandle&& other) noexcept {
    if (this != &other) {
        release();
        name_ = std::move(other.name_);
        byteSize_ = other.byteSize_;
        valid_ = other.valid_;
        other.valid_ = false;
        other.byteSize_ = 0;
    }
    return *this;
}

void GpuBufferHandle::release() {
    if (valid_) {
        std::cout << "[RAII] 释放 GPU 缓冲: " << name_ << "\n";
        valid_ = false;
        byteSize_ = 0;
    }
}

void run_raii_demo() {
    auto vertexBuffer = std::make_unique<GpuBufferHandle>("VertexBuffer", 1024 * 64);
    GpuBufferHandle indexBuffer("IndexBuffer", 4096);

    {
        GpuBufferHandle moved = std::move(indexBuffer);
        std::cout << "[RAII] 移动后索引缓冲仍有效: " << moved.name() << "\n";
    }

    std::cout << "[RAII] shared_ptr 示例 — 多引用同一逻辑资源\n";
    std::shared_ptr<GpuBufferHandle> shared =
        std::make_shared<GpuBufferHandle>("UniformBlock", 256);
    auto alias = shared;
    std::cout << "[RAII] 引用计数 ≈ " << shared.use_count() << "\n";
}
