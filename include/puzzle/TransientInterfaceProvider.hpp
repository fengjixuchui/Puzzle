#pragma once
#ifndef _PUZZLE_TRANSIENTINTERFACEPROVIDER_HPP
#define _PUZZLE_TRANSIENTINTERFACEPROVIDER_HPP

#include "ServiceConstructor.hpp"
#include "ITransientServiceProvider.hpp"
#include "TransientPtr.hpp"

namespace puzzle
{
    template<typename _Interface,typename _Impl>
    class TransientInterfaceProvider:public puzzle::ITransientServiceProvider<puzzle::TransientPtr<_Interface>>
    {
    private:
        using Self = TransientInterfaceProvider;
    
    public:
    
        TransientInterfaceProvider();
    
        TransientInterfaceProvider(const Self &other);
    
        TransientInterfaceProvider(Self &&other) noexcept;
    
        inline Self &operator=(const Self &other)
        {
            if(this != std::addressof(other))
            {
                Self tmp{other};
                std::swap(tmp,*this);
            }
            return *this;
        }
    
        Self &operator=(Self &&other) noexcept;
    
        ~TransientInterfaceProvider() noexcept = default;
    
        inline const Self &Const() const noexcept
        {
            return *this;
        }
    };
}

#endif