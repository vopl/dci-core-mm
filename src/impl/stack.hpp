/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include <cstddef>
#include "stack/content.hpp"

namespace dci::mm::impl
{
    class Stack final
    {
    public:
        Stack();
        Stack(Stack&& from);
        ~Stack();

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

    private:
        stack::Content* _content;
    };
}
