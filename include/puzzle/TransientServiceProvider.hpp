#pragma once
#ifndef _PUZZLE_TRANSIENTSERVICEPROVIDER_HPP
#define _PUZZLE_TRANSIENTSERVICEPROVIDER_HPP

#include <new>
#include <utility>
#include <cstring>

#include "ServiceConstructor.hpp"
#include "ITransientServiceProvider.hpp"

namespace puzzle
{
    template<typename _T>
    class TransientServiceProvider:public puzzle::ITransientServiceProvider<_T>
    {
    private:
        using Self = puzzle::TransientServiceProvider<_T>;

        puzzle::IServiceBuilder *builder_;
        
        inline virtual _T *DoProvide(_T *buffer) override
        {
            return ::new(buffer) _T{puzzle::ServiceConstructor<_T>::ConstructService(this->builder_)};
        }
    public:
    
        explicit TransientServiceProvider(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
        {}
    
        TransientServiceProvider(const Self &other) = default;
    
        TransientServiceProvider(Self &&other) noexcept
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
    
        ~TransientServiceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };   
}

#endif