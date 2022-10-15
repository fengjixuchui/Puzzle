#pragma once
#ifndef _PUZZLE_TRANSIENTINTERFACEPROVIDER_HPP
#define _PUZZLE_TRANSIENTINTERFACEPROVIDER_HPP

#include "ServiceConstructor.hpp"
#include "ITransientServiceProvider.hpp"
#include "TransientPtr.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl>
    class TransientInterfaceProvider:public puzzle::ITransientServiceProvider<puzzle::TransientPtr<_Interface>>
    {
    private:
        using Self = puzzle::TransientInterfaceProvider<_Interface,_Impl>;
    
        puzzle::IServiceBuilder *builder_;

        inline virtual puzzle::TransientPtr<_Interface> *DoProvide(puzzle::IServiceCollection *services,puzzle::TransientPtr<_Interface> *buffer) override
        {
            _Interface *p{puzzle::ServiceConstructor<_Impl>::ConstructServicePtr(this->builder_,services)};
            if(!p)
            {
                throw std::bad_alloc{};
            }
            return ::new(buffer) puzzle::TransientPtr<_Interface>{p};
        }
    public:
    
        explicit TransientInterfaceProvider(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
        {}
    
        TransientInterfaceProvider(const Self &other) = default;
    
        TransientInterfaceProvider(Self &&other) noexcept
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
    
        ~TransientInterfaceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif