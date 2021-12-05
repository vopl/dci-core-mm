/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include "config.hpp"
#include "bitIndex.hpp"
#include "utils/align.hpp"

#include "stack/content.hpp"

namespace dci::mm::impl
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////
    class VirtualSpace
    {

    public:
        VirtualSpace();
        ~VirtualSpace();

        static VirtualSpace& single();

    public:
        stack::Content* allocStackContent();
        void freeStackContent(stack::Content* stackContent);
        void setupPanicHandler(void(*)(int));

        ////////////////////////////////////////////////////////////////
        bool vmAccessHandler(void* addr);
        void vmPanic(int signum);

    private:
        using StacksBitIndex = BitIndex<Config::_stacksAmount>;

        static constexpr std::size_t _stacksBitIndexAlignedSize = utils::alignUp(sizeof(StacksBitIndex), Config::_pageSize);
        static constexpr std::size_t _stacksPad = Config::_stackPages*Config::_pageSize;
        static constexpr std::size_t _stackSize = Config::_stackPages*Config::_pageSize;
        static constexpr std::size_t _stacksAlignedSize = Config::_stacksAmount * _stackSize;

        static constexpr std::size_t _vmSize =
                _stacksBitIndexAlignedSize + _stacksPad + _stacksAlignedSize;


        void* _vm;

        StacksBitIndex* _stacksBitIndex;
        void* _stacks;
        void(*_panic)(int){};
    };
}
