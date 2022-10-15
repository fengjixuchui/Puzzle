#pragma once
#ifndef _PUZZLE_CUSTOMIZEDSINGLETIONSERVICEPROVIDER_HPP
#define _PUZZLE_CUSTOMIZEDSINGLETIONSERVICEPROVIDER_HPP

#include <mutex>
#include <functional>

#include "ISingletonServiceProvider.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl = _Interface>
    class CustomizedSingletionServiceProvider:public puzzle::ISingletonServiceProvider<_Interface>
    {
    private:
        using Self = CustomizedSingletionServiceProvider<_Interface,_Impl>;
        using Func = std::function<_Impl*(puzzle::IServiceCollection*,char*)>;
    
        Func func_;
        std::once_flag flag_;
        char buf_[sizeof(_Impl)];
        _Interface *ptr_;

        void SetPtr(puzzle::IServiceCollection *services)
        {
            assert(this->func_);
            this->ptr_ = this->func_(services,this->buf_);
        }

        inline virtual _Interface *DoProvide(puzzle::IServiceCollection *services) override
        {
            assert(services != nullptr);
            assert(this->func_);
            std::call_once(this->flag_,std::bind(&Self::SetPtr,this,services));
            return this->ptr_;
        }
    public:
    
        explicit CustomizedSingletionServiceProvider(Func func)
            :func_(std::move(func))
            ,flag_()
            ,buf_()
            ,ptr_(nullptr)
        {
            assert(this->func_);
            std::memset(this->buf_,0,sizeof(this->buf_));
        }
    
        CustomizedSingletionServiceProvider(const Self &other) = delete;
    
        CustomizedSingletionServiceProvider(Self &&other) noexcept = delete;
    
        Self &operator=(const Self &other) = delete;
    
        Self &operator=(Self &&other) noexcept = delete;
    
        virtual ~CustomizedSingletionServiceProvider() noexcept
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