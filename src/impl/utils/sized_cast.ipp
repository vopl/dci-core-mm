/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "sized_cast.hpp"

namespace dci::mm::impl::utils
{
    template <class To, class From>
    constexpr To sized_cast(From from) noexcept
    {
        static_assert(sizeof(To) == sizeof(From), "operands must be same size for sized_cast");

        if constexpr(std::is_pointer_v<From> || std::is_pointer_v<To>)
        {
            union U
            {
                From    _from;
                To      _to;

                U(From from) : _from{from} {}
                ~U(){}
            } u{from};

            return u._to;
        }
        else
        {
            return static_cast<To>(from);
        }
    }
}
