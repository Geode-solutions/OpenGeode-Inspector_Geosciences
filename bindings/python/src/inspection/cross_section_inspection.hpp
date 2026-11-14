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

#include <geode/inspector_geosciences/inspection/cross_section_inspection.hpp>

namespace geode
{
    void define_cross_section_inspector( pybind11::module& module )
    {
        pybind11::class_< CrossSectionInspectionResult >(
            module, "CrossSectionInspectionResult" )
            .def( pybind11::init<>(), ReleaseGil() )
            .def_readwrite( "section", &CrossSectionInspectionResult::section )
            .def_readwrite( "geology", &CrossSectionInspectionResult::geology )
            .def( "nb_issues", &CrossSectionInspectionResult::nb_issues )
            .def( "string", &CrossSectionInspectionResult::string )
            .def( "__str__", &CrossSectionInspectionResult::string )
            .def( "__repr__", &result_repr )
            .def( "inspection_type",
                &CrossSectionInspectionResult::inspection_type );

        pybind11::class_< CrossSectionInspector, SectionInspector,
            CrossSectionGeologyInspector >(
            module, "CrossSectionInspector", pybind11::dynamic_attr() )
            .def( pybind11::init< const CrossSection& >(),
                pybind11::keep_alive< 1, 2 >(), remember_model(), ReleaseGil() )
            .def( "inspect_cross_section",
                &CrossSectionInspector::inspect_cross_section, ReleaseGil() );

        module.def(
            "inspect_cross_section",
            []( const CrossSection& model ) {
                CrossSectionInspector inspector{ model };
                return inspector.inspect_cross_section();
            },
            ReleaseGil() );
    }
} // namespace geode
