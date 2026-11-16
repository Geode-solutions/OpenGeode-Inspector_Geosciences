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

#include <geode/inspector/bindings/python/inspection_helpers.hpp>

namespace geode
{
    void define_cross_section_inspector( pybind11::module& module )
    {
        auto result = define_inspection_result< CrossSectionInspectionResult >(
            module, "CrossSectionInspectionResult",
            "Result of the inspection of a CrossSection: the inspection "
            "of its Section and of its geology" );
        INSPECTION_RESULT_MEMBER( result, CrossSectionInspectionResult, section,
            "SectionInspectionResult: the inspection of the meshes and of "
            "the topology of the model" );
        INSPECTION_RESULT_MEMBER( result, CrossSectionInspectionResult, geology,
            "CrossSectionGeologyInspectionResult: the inspection of the "
            "geology of the model" );

        pybind11::class_< CrossSectionInspector, SectionInspector,
            CrossSectionGeologyInspector >( module, "CrossSectionInspector",
            pybind11::dynamic_attr(),
            "Complete inspection of a CrossSection: the inspection of its "
            "Section (SectionInspector) and of its geology "
            "(CrossSectionGeologyInspector), whose methods it also has" )
            .def( pybind11::init< const CrossSection& >(),
                pybind11::arg( "cross_section" ),
                pybind11::keep_alive< 1, 2 >(), remember_model(), ReleaseGil(),
                "Create the inspector of this CrossSection. The inspector "
                "keeps the CrossSection alive" )
            .def( "inspect_cross_section",
                &CrossSectionInspector::inspect_cross_section, ReleaseGil(),
                "Return the CrossSectionInspectionResult: the inspection of "
                "the Section and of the geology of the model" );

        module.def(
            "inspect_cross_section",
            []( const CrossSection& cross_section ) {
                const CrossSectionInspector inspector{ cross_section };
                return inspector.inspect_cross_section();
            },
            pybind11::arg( "cross_section" ), ReleaseGil(),
            "Inspect the CrossSection with a CrossSectionInspector and "
            "return the CrossSectionInspectionResult" );
    }
} // namespace geode
