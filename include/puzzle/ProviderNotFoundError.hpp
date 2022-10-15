#pragma once
#ifndef _PUZZLE_PROVIDERNOTFOUNDERROR_HPP
#define _PUZZLE_PROVIDERNOTFOUNDERROR_HPP

#include <stdexcept>

namespace puzzle
{
    class ProviderNotFoundError:public std::logic_error
    {
    private:
    
        using Self = ProviderNotFoundError;
        using Base = std::logic_error;
    public:
    
        ProviderNotFoundError() noexcept
            :Base("Provider not found")
        {}
    
        explicit ProviderNotFoundError(const char *msg) noexcept
            :Base(msg)
        {}
    
        ProviderNotFoundError(const Self &other) noexcept = default;
    
        ProviderNotFoundError(Self &&other) noexcept = default;
    
        ~ProviderNotFoundError() noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    };
}

#endif