/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "stack.hpp"
#include "virtualSpace.hpp"

namespace dci::mm::impl
{
    Stack::Stack()
        : _content(nullptr)
    {
    }

    Stack::Stack(Stack&& from)
        : _content(from._content)
    {
        from._content = nullptr;
    }

    Stack::~Stack()
    {
        if(_content)
        {
            VirtualSpace::single().freeStackContent(_content);
            _content = nullptr;
        }
    }

    Stack& Stack::operator=(Stack&& from)
    {
        _content = from._content;
        from._content = nullptr;
        return *this;
    }

    void Stack::initialize()
    {
        if(_content)
        {
            throw "already initialized";
            return;
        }

        _content = VirtualSpace::single().allocStackContent();
    }

    bool Stack::initialized() const
    {
        return !!_content;
    }

    bool Stack::growsDown() const
    {
        dbgAssert(initialized());
        return _content->_growsDown;
    }

    bool Stack::hasGuard() const
    {
        dbgAssert(initialized());
        return _content->_hasGuard;
    }

    char* Stack::begin() const
    {
        dbgAssert(initialized());
        auto& header = _content->header();
        return header._userspaceBegin;
    }

    char* Stack::end() const
    {
        dbgAssert(initialized());
        auto& header = _content->header();
        return header._userspaceEnd;
    }

    std::size_t Stack::size() const
    {
        dbgAssert(initialized());
        auto& header = _content->header();
        return static_cast<std::size_t>(header._userspaceEnd - header._userspaceBegin);
    }

    void Stack::compact()
    {
        dbgAssert(initialized());
        _content->compact();
    }
}
