#pragma once
#ifndef _PUZZLE_CUSTOMIZEDTRANSIENTPROVIDER_HPP
#define _PUZZLE_CUSTOMIZEDTRANSIENTPROVIDER_HPP

#include <functional>

#include "ITransientServiceProvider.hpp"

namespace puzzle
{
    template<typename _T>
    class CustomizedTransientServiceProvider:public puzzle::ITransientServiceProvider<_T>
    {
    private:
        using Self = puzzle::CustomizedTransientServiceProvider<_T>;
        using Func = std::function<_T(puzzle::IServiceCollection*)>;
    
        Func func_;

        virtual _T *DoProvide(puzzle::IServiceCollection *services,_T *buffer) override
        {
            assert(services != nullptr);
            assert(this->func_);
            return ::new _T{this->func_(*services)};
        }
    public:
    
        explicit CustomizedTransientServiceProvider(Func func)
            :func_(std::move(func))
        {}
    
        CustomizedTransientServiceProvider(const Self &other) = default;
    
        CustomizedTransientServiceProvider(Self &&other) noexcept = default;
    
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
                this->func_ = std::move(other.func_);
            }
            return *this;
        }
    
        virtual ~CustomizedTransientServiceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif