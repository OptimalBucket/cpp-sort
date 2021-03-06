/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
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
#include <forward_list>
#include <iterator>
#include <catch.hpp>
#include <cpp-sort/probes/mono.h>

TEST_CASE( "presortedness measure: mono", "[probe][mono]" )
{
    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( cppsort::probe::mono(li) == 2 );
        CHECK( cppsort::probe::mono(std::begin(li), std::end(li)) == 2 );
    }

    SECTION( "lower bound" )
    {
        const std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::mono(li) == 0 );
        CHECK( cppsort::probe::mono(std::begin(li), std::end(li)) == 0 );

        const std::forward_list<int> li1 = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::mono(li1) == 0 );

        const std::forward_list<int> li2 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        CHECK( cppsort::probe::mono(li2) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to:
        // size / 2

        const std::forward_list<int> li = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
        CHECK( cppsort::probe::mono(li) == 5 );
        CHECK( cppsort::probe::mono(std::begin(li), std::end(li)) == 5 );
    }

    SECTION( "equal neighbours in the sequence" )
    {
        const std::forward_list<int> li = { 0, 0, 0, 1, 2, 3, 4, 6, 5, 3 };
        CHECK( cppsort::probe::mono(li) == 1 );

        const std::forward_list<int> li1 = { 6, 5, 4, 3, 2, 2, 2, 2 };
        CHECK( cppsort::probe::mono(li1) == 0 );

        const std::forward_list<int> li2 = { 1, 1, 2, 8, 3, 3, 2, 1, 1, 5, 6 };
        CHECK( cppsort::probe::mono(li2) == 2 );
    }
}
