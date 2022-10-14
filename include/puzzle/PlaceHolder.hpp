#pragma once
#ifndef _PUZZLE_PLACEHOLDER_HPP
#define _PUZZLE_PLACEHOLDER_HPP

#include <utility>
#include <cassert>

#include "IServiceBuilder.hpp"

namespace puzzle
{
    class PlaceHolder
    {
    private:
        using Self = puzzle::PlaceHolder;
    
        puzzle::IServiceBuilder *builder_;
    public:
    
        explicit PlaceHolder(puzzle::IServiceBuilder *builder)
            :builder_(builder)
        {}
    
        PlaceHolder(const Self &other) noexcept = default;
    
        PlaceHolder(Self &&other) noexcept
            :builder_(other.builder_)
        {
            other.builder_ = nullptr;
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
            this->builder_ = other.builder_;
            other.builder_ = nullptr;
            return *this;
        }
    
        ~PlaceHolder() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }

        template<typename _T>
        operator _T() const
        {
            return this->builder_->BuildService<_T>();
        }
    };
}

#endif