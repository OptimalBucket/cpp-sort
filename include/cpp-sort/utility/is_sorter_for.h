/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_UTILITY_IS_SORTER_FOR_H_
#define CPPSORT_UTILITY_IS_SORTER_FOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename Sorter, typename Iterable>
        using is_sorter_t = std::result_of_t<Sorter(Iterable&)>;

        template<typename Sorter, typename Iterable, typename Compare>
        using is_comparison_sorter_t = std::result_of_t<Sorter(Iterable&, Compare)>;
    }

    template<typename Sorter, typename Iterable>
    constexpr bool is_sorter = is_detected_v<detail::is_sorter_t, Sorter, Iterable>;

    template<typename Sorter, typename Iterable, typename Compare>
    constexpr bool is_comparison_sorter = is_detected_v<detail::is_comparison_sorter_t, Sorter, Iterable>;
}}

#endif // CPPSORT_UTILITY_IS_SORTER_FOR_H_
