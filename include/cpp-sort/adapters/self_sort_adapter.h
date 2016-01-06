/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
#define CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/utility/has_sort_method.h>
#include "../detail/checkers.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct self_sort_adapter_impl:
            check_iterator_category<Sorter>
        {
            template<
                typename Iterable,
                typename... Args,
                typename = std::enable_if_t<utility::has_sort_method<Iterable, Args...>>
            >
            auto operator()(Iterable& iterable, Args&&... args) const
                -> decltype(iterable.sort(std::forward<Args>(args)...))
            {
                return iterable.sort(std::forward<Args>(args)...);
            }

            template<
                typename Iterable,
                typename... Args,
                typename = std::enable_if_t<not utility::has_sort_method<Iterable, Args...>>,
                typename = void // dummy parameter for ODR
            >
            auto operator()(Iterable& iterable, Args&&... args) const
                -> decltype(Sorter{}(iterable, std::forward<Args>(args)...))
            {
                return Sorter{}(iterable, std::forward<Args>(args)...);
            }

            template<typename Iterator, typename... Args>
            auto operator()(Iterator first, Iterator last, Args&&... args) const
                -> decltype(Sorter{}(first, last, std::forward<Args>(args)...))
            {
                return Sorter{}(first, last, std::forward<Args>(args)...);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            // We can't guarantee the stability of the sort method,
            // therefore we default the stability to false
            using is_stable = std::false_type;
        };
    }

    template<typename Sorter>
    struct self_sort_adapter:
        sorter_facade<detail::self_sort_adapter_impl<Sorter>>
    {};
}

#endif // CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
