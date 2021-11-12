/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "vm.hpp"

#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>

#include <iostream>

/*
 * системное ограничение на количество сегментов vm для процесса,
 * /proc/sys/vm/max_map_count,
 * sysctl vm.max_map_count=16777216
 *
 * по этому лимиту будет отваливаться mprotect с ошибкой ENOMEM
 *
 *
*/

namespace dci::mm::impl::vm
{
    namespace
    {
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        extern char stateArea[];

        struct State
        {
            TVmAccessHandler    _accessHandler {};
            TVmPanic            _panic {};

            size_t              _altStackSize {SIGSTKSZ};
            char                _altStackArea[SIGSTKSZ] {};
            ::stack_t           _oldAltStack {};
            struct ::sigaction  _oldAction {};

            void * operator new(size_t size)
            {
                (void)size;
                return &stateArea;
            }

            void operator delete(void* ptr)
            {
                (void)ptr;
            }
        };
        char stateArea[sizeof(State)];

        State* _state = nullptr;

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void segvHandler(int signal_number, siginfo_t* info, void* ctx)
        {
            State* state = _state;

            if(state)
            {
                if(state->_accessHandler(info->si_addr))
                {
                    return;
                }


                char buf[64];
                std::sprintf(buf, "%p", info->si_addr);
                fputs("call SIGSEGV default handler for ", stderr);
                fputs(buf, stderr);
                fputs("\n", stderr);
                fflush(stderr);

                if(state->_oldAction.sa_flags & SA_SIGINFO)
                {
                    return state->_oldAction.sa_sigaction(signal_number, info, ctx);
                }
                else
                {
                    return state->_oldAction.sa_handler(signal_number);
                }
            }

            char buf[64];
            std::sprintf(buf, "%p", info->si_addr);
            fputs("unable to handle SIGSEGV for 0x", stderr);
            fputs(buf, stderr);
            fputs("\n", stderr);
            fflush(stderr);
            std::abort();
        }
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool init(TVmAccessHandler accessHandler, TVmPanic panic)
    {
        if(_state)
        {
            fprintf(stderr, "vm::threadInit: secondary call\n");
            return false;
        }
        _state = new State;
        _state->_accessHandler = accessHandler;
        _state->_panic = panic;

        ::stack_t altstack;
        memset(&altstack, 0, sizeof(altstack));
        altstack.ss_size = _state->_altStackSize;
        altstack.ss_sp = _state->_altStackArea;
        altstack.ss_flags = 0;
        if(sigaltstack(&altstack, &_state->_oldAltStack))
        {
            delete _state;
            _state = nullptr;
            perror("sigaltstack");
            return false;
        }

        struct ::sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = &segvHandler;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigfillset (&sa.sa_mask);
        if(sigaction(SIGSEGV, &sa, &_state->_oldAction))
        {
            delete _state;
            _state = nullptr;
            perror("sigaction");
            return false;
        }

        return true;

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool deinit(TVmAccessHandler accessHandler)
    {
        if(!_state)
        {
            fprintf(stderr, "vm::threadDeinit: already deinited\n");
            return false;
        }

        if(_state->_accessHandler != accessHandler)
        {
            fprintf(stderr, "vm::threadDeinit: wrong accessHandler\n");
            return false;
        }

        if(sigaction(SIGSEGV, &_state->_oldAction, nullptr))
        {
            perror("sigaction");
        }

        if(sigaltstack(&_state->_oldAltStack, nullptr))
        {
            perror("sigaltstack");
        }

        delete _state;
        _state = nullptr;
        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void* alloc(std::size_t size)
    {
        void* addr = mmap(
                            nullptr,
                            size,
                            PROT_NONE,
                            MAP_ANONYMOUS|MAP_PRIVATE,
                            0,
                            0);

        if(MAP_FAILED == addr)
        {
            perror("mmap");
            return nullptr;
        }


        if(madvise(addr, size, MADV_DONTDUMP))
        {
            perror("madvise");
            munmap(addr, size);//ignore error
            return nullptr;
        }

        return addr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool free(void* addr, std::size_t size)
    {
        if(munmap(addr, size))
        {
            perror("munmap");
            return false;
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool protect(void* addr, std::size_t size, bool access)
    {
        //std::cout<<(access ? "protect" : "unprotect")<<", "<<addr<<", "<<size<<std::endl;

        if(mprotect(addr, size, access ? (PROT_READ|PROT_WRITE) : PROT_NONE))
        {
            perror("mprotect");
            return false;
        }

        if(madvise(addr, size, access ? MADV_DODUMP : MADV_DONTDUMP))
        {
            perror("madvise");
            return false;
        }

        return true;
    }

}
