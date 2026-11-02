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

#include <geode/geosciences/explicit/representation/core/cross_section.hpp>

#include <geode/inspector_geosciences/inspection/geology/cross_section_geology.hpp>

namespace geode
{
    void define_cross_section_geology_inspector( pybind11::module& module )
    {
        pybind11::class_< CrossSectionGeologyInspectionResult >(
            module, "CrossSectionGeologyInspectionResult" )
            .def( pybind11::init<>() )
            .def_readwrite( "components_not_part_of_geology",
                &CrossSectionGeologyInspectionResult::
                    components_not_part_of_geology )
            .def_readwrite( "empty_geological_components",
                &CrossSectionGeologyInspectionResult::
                    empty_geological_components )
            .def_readwrite( "invalid_horizons",
                &CrossSectionGeologyInspectionResult::invalid_horizons )
            .def( "nb_issues", &CrossSectionGeologyInspectionResult::nb_issues )
            .def( "string", &CrossSectionGeologyInspectionResult::string )
            .def( "__str__", &CrossSectionGeologyInspectionResult::string )
            .def( "__repr__",
                &issues_repr< CrossSectionGeologyInspectionResult > )
            .def( "inspection_type",
                &CrossSectionGeologyInspectionResult::inspection_type );

        pybind11::class_< CrossSectionGeologyInspector >(
            module, "CrossSectionGeologyInspector" )
            .def( pybind11::init< const CrossSection& >() )
            .def( "cross_section_geology_is_valid",
                &CrossSectionGeologyInspector::cross_section_geology_is_valid )
            .def( "geometrical_components_are_linked_to_geology",
                &CrossSectionGeologyInspector::
                    geometrical_components_are_linked_to_geology )
            .def( "geological_components_are_linked_to_geometry",
                &CrossSectionGeologyInspector::
                    geological_components_are_linked_to_geometry )
            .def( "horizons_are_valid",
                &CrossSectionGeologyInspector::horizons_are_valid )
            .def( "inspect_cross_section_geology",
                &CrossSectionGeologyInspector::inspect_cross_section_geology );

        module.def(
            "inspect_cross_section_geology", []( const CrossSection& model ) {
                CrossSectionGeologyInspector inspector{ model };
                return inspector.inspect_cross_section_geology();
            } );
    }
} // namespace geode
