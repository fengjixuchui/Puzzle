#pragma once
#ifndef _PUZZLE_IBASICSERVICEPROVIDER_HPP
#define _PUZZLE_IBASICSERVICEPROVIDER_HPP

#include "IServiceProvider.hpp"

namespace puzzle
{
    template<typename _T>
    class IBasicServiceProvider:public puzzle::IServiceProvider
    {
    private:
        using Self = puzzle::IBasicServiceProvider<_T>;

        inline virtual bool DoCheckType(const std::type_info &type) const noexcept override
        {
            return typeid(_T) == type;
        }
    protected:
    public:
    
        IBasicServiceProvider() noexcept = default;
    
        IBasicServiceProvider(const Self &other) noexcept = default;
    
        IBasicServiceProvider(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IBasicServiceProvider() noexcept = default;
    };   
}

#endif