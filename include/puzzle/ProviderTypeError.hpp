#pragma once
#ifndef _PUZZLE_PROVIDERTYPEERROR_HPP
#define _PUZZLE_PROVIDERTYPEERROR_HPP

#include <stdexcept>

namespace puzzle
{
    class ProviderTypeError:public std::logic_error
    {
    private:
    
        using Self = ProviderTypeError;
        using Base = std::logic_error;
    public:
    
        ProviderTypeError() noexcept
            :Base("provider's type is incorrect")
        {}
    
        ProviderTypeError(const char *msg) noexcept
            :Base(msg)
        {}
    
        ProviderTypeError(const Self &other) noexcept = default;
    
        ProviderTypeError(Self &&other) noexcept = default;
    
        ~ProviderTypeError() noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    };
}

#endif