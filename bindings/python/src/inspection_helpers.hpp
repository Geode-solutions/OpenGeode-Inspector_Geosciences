/*
 * Copyright (c) 2019 - 2026 Geode-solutions
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include <string>
#include <vector>

#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>

#include <pybind11/pybind11.h>

#include <geode/basic/types.hpp>

#include <geode/bindings/python/common.hpp>
#include <geode/bindings/python/model/model_vertex_check.hpp>

namespace geode
{
    /*!
     * The inspections only read the model: the GIL is released while they run.
     */
    using ReleaseGil = pybind11::call_guard< pybind11::gil_scoped_release >;

    // Call form of the inspection results: the total number of issues, and
    // the number of issues of each member having some, e.g.
    // StructuralModelInspectionResult(nb_issues=8, brep=5, geology=3)
    // Same representation as the results of OpenGeode-Inspector.
    inline std::string result_repr( const pybind11::object& self )
    {
        const auto property_type =
            pybind11::module_::import( "builtins" ).attr( "property" );
        std::vector< std::string > arguments{ absl::StrCat(
            "nb_issues=", self.attr( "nb_issues" )().cast< index_t >() ) };
        for( const auto item :
            pybind11::type::of( self ).attr( "__dict__" ).attr( "items" )() )
        {
            const auto member = item.cast< pybind11::tuple >();
            if( !pybind11::isinstance( member[1], property_type ) )
            {
                continue;
            }
            const auto value = self.attr( member[0] );
            if( !pybind11::hasattr( value, "nb_issues" ) )
            {
                continue;
            }
            const auto nb_issues =
                value.attr( "nb_issues" )().cast< index_t >();
            if( nb_issues != 0 )
            {
                arguments.push_back( absl::StrCat(
                    member[0].cast< std::string >(), "=", nb_issues ) );
            }
        }
        return absl::StrCat(
            pybind11::type::of( self ).attr( "__name__" ).cast< std::string >(),
            "(", absl::StrJoin( arguments, ", " ), ")" );
    }
} // namespace geode
