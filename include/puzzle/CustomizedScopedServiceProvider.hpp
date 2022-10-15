#pragma once
#ifndef _PUZZLE_CUSTOMIZEDSCOPEDSERVICEPROVIDER_HPP
#define _PUZZLE_CUSTOMIZEDSCOPEDSERVICEPROVIDER_HPP

#include <cassert>
#include <functional>

#include "IScopedServiceProvider.hpp"
#include "ServiceObject.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl = _Interface>
    class CustomizedScopedServiceProvider:public puzzle::IScopedServiceProvider<_Interface>
    {
    private:
        using Self = puzzle::CustomizedScopedServiceProvider<_Interface,_Impl>;
        using Func = std::function<_Impl(puzzle::IServiceCollection*)>;

        Func func_;

        virtual _Interface *DoProvide(puzzle::IServiceCollection *services) override
        {
            assert(services->GetService<puzzle::PersistentPtr<_Interface>>() == nullptr);
            assert(this->func_);
            services->SetService<puzzle::PersistentPtr<_Interface>,puzzle::ServiceObject<_Impl>>(this->func_(services));
            puzzle::IServiceObject *service{services->GetService<puzzle::PersistentPtr<_Interface>>()};
            return service->GetValue<_Interface>();
        }
    public:
    
        explicit CustomizedScopedServiceProvider(Func func)
            :func_(std::move(func))
        {}
    
        CustomizedScopedServiceProvider(const Self &other) = default;
    
        CustomizedScopedServiceProvider(Self &&other) noexcept = default;
    
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
    
        ~CustomizedScopedServiceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif