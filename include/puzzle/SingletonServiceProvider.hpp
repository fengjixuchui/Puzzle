#pragma once
#ifndef _PUZZLE_SINGLETONSERVICEPROVIDER_HPP
#define _PUZZLE_SINGLETONSERVICEPROVIDER_HPP

#include <mutex>
#include <cstring>

#include "ServiceConstructor.hpp"
#include "ISingletonServiceProvider.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl = _Interface>
    class SingletonServiceProvider:public puzzle::ISingletonServiceProvider<_Interface>
    {
    private:
        using Self = puzzle::SingletonServiceProvider<_Interface,_Impl>;
    
        puzzle::IServiceBuilder *builder_;
        char buf_[sizeof(_Impl)];
        _Interface *ptr_;
        std::once_flag flag_;

        void SetPtr(puzzle::IServiceCollection *services)
        {
            this->ptr_ = ::new(this->buf_) _Impl{puzzle::ServiceConstructor<_Impl>::ConstructService(this->builder_,services)};
        }

        virtual _Interface *DoProvide(puzzle::IServiceCollection *services) override
        {
            std::call_once(this->flag_,std::bind(&Self::SetPtr,this,services));
            return this->ptr_;
        }
    public:
    
        explicit SingletonServiceProvider(puzzle::IServiceBuilder &builder)
            :builder_(&builder)
            ,buf_()
            ,ptr_(nullptr)
            ,flag_()
        {
            assert(this->builder_ != nullptr);
            std::memset(this->buf_,0,sizeof(buf_));
        }
    
        SingletonServiceProvider(const Self &other) = delete;
    
        SingletonServiceProvider(Self &&other) noexcept = delete;
    
        Self &operator=(const Self &other) = delete;
    
        Self &operator=(Self &&other) noexcept = delete;
    
        virtual ~SingletonServiceProvider() noexcept
        {
            if(this->ptr_)
            {
                this->ptr_->~_Interface();
            }
        }
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif