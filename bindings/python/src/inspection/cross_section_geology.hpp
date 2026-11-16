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

#include <geode/inspector/bindings/python/inspection_helpers.hpp>

namespace geode
{
    void define_cross_section_geology_inspector( pybind11::module& module )
    {
        auto result =
            define_inspection_result< CrossSectionGeologyInspectionResult >(
                module, "CrossSectionGeologyInspectionResult",
                "Result of the inspection of the geology of a CrossSection: "
                "the components not linked to the geology, the empty "
                "geological components and the invalid horizons" );
        INSPECTION_RESULT_MEMBER( result, CrossSectionGeologyInspectionResult,
            components_not_part_of_geology,
            "InspectionIssues of the ComponentID of the lines and surfaces "
            "not linked to any geological collection, and of the surfaces "
            "not in any FaultBlock" );
        INSPECTION_RESULT_MEMBER( result, CrossSectionGeologyInspectionResult,
            empty_geological_components,
            "InspectionIssues of the ComponentID of the geological "
            "components (faults, horizons...) without any geometrical "
            "component" );
        INSPECTION_RESULT_MEMBER( result, CrossSectionGeologyInspectionResult,
            invalid_horizons,
            "InspectionIssues of the uuid of the horizons containing a "
            "closed line or a line with a free border" );

        pybind11::class_< CrossSectionGeologyInspector >( module,
            "CrossSectionGeologyInspector",
            "Inspection of the geology of a CrossSection: the links between "
            "its geometrical components and its geological components "
            "(faults, horizons, fault blocks, stratigraphic units), and the "
            "validity of its horizons" )
            .def( pybind11::init< const CrossSection& >(),
                pybind11::arg( "cross_section" ),
                pybind11::keep_alive< 1, 2 >(), ReleaseGil(),
                "Create the inspector of the geology of this CrossSection. "
                "The inspector keeps the CrossSection alive" )
            .def( "cross_section_geology_is_valid",
                &CrossSectionGeologyInspector::cross_section_geology_is_valid,
                ReleaseGil(),
                "Return True if the geometrical components are linked to the "
                "geology, the geological components are linked to the "
                "geometry and the horizons are valid" )
            .def( "geometrical_components_are_linked_to_geology",
                &CrossSectionGeologyInspector::
                    geometrical_components_are_linked_to_geology,
                ReleaseGil(),
                "Return True if every surface is in a FaultBlock and every "
                "line and surface is in a geological collection or a "
                "ModelBoundary" )
            .def( "geological_components_are_linked_to_geometry",
                &CrossSectionGeologyInspector::
                    geological_components_are_linked_to_geometry,
                ReleaseGil(),
                "Return True if every geological component (fault, horizon, "
                "fault block...) contains at least one geometrical "
                "component" )
            .def( "horizons_are_valid",
                &CrossSectionGeologyInspector::horizons_are_valid, ReleaseGil(),
                "Return True if no horizon contains a closed line or a line "
                "with a free border" )
            .def( "inspect_cross_section_geology",
                &CrossSectionGeologyInspector::inspect_cross_section_geology,
                ReleaseGil(),
                "Return the CrossSectionGeologyInspectionResult: every issue "
                "of the geology of the cross section" );

        module.def(
            "inspect_cross_section_geology",
            []( const CrossSection& cross_section ) {
                const CrossSectionGeologyInspector inspector{ cross_section };
                return inspector.inspect_cross_section_geology();
            },
            pybind11::arg( "cross_section" ), ReleaseGil(),
            "Inspect the geology of the CrossSection with a "
            "CrossSectionGeologyInspector and return the "
            "CrossSectionGeologyInspectionResult" );
    }
} // namespace geode
