#pragma once
#ifndef _PUZZLE_ITRANSIENTSERVICEPROVIDER_HPP
#define _PUZZLE_ITRANSIENTSERVICEPROVIDER_HPP

#include "IBasicServiceProvider.hpp"
#include "ProviderTypeError.hpp"

namespace puzzle
{
    template<typename _T>
    class ITransientServiceProvider:public puzzle::IBasicServiceProvider<_T>
    {
    private:
        using Self = puzzle::ITransientServiceProvider<_T>;
        
        inline virtual void *DoProvide(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer)
        {
            if(this->CheckType(type))
            {
                _T *buf{reinterpret_cast<_T*>(buffer)};
                return this->DoProvide(services,buf);
            }
            throw puzzle::ProviderTypeError{};
        }
    protected:

        virtual _T *DoProvide(puzzle::IServiceCollection *services,_T *buffer) = 0;
    public:
    
        ITransientServiceProvider() noexcept = default;
    
        ITransientServiceProvider(const Self &other) noexcept = default;
    
        ITransientServiceProvider(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~ITransientServiceProvider() noexcept = default;
    };
}

#endif