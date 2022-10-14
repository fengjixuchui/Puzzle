#pragma once
#ifndef _PUZZLE_PERSISTENTPTR_HPP
#define _PUZZLE_PERSISTENTPTR_HPP

#include <utility>

namespace puzzle
{
    template<typename _T>
    class PersistentPtr
    {
    private:
        using Self = PersistentPtr;
    
        _T ptr_;
    public:
        explicit PersistentPtr(_T &obj)
            :ptr_(&obj)
        {}
    
        PersistentPtr(const Self &other) noexcept = default;
    
        PersistentPtr(Self &&other) noexcept = default;
    
        inline Self &operator=(const Self &other) noexcept
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
                this->ptr_ = other.ptr_;
            }
            return *this;
        }
    
        ~PersistentPtr() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif