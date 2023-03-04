/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include <cstddef>
#include "api.hpp"
#include <dci/himpl.hpp>
#include <dci/mm/implMetaInfo.hpp>

namespace dci::mm
{
    ////////////////////////////////////////////////////////////////
    class API_DCI_MM Stack
        : public dci::himpl::FaceLayout<Stack, impl::Stack>
    {
        using Base = dci::himpl::FaceLayout<Stack, impl::Stack>;

    public:
        Stack();
        Stack(const Stack& from) = delete;
        Stack(Stack&& from);
        ~Stack();

        Stack& operator=(const Stack& from) = delete;
        Stack& operator=(Stack&& from);

        void initialize();
        bool initialized() const;

    public:
        bool growsDown() const;
        bool hasGuard() const;

        char* begin() const;
        char* end() const;
        std::size_t size() const;

        void compact();
    };
}
