#pragma once
#ifndef _PUZZLE_SERVICESCOPE_HPP
#define _PUZZLE_SERVICESCOPE_HPP

#include <cassert>
#include <typeindex>
#include <unordered_map>

#include "IServiceBuilder.hpp"
#include "IServiceCollection.hpp"

namespace puzzle
{
    class ServiceScope:puzzle::IServiceCollection
    {
    private:
        using Self = puzzle::ServiceScope;
        using ServiceMap = std::unordered_map<std::type_index,puzzle::IServiceObject *>;

        puzzle::IServiceBuilder *builder_;
        ServiceMap services_;

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
    public:

        explicit ServiceScope(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
            ,services_()
        {}

        ServiceScope(const Self &other) = delete;

        ServiceScope(Self &&other) noexcept
            :builder_(other.builder_)
            ,services_(std::move(other.services_))
        {
            other.builder_ = nullptr;
        }

        Self &operator=(const Self &other) = delete;

        inline Self &operator=(Self &&other) noexcept
        {
            if(this != std::addressof(other))
            {
                this->ClearServices();
                this->builder_ = other.builder_;
                other.builder_ = nullptr;
                this->services_ = std::move(other.services_);
            }
            return *this;
        }

        virtual ~ServiceScope() noexcept
        {
            this->ClearServices();
        }

        inline const Self &Const() const noexcept
        {
            return *this;
        }

        template<typename _T>
        inline _T BuildService()
        {
            assert(this->builder_ != nullptr);
            return this->builder_->BuildService<_T>(*this);
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
    };
}

#endif