#pragma once
#ifndef _PUZZLE_ISCOPEDSERVICEPROVIDER_HPP
#define _PUZZLE_ISCOPEDSERVICEPROVIDER_HPP

#include "PersistentPtr.hpp"
#include "IBasicServiceProvider.hpp"
#include "ProviderTypeError.hpp"

namespace puzzle
{
    template<typename _T>
    class IScopedServiceProvider:public puzzle::IBasicServiceProvider<puzzle::PersistentPtr<_T>>
    {
    private:
        using Self = puzzle::IScopedServiceProvider<_T>;

        virtual void *DoProvide(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer) override
        {
            if(this->CheckType(type))
            {
                return ::new(buffer) puzzle::PersistentPtr<_T>{*this->DoProvide(services)};
            }
            throw puzzle::ProviderTypeError{};
        }
    protected:

        virtual _T *DoProvide(puzzle::IServiceCollection *services) = 0;
    public:
    
        IScopedServiceProvider() noexcept = default;
    
        IScopedServiceProvider(const Self &other) noexcept = default;
    
        IScopedServiceProvider(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IScopedServiceProvider() noexcept = default;
    };
}

#endif