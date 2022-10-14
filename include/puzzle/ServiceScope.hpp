#pragma once
#ifndef _PUZZLE_SERVICESCOPE_HPP
#define _PUZZLE_SERVICESCOPE_HPP

#include <typeindex>
#include <unordered_map>

namespace puzzle
{
    class ServiceScope
    {
    private:
        using Self = puzzle::ServiceScope;
    
        
    public:
    
        ServiceScope();
    
        ServiceScope(const Self &other);
    
        ServiceScope(Self &&other) noexcept;
    
        inline Self &operator=(const Self &other)
        {
            if(this != std::addressof(other))
            {
                Self tmp{other};
                std::swap(tmp,*this);
            }
            return *this;
        }
    
        Self &operator=(Self &&other) noexcept;
    
        ~ServiceScope() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif