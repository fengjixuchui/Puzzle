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
        puzzle::IServiceCollection *services_;
    public:

        explicit PlaceHolder(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services)
            :builder_(builder)
            ,services_(services)
        {}

        PlaceHolder(const Self &other) noexcept = default;

        PlaceHolder(Self &&other) noexcept
            :builder_(other.builder_)
            ,services_(other.services_)
        {
            other.builder_ = nullptr;
            other.services_ = nullptr;
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
                this->builder_ = other.builder_;
                other.builder_ = nullptr;
                this->services_ = other.services_;
                other.services_ = nullptr;
            }
            return *this;
        }

        ~PlaceHolder() noexcept = default;

        inline const Self &Const() const noexcept
        {
            return *this;
        }

        template<typename _T>
        inline operator _T() const
        {
            assert(this->services_ != nullptr);
            return this->builder_->BuildService<_T>(*this->services_);
        }

        inline operator puzzle::IServiceBuilder*() const noexcept
        {
            return this->builder_;
        }

        inline operator puzzle::IServiceBuilder&() const noexcept
        {
            assert(this->builder_ != nullptr);
            return *this->builder_;
        }
    };
}

#endif