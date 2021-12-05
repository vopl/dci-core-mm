/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <cstddef>

namespace dci::mm::impl::vm
{
    using TVmAccessHandler = bool (*)(void* addr);
    using TVmPanic = void (*)(int signum);

    bool init(TVmAccessHandler accessHandler, TVmPanic panic);
    bool deinit(TVmAccessHandler accessHandler);

    void* alloc(std::size_t size);
    bool free(void* addr, std::size_t size);

    enum class Protection
    {
        none,
#ifdef _WIN32
        guard,
#endif
        rw,
    };

    bool protect(void* addr, std::size_t size, Protection protection);
}
