#pragma once
#ifndef _PUZZLE_ISERVERPROVIDER_HPP
#define _PUZZLE_ISERVERPROVIDER_HPP

#include <cstring>
#include <typeinfo>

#include "IServiceCollection.hpp"

namespace puzzle
{
    class IServiceProvider
    {
    private:
        using Self = puzzle::IServiceProvider;
    protected:
        virtual void *DoProvide(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer) = 0;

        virtual bool DoCheckType(const std::type_info &type) const noexcept = 0;
    public:
    
        IServiceProvider() noexcept = default;
    
        IServiceProvider(const Self &other) noexcept = default;
    
        IServiceProvider(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IServiceProvider() noexcept = default;
    
        template<typename _T>
        inline _T Provide(puzzle::IServiceCollection *services)
        {
            char buffer[sizeof(_T)];
            _T *p = reinterpret_cast<_T*>(this->DoProvide(services,typeid(_T),buffer));
            return *p;
        }

        inline void *Provide(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer)
        {
            return this->DoProvide(services,type,buffer);
        }

        template<typename _T>
        inline bool CheckType() const noexcept
        {
            return this->DoCheckType(typeid(_T));
        }

        inline bool CheckType(const std::type_info &type) const noexcept
        {
            return this->DoCheckType(type);
        }
    };
}

#endif