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
#include "TransientInterfaceProvider.hpp"
#include "SingletonServiceProvider.hpp"
#include "ScopedServiceProvider.hpp"
#include "CustomizedTransientServiceProvider.hpp"
#include "CustomizedSingletionServiceProvider.hpp"
#include "CustomizedScopedServiceProvider.hpp"

namespace puzzle
{
    class ServiceCollection:public puzzle::IServiceCollection,public puzzle::IServiceBuilder
    {
    private:
        using Self = puzzle::ServiceCollection;
        using ProviderMap = std::unordered_map<std::type_index,puzzle::IServiceProvider *>;
        using ServiceMap = std::unordered_map<std::type_index,puzzle::IServiceObject *>;

        ProviderMap providers_;
        ServiceMap services_;

        inline virtual void *DoBuildService(puzzle::IServiceCollection *services,const std::type_info &type,char *buffer) override
        {
            puzzle::IServiceObject *service{services->GetService(type)};
            if(service != nullptr)
            {
                return ::new(buffer) void*{service->GetValue()};
            }
            auto ite = this->providers_.find(std::type_index{type});
            if(ite != this->providers_.end())
            {
                return ite->second->Provide(services,type,buffer);
            }
            throw puzzle::ProviderNotFoundError{};
        }

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

        inline virtual void DoClearServices() noexcept override
        {
            for(auto begin = this->services_.begin(),end = this->services_.end(); begin != end; ++begin)
            {
                delete begin->second;
            }
        }

        inline void DoClearProviders() noexcept
        {
            for(auto begin = this->providers_.begin(),end = this->providers_.end(); begin != end; ++begin)
            {
                delete begin->second;
            }
        }
    protected:
    public:

        ServiceCollection()
            :providers_()
            ,services_()
        {}

        ServiceCollection(const Self &other) = delete;

        ServiceCollection(Self &&other) noexcept = default;

        Self &operator=(const Self &other) = delete;

        inline Self &operator=(Self &&other) noexcept
        {
            if(this != std::addressof(other))
            {
                this->Clear();
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

        inline void ClearProviders() noexcept
        {
            this->DoClearProviders();
        }

        inline void Clear() noexcept
        {
            this->DoClearProviders();
            this->DoClearServices();
        }

        template<typename _T>
        inline _T BuildService(puzzle::IServiceCollection &service)
        {
            puzzle::IServiceBuilder *builder{this};
            return builder->BuildService<_T>(service);
        }

        template<typename _T>
        inline _T BuildService()
        {
            return this->BuildService<_T>(*this);
        }

        template<typename _T>
        inline puzzle::TransientPtr<_T> BuildTransient()
        {
            return this->BuildService<puzzle::TransientPtr<_T>>();
        }

        template<typename _T>
        inline puzzle::PersistentPtr<_T> BuildPersistent()
        {
            return this->BuildService<puzzle::PersistentPtr<_T>>();
        }

        template<typename _T,typename _Provider,typename ..._Args
            ,typename _CheckBase = typename std::enable_if<std::is_base_of<puzzle::IServiceProvider,_Provider>::value>::type
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
            catch(const std::exception &)
            {
                throw;
            }
            delete provider;
        }

        template<typename _T,std::size_t _Check = puzzle::ServiceConstructor<_T>::ParameterCount>
        inline Self &AddTransient()
        {
            return this->AddProvider<_T,puzzle::TransientObjectProvider<_T>>(*this);
        }

        template<typename _T>
        inline Self &AddTransient(std::function<_T(puzzle::IServiceCollection*)> func)
        {
            return this->AddProvider<_T,puzzle::CustomizedTransientServiceProvider<_T>>(std::move(func));
        }

        template<typename _Interface,typename _Impl,std::size_t _Check = puzzle::ServiceConstructor<_Impl>::ParameterCount>
        inline Self &AddTransient()
        {
            return this->AddProvider<puzzle::TransientPtr<_Interface>,puzzle::TransientInterfaceProvider<_Interface,_Impl>>(*this);
        }

        template<typename _Interface,typename _Impl>
        inline Self &AddTransient(std::function<puzzle::TransientPtr<_Interface>(puzzle::IServiceCollection*)> func)
        {
            return this->AddProvider<puzzle::TransientPtr<_Interface>,puzzle::CustomizedTransientServiceProvider<puzzle::TransientPtr<_Interface>>>(std::move(func));
        }

        template<typename _T,std::size_t _Check = puzzle::ServiceConstructor<_T>::ParameterCount>
        inline Self &AddSingletion()
        {
            return this->AddProvider<puzzle::PersistentPtr<_T>,puzzle::SingletonServiceProvider<_T>>(*this);
        }

        template<typename _T>
        inline Self &AddSingletion(std::function<_T*(puzzle::IServiceCollection*,char*)> func)
        {
            return this->AddProvider<puzzle::PersistentPtr<_T>,puzzle::CustomizedSingletionServiceProvider<_T>>(std::move(func));
        }

        template<typename _Interface,typename _Impl,std::size_t _Check = puzzle::ServiceConstructor<_Impl>::ParameterCount>
        inline Self &AddSingletion()
        {
            return this->AddProvider<puzzle::PersistentPtr<_Interface>,puzzle::SingletonServiceProvider<_Interface,_Impl>>(*this);
        }

        template<typename _Interface,typename _Impl>
        inline Self &AddSingletion(std::function<_Impl*(puzzle::IServiceCollection*,char*)> func)
        {
            return this->AddProvider<puzzle::PersistentPtr<_Interface>,puzzle::CustomizedSingletionServiceProvider<_Interface,_Impl>>(std::move(func));
        }

        template<typename _T,std::size_t _Check = puzzle::ServiceConstructor<_T>::ParameterCount>
        inline Self &AddScoped()
        {
            return this->AddProvider<puzzle::PersistentPtr<_T>,puzzle::ScopedServiceProvider<_T>>(*this);
        }

        template<typename _T>
        inline Self &AddScoped(std::function<_T(puzzle::IServiceCollection*)> func)
        {
            return this->AddProvider<puzzle::PersistentPtr<_T>,puzzle::CustomizedScopedServiceProvider<_T>>(std::move(func));
        }

        template<typename _Interface,typename _Impl,std::size_t _Check = puzzle::ServiceConstructor<_Impl>::ParameterCount>
        inline Self &AddScoped()
        {
            return this->AddProvider<puzzle::PersistentPtr<_Interface>,puzzle::ScopedServiceProvider<_Interface,_Impl>>(*this);
        }

        template<typename _Interface,typename _Impl>
        inline Self &AddScoped(std::function<_Impl(puzzle::IServiceCollection*)> func)
        {
            return this->AddProvider<puzzle::PersistentPtr<_Interface>,puzzle::CustomizedScopedServiceProvider<_Interface,_Impl>>(std::move(func));
        }
    };
}

#endif