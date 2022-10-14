#pragma once
#ifndef _PUZZLE_TRANSIENTPTR_HPP
#define _PUZZLE_TRANSIENTPTR_HPP

#include <memory>
#include <utility>

namespace puzzle
{
    template<typename _T>
    class TransientPtr
    {
    private:
        using Self = puzzle::TransientPtr<_T>;

        _T ptr_;
    public:

        TransientPtr()
            :ptr_(nullptr)
        {}

        explicit TransientPtr(_T *ptr)
            :ptr_(ptr)
        {}

        TransientPtr(const Self &other) = default;

        TransientPtr(Self &&other) noexcept
            :ptr_(other.ptr_)
        {
            other.ptr_ = nullptr;
        }

        inline Self &operator=(const Self &other)
        {
            if(this != std::addressof(other))
            {
                Self tmp{other};
                std::swap(tmp,*this);
            }
            return *this;
        }

        inline Self &operator=(Self &&other) noexcept
        {
            if(this != std::addressof(other))
            {
                if(this->ptr_)
                {
                    delete this->ptr_;
                }
                this->ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        ~TransientPtr() noexcept
        {
            if(this->ptr_)
            {
                delete this->ptr_;
            }
        }

        inline const Self &Const() const noexcept
        {
            return *this;
        }

        _T *Move() noexcept
        {
            _T ptr{this->ptr_};
            this->ptr_ = nullptr;
            return ptr;
        }

        std::unique_ptr<_T> AsUnique() noexcept
        {
            return std::unique_ptr<_T>{this->Move()};
        }

        std::shared_ptr<_T> AsShared()
        {
            return std::shared_ptr<_T>{this->Move()};
        }
    };
}

#endif