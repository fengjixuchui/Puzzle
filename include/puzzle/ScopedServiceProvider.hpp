#pragma once
#ifndef _PUZZLE_SCOPEDSERVICEPROVIDER_HPP
#define _PUZZLE_SCOPEDSERVICEPROVIDER_HPP

#include "ServiceConstructor.hpp"
#include "IScopedServiceProvider.hpp"
#include "ServiceObject.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl = _Interface>
    class ScopedServiceProvider:public puzzle::IScopedServiceProvider<_Interface>
    {
    private:
        using Self = puzzle::ScopedServiceProvider<_Interface,_Impl>;
    
        puzzle::IServiceBuilder *builder_;

        virtual _Interface *DoProvide(puzzle::IServiceCollection *services) override
        {
            assert(services->GetService<puzzle::PersistentPtr<_Interface>>() == nullptr);
            services->SetService<puzzle::PersistentPtr<_Interface>,puzzle::ServiceObject<_Impl>>(ServiceConstructor<_Impl>::ConstructService(this->builder_,services));
            puzzle::IServiceObject *service{services->GetService<puzzle::PersistentPtr<_Interface>>()};
            return service->GetValue<_Impl>();
        }
    public:
    
        explicit ScopedServiceProvider(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
        {}
    
        ScopedServiceProvider(const Self &other) = default;
    
        ScopedServiceProvider(Self &&other) noexcept
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
    
        ~ScopedServiceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };   
}

#endif