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
#ifndef CPPSORT_SORTER_FACADE_H_
#define CPPSORT_SORTER_FACADE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/detection.h>
#include "detail/as_function.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Sorter, typename Iterable>
        using has_sort_t = std::result_of_t<Sorter(Iterable&)>;

        template<typename Sorter, typename Iterable, typename Compare>
        using has_comparison_sort_t = utility::void_t<
            std::result_of_t<Sorter(Iterable&, Compare)>,
            std::result_of_t<Compare(
                decltype(*std::begin(std::declval<Iterable&>())),
                decltype(*std::end(std::declval<Iterable&>()))
            )>
        >;

        template<typename Sorter, typename Iterable, typename Projection,
                 typename ProjFunc = decltype(detail::as_function(std::declval<Projection&>()))>
        using has_projection_sort_t = utility::void_t<
            std::result_of_t<Sorter(Iterable&, Projection)>,
            std::result_of_t<ProjFunc(decltype(*std::begin(std::declval<Iterable&>())))>
        >;

        template<
            typename Sorter, typename Iterable,
            typename Compare, typename Projection,
            typename ProjFunc = decltype(detail::as_function(std::declval<Projection&>()))
        >
        using has_comparison_projection_sort_t = utility::void_t<
            std::result_of_t<Sorter(Iterable&, Compare, Projection)>,
            std::result_of_t<ProjFunc(decltype(*std::begin(std::declval<Iterable&>())))>,
            std::result_of_t<Compare(
                std::result_of_t<ProjFunc(decltype(*std::begin(std::declval<Iterable&>())))>,
                std::result_of_t<ProjFunc(decltype(*std::end(std::declval<Iterable&>())))>
            )>
        >;

        template<typename Sorter, typename Iterator>
        using has_sort_iterator_t = std::result_of_t<Sorter(Iterator, Iterator)>;

        template<typename Sorter, typename Iterator, typename Compare>
        using has_comparison_sort_iterator_t = utility::void_t<
            std::result_of_t<Sorter(Iterator, Iterator, Compare)>,
            std::result_of_t<Compare(
                decltype(*std::declval<Iterator&>()),
                decltype(*std::declval<Iterator&>())
            )>
        >;

        template<typename Sorter, typename Iterator, typename Projection,
                 typename ProjFunc = decltype(detail::as_function(std::declval<Projection&>()))>
        using has_projection_sort_iterator_t = utility::void_t<
            std::result_of_t<Sorter(Iterator, Iterator, Projection)>,
            std::result_of_t<ProjFunc(decltype(*std::declval<Iterator&>()))>
        >;

        template<
            typename Sorter, typename Iterator,
            typename Compare, typename Projection,
            typename ProjFunc = decltype(detail::as_function(std::declval<Projection&>()))
        >
        using has_comparison_projection_sort_iterator_t = utility::void_t<
            std::result_of_t<Sorter(Iterator, Iterator, Compare, Projection)>,
            std::result_of_t<ProjFunc(decltype(*std::declval<Iterator&>()))>,
            std::result_of_t<Compare(
                std::result_of_t<ProjFunc(decltype(*std::declval<Iterator&>()))>,
                std::result_of_t<ProjFunc(decltype(*std::declval<Iterator&>()))>
            )>
        >;

        template<typename Sorter, typename Iterable>
        constexpr bool has_sort
            = utility::is_detected_v<has_sort_t, Sorter, Iterable>;

        template<typename Sorter, typename Iterable, typename Compare>
        constexpr bool has_comparison_sort
            = utility::is_detected_v<has_comparison_sort_t, Sorter, Iterable, Compare>;

        template<typename Sorter, typename Iterable, typename Projection>
        constexpr bool has_projection_sort
            = utility::is_detected_v<has_projection_sort_t, Sorter, Iterable, Projection>;

        template<
            typename Sorter, typename Iterable,
            typename Compare, typename Projection
        >
        constexpr bool has_comparison_projection_sort
            = utility::is_detected_v<
                has_comparison_projection_sort_t,
                Sorter, Iterable, Compare, Projection
            >;

        template<typename Sorter, typename Iterator>
        constexpr bool has_sort_iterator
            = utility::is_detected_v<has_sort_iterator_t, Sorter, Iterator>;

        template<typename Sorter, typename Iterator, typename Compare>
        constexpr bool has_comparison_sort_iterator
            = utility::is_detected_v<has_comparison_sort_iterator_t, Sorter, Iterator, Compare>;

        template<typename Sorter, typename Iterator, typename Projection>
        constexpr bool has_projection_sort_iterator
            = utility::is_detected_v<has_projection_sort_iterator_t, Sorter, Iterator, Projection>;

        template<
            typename Sorter, typename Iterator,
            typename Compare, typename Projection
        >
        constexpr bool has_comparison_projection_sort_iterator
            = utility::is_detected_v<
                has_comparison_projection_sort_iterator_t,
                Sorter, Iterator, Compare, Projection
            >;
        // Black magic. Trust me, you *don't* want to know how this
        // works. This is probably retarded anyway, but I don't think
        // there is a standard-compliant way to achieve that without
        // breaking the user interface.

        template<typename Sorter>
        struct wrapper:
            Sorter
        {
#ifdef __clang__
            using Sorter::operator();

            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<false, utility::void_t<Iterable>>
            {}

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<false, utility::void_t<Iterable>>
            {}

            template<typename Iterable>
            auto operator()(Iterable& iterable, std::less<>) const
               -> std::enable_if_t<false, utility::void_t<Iterable>>
            {}

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, std::less<>) const
                -> std::enable_if_t<false, utility::void_t<Iterator>>
            {}

            template<typename Iterable>
            auto operator()(Iterable& iterable,
                            std::less<typename std::iterator_traits<decltype(std::begin(iterable))>::value_type>) const
                -> std::enable_if_t<false, utility::void_t<Iterable>>
            {}

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last,
                            std::less<typename std::iterator_traits<Iterator>::value_type>) const
                -> std::enable_if_t<false, utility::void_t<Iterator>>
            {}
