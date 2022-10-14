#pragma once
#ifndef _PUZZLE_SERVICECOLLECTION_HPP
#define _PUZZLE_SERVICECOLLECTION_HPP

#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "IServiceBuilder.hpp"
#include "IServiceCollection.hpp"
#include "ProviderNotFoundError.hpp"
#include "ProviderExistError.hpp"
#include "TransientObjectProvider.hpp"

namespace puzzle
{
    class ServiceCollection:public puzzle::IServiceCollection,public puzzle::IServiceBuilder
    {
    private:
        using Self = puzzle::ServiceCollection;
        using ProviderMap = std::unordered_map<std::type_index,puzzle::IServiceProvider*>;
        using ServiceMap = std::unordered_map<std::type_index,puzzle::IServiceObject*>;

        ProviderMap providers_;
        ServiceMap services_;

        inline void *DoBuildService(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer)
        {
            puzzle::IServiceObject *service{services->GetService(type)};
            if(service != nullptr)
            {
                return service->GetValue(type);
            }
            auto ite = this->providers_.find(std::type_index{type});
            if(ite != this->providers_.end())
            {
                return ite->second->Provide(services,type,buffer);
            }
            throw puzzle::ProviderNotFoundError{};
        }
    protected:

        inline virtual void DoSetService(std::type_index index,puzzle::IServiceObject *service) override
        {
            this->services_.emplace(index,std::move(service));
        }

        inline virtual puzzle::IServiceObject *DoGetService(const std::type_index &index) override
        {
            auto ite{this->services_.find(index)};
            if(ite != this->services_.end())
            {
                return ite->second;
            }
            return nullptr;
        }

        inline virtual void DoClear() noexcept override
        {
            for(auto begin = this->services_.begin(),end = this->services_.end(); begin != end; ++begin)
            {
                delete begin->second;
            }   
            for(auto begin = this->providers_.begin(),end = this->providers_.end(); begin != end; ++begin)
            {
                delete begin->second;
            }   
        }

        inline virtual void *DoBuildService(const std::type_info &type,char *buffer) override
        {
            return this->DoBuildService(this,type,buffer);
        }
    public:
    
        ServiceCollection()
            :providers_()
            ,services_()
        {}
    
        ServiceCollection(const Self &other) = default;
    
        ServiceCollection(Self &&other) noexcept = default;
    
        inline Self &operator=(const Self &other)
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
                this->providers_ = std::move(other.providers_);
                this->services_ = std::move(other.services_);
            }
            return *this;
        }
    
        ~ServiceCollection() noexcept
        {
            this->Clear();
        }
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }

        template<typename _T>
        inline _T BuildService(puzzle::IServiceCollection &service)
        {
            char buf[sizeof(_T)] = {0};
            _T *p{reinterpret_cast<_T>(this->DoBuildService(&service,typeid(_T),buf))};
            return *p;
        }

        template<typename _T>
        inline _T BuildService()
        {
            puzzle::IServiceBuilder *builder{this};
            return builder->BuildService<_T>();
        }

        template<typename _T,typename _Provider,typename ..._Args
                    ,typename _CheckBase = std::enable_if<std::is_base_of<puzzle::IServiceProvider,_Provider>::value>::type
                    ,typename _Check = decltype(_Provider{std::declval<_Args>()...})>
        inline Self &AddProvider(_Args &&...args)
        {
            puzzle::IServiceProvider *provider{new _Provider{std::forward<_Args>(args)...}};
            if(!provider)
            {
                throw std::bad_alloc{};
            }
            try
            {
                auto result{this->providers_.emplace(std::type_index{typeid(_T)},provider)};
                if(!result.second)
                {
                    delete provider;
                    throw puzzle::ProviderExistError{};
                }
                return *this;
            }
            catch(const std::exception&)
            {
                throw;
            }
            delete provider;
        }

        template<typename _T>
        inline Self &AddTransient()
        {
            return this->AddProvider<_T,puzzle::TransientObjectProvider<_T>>(*this);
        }
    };
}

#endif