#pragma once
#ifndef _PUZZLE_SERVICECONSTRUCTOR_HPP
#define _PUZZLE_SERVICECONSTRUCTOR_HPP

#include <new>
#include <cstdint>
#include <utility>
#include <functional>
#include <type_traits>

#include "IServiceBuilder.hpp"
#include "ServiceInject.hpp"
#include "PlaceHolder.hpp"

namespace puzzle
{
    template<typename _T,typename ..._Types>
    struct ServiceConstructor
    {
    private:
        template<typename _T>
        static puzzle::PlaceHolder MakePlaceHolder(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services) noexcept
        {
            return puzzle::PlaceHolder{builder,services};
        }

        template<typename _Check = decltype(_T{puzzle::InjectTag,std::declval<_Types>()...})>
        static constexpr std::size_t InternalGetParameterCount(int)
        {
            return sizeof...(_Types);
        }

        template<typename _Check = void>
        static constexpr std::size_t InternalGetParameterCount(...)
        {
            return ServiceConstructor<_T,_Types...,puzzle::PlaceHolder>::ParameterCount;
        }

        template<typename _U = _T,typename _Check = decltype(_T{puzzle::InjectTag,std::declval<_Types>()...})>
        static _U InternalConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,int)
        {
            return _U{puzzle::InjectTag,MakePlaceHolder<_Types>(builder,services)...};
        }

        template<typename _U = _T,typename _Check = void>
        static _U InternalConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,...)
        {
            return ServiceConstructor<_U,_Types...,puzzle::PlaceHolder>::ConstructService(builder,services);
        }

        template<typename _U = _T,typename _Check = decltype(_T{puzzle::InjectTag,std::declval<_Types>()...})>
        static _U *InternalConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,int)
        {
            return new _U{puzzle::InjectTag,MakePlaceHolder<_Types>(builder,services)...};
        }

        template<typename _U = _T,typename _Check = void>
        static _U *InternalConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,...)
        {
            return ServiceConstructor<_U,_Types...,puzzle::PlaceHolder>::ConstructServicePtr(builder,services);
        }
    public:

        template<typename _U = _T>
        static _U ConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services)
        {
            return InternalConstructService(builder,services,0);
        }

        template<typename _U = _T>
        static _U *ConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services)
        {
            return InternalConstructServicePtr(builder,services,0);
        }

        static constexpr std::size_t ParameterCount{ServiceConstructor<_T,_Types...>::InternalGetParameterCount(0)};
    };

    template<typename _T>
    struct ServiceConstructor<_T>
    {
    private:

        template<typename _Check = decltype(_T{puzzle::InjectTag})>
        static constexpr std::size_t InternalGetParameterCount(int)
        {
            return 0;
        }

        template<typename _Check = void>
        static constexpr std::size_t InternalGetParameterCount(...)
        {
            return ServiceConstructor<_T,puzzle::PlaceHolder>::ParameterCount;
        }

        template<typename _U = _T,typename _Check = decltype(_T{puzzle::InjectTag})>
        static _U InternalConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,int)
        {
            return _U{puzzle::InjectTag};
        }

        template<typename _U = _T,typename _Check = void>
        static _U InternalConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,...)
        {
            return ServiceConstructor<_U,puzzle::PlaceHolder>::ConstructService(builder,services);
        }

        template<typename _U = _T,typename _Check = decltype(_T{puzzle::InjectTag})>
        static _U *InternalConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,int)
        {
            return new _U{puzzle::InjectTag};
        }

        template<typename _U = _T,typename _Check = void>
        static _U *InternalConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services,...)
        {
            return ServiceConstructor<_U,puzzle::PlaceHolder>::ConstructServicePtr(builder,services);
        }

    public:

        template<typename _U = _T>
        static _U ConstructService(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services)
        {
            return InternalConstructService(builder,services,0);
        }

        template<typename _U = _T>
        static _U *ConstructServicePtr(puzzle::IServiceBuilder *builder,puzzle::IServiceCollection *services)
        {
            return InternalConstructServicePtr(builder,services,0);
        }

        static constexpr std::size_t ParameterCount{ServiceConstructor<_T>::InternalGetParameterCount(0)};
    };

#ifndef _PUZZLE_UNLIMITED_PARAMETER_COUNT
//max parameter count is 20
#define _PUZZLE_MAX_PARAMETER    puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder \
                                ,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder \
                                ,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder \
                                ,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder,puzzle::PlaceHolder \
                                ,puzzle::PlaceHolder

    template<typename _T>
    struct ServiceConstructor<_T,_PUZZLE_MAX_PARAMETER>
    {};
#undef _PUZZLE_MAX_PARAMETER
#endif
}

#endif