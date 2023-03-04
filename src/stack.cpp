/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/mm/stack.hpp>
#include "impl/stack.hpp"

namespace dci::mm
{
    ////////////////////////////////////////////////////////////////
    Stack::Stack()
        : Base()
    {
    }

    Stack::Stack(Stack&& from)
        : Base(std::move(from.impl()))
    {
    }

    Stack::~Stack()
    {
    }

    Stack& Stack::operator=(Stack&& from)
    {
        Base::operator=(std::move(from));
        return *this;
    }

    void Stack::initialize()
    {
        return impl().initialize();
    }

    bool Stack::initialized() const
    {
        return impl().initialized();
    }

    bool Stack::growsDown() const
    {
        return impl().growsDown();
    }

    bool Stack::hasGuard() const
    {
        return impl().hasGuard();
    }

    char* Stack::begin() const
    {
        return impl().begin();
    }

    char* Stack::end() const
    {
        return impl().end();
    }

    std::size_t Stack::size() const
    {
        return impl().size();
    }

    void Stack::compact()
    {
        return impl().compact();
    }

}
