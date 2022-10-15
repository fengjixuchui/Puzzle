#pragma once
#ifndef _PUZZLE_ISERVICECOLLECTION_HPP
#define _PUZZLE_ISERVICECOLLECTION_HPP

#include <exception>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include "IServiceObject.hpp"

namespace puzzle
{
    class IServiceCollection
    {
    private:
        using Self = puzzle::IServiceCollection;
    protected:

        virtual void DoSetService(std::type_index index,puzzle::IServiceObject *object) = 0;

        virtual puzzle::IServiceObject *DoGetService(const std::type_index &index) = 0;

        virtual void DoClearServices() noexcept = 0;
    public:
    
        IServiceCollection() noexcept = default;
    
        IServiceCollection(const Self &other) noexcept = default;
    
        IServiceCollection(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IServiceCollection() noexcept = default;

        template<typename _T,typename _Impl,typename ..._Args
                            ,typename _CheckBase = std::enable_if<std::is_base_of<puzzle::IServiceObject,_Impl>::value>::type
                            ,typename _Check = decltype(_Impl{std::declval<_Args>()...})>
        inline void SetService(_Args &&...args)
        {
            puzzle::IServiceObject *service{new _Impl{std::forward<_Args>(args)...}};
            if(!service)
            {
                throw std::bad_alloc{};
            }
            this->DoSetService(std::type_index{typeid(_T)},service);
        }

        template<typename _T>
        inline puzzle::IServiceObject *GetService()
        {
            puzzle::IServiceObject *service{this->DoGetService(std::type_index{typeid(_T)})};
            return service;
        }

        inline puzzle::IServiceObject *GetService(const std::type_info &type)
        {
            return this->DoGetService(std::type_index{type});
        }

        inline void ClearServices() noexcept
        {
            this->DoClearServices();
        }
    };
}

#endif