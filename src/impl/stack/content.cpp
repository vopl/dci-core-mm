/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "content.hpp"

#include "config.hpp"
#ifdef HAVE_VALGRIND
#   include <valgrind.h>
#endif

namespace dci::mm::impl::stack
{
    Content::Content()
        : Base()
    {
#ifdef HAVE_VALGRIND
        auto& header = Base::header();
        header._valgrindId = VALGRIND_STACK_REGISTER(header._userspaceBegin, header._userspaceEnd);
#endif
    }

    Content::~Content()
    {
#ifdef HAVE_VALGRIND
        auto& header = Base::header();
        VALGRIND_STACK_DEREGISTER(header._valgrindId);
#endif
    }

    const Header& Content::header()
    {
        return Base::header();
    }

}
