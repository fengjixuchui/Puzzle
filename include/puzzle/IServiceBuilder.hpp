#pragma once
#ifndef _PUZZLE_ISERVICEBUILDER_HPP
#define _PUZZLE_ISERVICEBUILDER_HPP

#include <utility>
#include <typeinfo>

namespace puzzle
{
    class IServiceCollection;

    class IServiceBuilder
    {
    private:
        using Self = puzzle::IServiceBuilder;
    protected:

        virtual void *DoBuildService(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer) = 0;
    public:
    
        IServiceBuilder() noexcept = default;
    
        IServiceBuilder(const Self &other) noexcept = default;
    
        IServiceBuilder(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IServiceBuilder() noexcept = default;

        template<typename _T>
        inline _T BuildService(puzzle::IServiceCollection &services)
        {
            char buf[sizeof(_T)] = {0};
            _T *p{reinterpret_cast<_T*>(this->DoBuildService(&services,typeid(_T),buf))};
            _T obj{std::move(*p)};
            p->~_T();
            return obj;
        }
    };   
}

#endif