#endif
        };
    }

    // This class is a CRTP base class whose sorters inherit
    // from which gives them the ability to convert to function
    // pointers. This mechanism is only possible if sorters are
    // stateless.

    template<typename Sorter>
    class sorter_facade
    {
        protected:

            ////////////////////////////////////////////////////////////
            // Function pointer types

            template<typename Iterable>
            using fptr_t = std::result_of_t<Sorter(Iterable&)>(*)(Iterable&);

            template<typename Iterable, typename Compare>
            using fptr_cmp_t = std::result_of_t<Sorter(Iterable&, Compare)>(*)(Iterable&, Compare);

            template<typename Iterator>
            using fptr_it_t = std::result_of_t<Sorter(Iterator, Iterator)>(*)(Iterator, Iterator);

            template<typename Iterator, typename Compare>
            using fptr_cmp_it_t = std::result_of_t<Sorter(Iterator, Iterator, Compare)>(*)(Iterator, Iterator, Compare);

        public:

            ////////////////////////////////////////////////////////////
            // Conversion to function pointers

            template<typename Iterable>
            operator fptr_t<Iterable>() const
            {
                return [](Iterable& iterable) {
                    return Sorter{}(iterable);
                };
            }

            template<typename Iterable, typename Compare>
            operator fptr_cmp_t<Iterable, Compare>() const
            {
                return [](Iterable& iterable, Compare compare) {
                    return Sorter{}(iterable, compare);
                };
            }

            template<typename Iterator>
            operator fptr_it_t<Iterator>() const
            {
                return [](Iterator first, Iterator last) {
                    return Sorter{}(first, last);
                };
            }

            template<typename Iterator, typename Compare>
            operator fptr_cmp_it_t<Iterator, Compare>() const
            {
                return [](Iterator first, Iterator last, Compare compare) {
                    return Sorter{}(first, last, compare);
                };
            }

            ////////////////////////////////////////////////////////////
            // Automatic iterable sorting interface

            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    not detail::has_sort<detail::wrapper<Sorter>, Iterable>,
                    decltype(std::declval<Sorter&>()(std::begin(iterable), std::end(iterable)))
                >
            {
                return Sorter{}(std::begin(iterable), std::end(iterable));
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort<detail::wrapper<Sorter>, Iterable, Compare>
                    &&  detail::has_comparison_sort_iterator<detail::wrapper<Sorter>, decltype(std::begin(iterable)), Compare>,
                    decltype(std::declval<Sorter&>()(std::begin(iterable), std::end(iterable), compare))
                >
            {
                return Sorter{}(std::begin(iterable), std::end(iterable), compare);
            }

            ////////////////////////////////////////////////////////////
            // Automatic overloads for std::less<>

            template<typename Iterable>
            auto operator()(Iterable& iterable, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<detail::wrapper<Sorter>, decltype(std::begin(iterable)), std::less<>>,
                    decltype(std::declval<Sorter&>()(iterable))
                >
            {
                return Sorter{}(iterable);
            }

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<detail::wrapper<Sorter>, Iterator, std::less<>>,
                    decltype(std::declval<Sorter&>()(first, last))
                >
            {
                return Sorter{}(first, last);
            }

            ////////////////////////////////////////////////////////////
            // Automatic overloads for std::less<T>

            template<typename Iterable>
            auto operator()(Iterable& iterable,
                            std::less<typename std::iterator_traits<decltype(std::begin(iterable))>::value_type>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<
                        detail::wrapper<Sorter>,
                        decltype(std::begin(iterable)),
                        std::less<typename std::iterator_traits<decltype(std::begin(iterable))>::value_type>
                    >,
                    decltype(std::declval<Sorter&>()(iterable))
                >
            {
                return Sorter{}(iterable);
            }

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last,
                            std::less<typename std::iterator_traits<Iterator>::value_type>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<
                        detail::wrapper<Sorter>,
                        Iterator,
                        std::less<typename std::iterator_traits<Iterator>::value_type>
                    >,
                    decltype(std::declval<Sorter&>()(first, last))
                >
            {
                return Sorter{}(first, last);
            }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
