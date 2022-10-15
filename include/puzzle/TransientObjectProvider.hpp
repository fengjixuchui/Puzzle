#pragma once
#ifndef _PUZZLE_TRANSIENTSERVICEPROVIDER_HPP
#define _PUZZLE_TRANSIENTSERVICEPROVIDER_HPP

#include "ServiceConstructor.hpp"
#include "ITransientServiceProvider.hpp"

namespace puzzle
{
    template<typename _T>
    class TransientObjectProvider:public puzzle::ITransientServiceProvider<_T>
    {
    private:
        using Self = puzzle::TransientObjectProvider<_T>;

        puzzle::IServiceBuilder *builder_;

        inline virtual _T *DoProvide(puzzle::IServiceCollection *services,_T *buffer) override
        {
            return ::new(buffer) _T{puzzle::ServiceConstructor<_T>::ConstructService(this->builder_,services)};
        }
    public:

        explicit TransientObjectProvider(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
        {}

        TransientObjectProvider(const Self &other) = default;

        TransientObjectProvider(Self &&other) noexcept
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
            if(this != std::addressof(other))
            {
                this->builder_ = other.builder_;
                other.builder_ = nullptr;
            }
            return *this;
        }

        ~TransientObjectProvider() noexcept = default;

        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif