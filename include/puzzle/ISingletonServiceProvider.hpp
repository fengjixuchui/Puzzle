#pragma once
#ifndef _PUZZLE_ISINGLETONSERVICEPROVIDER_HPP
#define _PUZZLE_ISINGLETONSERVICEPROVIDER_HPP

#include <new>

#include "PersistentPtr.hpp"
#include "IBasicServiceProvider.hpp"
#include "ProviderTypeError.hpp"

namespace puzzle
{
    template<typename _T>
    class ISingletonServiceProvider:public puzzle::IBasicServiceProvider<puzzle::PersistentPtr<_T>>
    {
    private:
        using Self = puzzle::ISingletonServiceProvider<_T>;

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
    
        ISingletonServiceProvider() noexcept = default;
    
        ISingletonServiceProvider(const Self &other) noexcept = default;
    
        ISingletonServiceProvider(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~ISingletonServiceProvider() noexcept = default;
    };
}

#endif