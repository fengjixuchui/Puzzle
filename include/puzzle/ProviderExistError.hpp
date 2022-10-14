#pragma once
#ifndef _PUZZLE_PROVIDERALREADYEXIST_HPP
#define _PUZZLE_PROVIDERALREADYEXIST_HPP

#include <stdexcept>

namespace puzzle
{
    class ProviderExistError:public std::logic_error
    {
    private:
    
        using Self = ProviderExistError;
        using Base = std::logic_error;
    public:
    
        ProviderExistError() noexcept
            :Base("provider already exist")
        {}
    
        ProviderExistError(const char *msg) noexcept
            :Base(msg)
        {}
    
        ProviderExistError(const Self &other) noexcept = default;
    
        ProviderExistError(Self &&other) noexcept = default;
    
        ~ProviderExistError() noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    };
}

#endif