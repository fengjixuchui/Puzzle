#pragma once
#ifndef _PUZZLE_SERVICEOBJECT_HPP
#define _PUZZLE_SERVICEOBJECT_HPP

#include "IServiceObject.hpp"

namespace puzzle
{
    template<typename _T>
    class ServiceObject:public puzzle::IServiceObject
    {
    private:
        using Self = puzzle::ServiceObject<_T>;
    
        _T value_;
    protected:
        virtual void *DoGetValue() const override
        {
            _T *ptr{const_cast<_T*>(&this->value_)};
            return ptr;
        }
    public:
    
        template<typename ..._Args,typename _Check = decltype(_T{std::declval<_Args>()...})>
        explicit ServiceObject(_Args &&...args)
            :value_(std::forward<_Args>(args)...)
        {}
    
        ServiceObject(const Self &other) = delete;
    
        ServiceObject(Self &&other) noexcept = delete;
    
        Self &operator=(const Self &other) = delete;
    
        Self &operator=(Self &&other) noexcept = delete;
    
        virtual ~ServiceObject() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif