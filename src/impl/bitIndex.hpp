/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "config.hpp"

#include "bitIndex/level.hpp"
#include "bitIndex/orderEvaluator.hpp"

namespace dci::mm::impl
{
    template <std::size_t volume>
    class BitIndex
    {
    public:
        BitIndex();
        ~BitIndex();

        bitIndex::Address allocate();
        bool isAllocated(bitIndex::Address address);
        void deallocate(bitIndex::Address address);

    private:
        void updateProtection(bitIndex::Address addr);
        void updateProtection(void* addr);

        static constexpr std::size_t _order = bitIndex::OrderEvaluator<volume>::_order;

        using TopLevel = bitIndex::Level<_order, Config::_cacheLineSize>;

        struct Header
        {
            std::size_t _protectedSize;
            bitIndex::Address _maxAllocatedAddress;

        } _header;

        char _pad[Config::_cacheLineSize - sizeof(Header)];

        TopLevel _topLevel;
    };
}
