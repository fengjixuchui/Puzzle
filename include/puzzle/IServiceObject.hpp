#pragma once
#ifndef _PUZZLE_ISERVICEOBJECT_HPP
#define _PUZZLE_ISERVICEOBJECT_HPP

#include <typeinfo>

namespace puzzle
{
    class IServiceObject
    {
    private:
        using Self = puzzle::IServiceObject;
    protected:

        virtual void *DoGetValue(const std::type_info &type) const = 0;
    public:
    
        IServiceObject() noexcept = default;
    
        IServiceObject(const Self &other) noexcept = default;
    
        IServiceObject(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IServiceObject() noexcept = default;

        inline void *GetValue(const std::type_info &type) const
        {
            return this->DoGetValue(type);
        }

        template<typename _T>
        inline _T *GetValue() const
        {
            _T *ptr{reinterpret_cast<_T*>(this->DoGetValue(typeid(_T)))};
            return ptr;
        }
    };
}

#endif