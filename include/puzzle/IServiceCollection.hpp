#pragma once
#ifndef _PUZZLE_ISERVICECOLLECTION_HPP
#define _PUZZLE_ISERVICECOLLECTION_HPP

#include <memory>
#include <typeinfo>
#include <typeindex>

#include "IServiceObject.hpp"

namespace puzzle
{
    class IServiceCollection
    {
    private:
        using Self = puzzle::IServiceCollection;
    protected:

        virtual void DoSet(std::type_index index,puzzle::IServiceObject *object) = 0;

        virtual puzzle::IServiceObject *DoGetService(const std::type_index &index) = 0;

        virtual void DoClear() noexcept = 0;
    public:
    
        IServiceCollection() noexcept = default;
    
        IServiceCollection(const Self &other) noexcept = default;
    
        IServiceCollection(Self &&other) noexcept = default;
    
        Self &operator=(const Self &other) noexcept = default;
    
        Self &operator=(Self &&other) noexcept = default;
    
        virtual ~IServiceCollection() noexcept = default;

        // template<typename _T>
        // inline void SetService(std::unique_ptr<puzzle::IServiceObject> service)
        // {
        //     this->DoSet(std::type_index{typeid(_T)},std::move(service));
        // }

        template<typename _T>
        inline _T *GetService()
        {
            puzzle::IServiceObject *service{this->DoGetService(std::type_index{typeid(_T)})};
            if(service)
            {
                return service->GetValue();
            }
            return nullptr;
        }

        inline puzzle::IServiceObject *GetService(const std::type_info &type)
        {
            return this->DoGetService(std::type_index{type});
        }

        inline void Clear() noexcept
        {
            this->DoClear();
        }
    };
}

#endif