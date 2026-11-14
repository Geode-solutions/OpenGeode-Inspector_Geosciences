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

#include <geode/geosciences/explicit/representation/core/structural_model.hpp>

#include <geode/inspector_geosciences/inspection/geology/structural_model_geology.hpp>

#include "../inspection_helpers.hpp"

namespace geode
{
    void define_structural_model_geology_inspector( pybind11::module& module )
    {
        auto result =
            define_inspection_result< StructuralModelGeologyInspectionResult >(
                module, "StructuralModelGeologyInspectionResult",
                "Result of the inspection of the geology of a StructuralModel: "
                "the components not linked to the geology, the empty "
                "geological components and the invalid horizons" );
        INSPECTION_RESULT_MEMBER( result,
            StructuralModelGeologyInspectionResult,
            components_not_part_of_geology,
            "InspectionIssues of the ComponentID of the surfaces and blocks "
            "not linked to any geological collection, and of the blocks not "
            "in any FaultBlock" );
        INSPECTION_RESULT_MEMBER( result,
            StructuralModelGeologyInspectionResult, empty_geological_components,
            "InspectionIssues of the ComponentID of the geological "
            "components (faults, horizons...) without any geometrical "
            "component" );
        INSPECTION_RESULT_MEMBER( result,
            StructuralModelGeologyInspectionResult, invalid_horizons,
            "InspectionIssues of the uuid of the horizons containing a "
            "closed surface or a surface with a free border" );

        pybind11::class_< StructuralModelGeologyInspector >( module,
            "StructuralModelGeologyInspector",
            "Inspection of the geology of a StructuralModel: the links "
            "between its geometrical components and its geological "
            "components (faults, horizons, fault blocks, stratigraphic "
            "units), and the validity of its horizons" )
            .def( pybind11::init< const StructuralModel& >(),
                pybind11::arg( "model" ), pybind11::keep_alive< 1, 2 >(),
                ReleaseGil(),
                "Create the inspector of the geology of this StructuralModel. "
                "The inspector keeps the model alive" )
            .def( "structural_model_geology_is_valid",
                &StructuralModelGeologyInspector::
                    structural_model_geology_is_valid,
                ReleaseGil(),
                "Return True if the geometrical components are linked to the "
                "geology, the geological components are linked to the "
                "geometry and the horizons are valid" )
            .def( "geometrical_components_are_linked_to_geology",
                &StructuralModelGeologyInspector::
                    geometrical_components_are_linked_to_geology,
                ReleaseGil(),
                "Return True if every block is in a FaultBlock and every "
                "surface and block is in a geological collection or a "
                "ModelBoundary" )
            .def( "geological_components_are_linked_to_geometry",
                &StructuralModelGeologyInspector::
                    geological_components_are_linked_to_geometry,
                ReleaseGil(),
                "Return True if every geological component (fault, horizon, "
                "fault block...) contains at least one geometrical "
                "component" )
            .def( "horizons_are_valid",
                &StructuralModelGeologyInspector::horizons_are_valid,
                ReleaseGil(),
                "Return True if no horizon contains a closed surface or a "
                "surface with a free border" )
            .def( "inspect_structural_model_geology",
                &StructuralModelGeologyInspector::
                    inspect_structural_model_geology,
                ReleaseGil(),
                "Return the StructuralModelGeologyInspectionResult: every "
                "issue of the geology of the model" );

        module.def(
            "inspect_structural_model_geology",
            []( const StructuralModel& model ) {
                const StructuralModelGeologyInspector inspector{ model };
                return inspector.inspect_structural_model_geology();
            },
            pybind11::arg( "model" ), ReleaseGil(),
            "Inspect the geology of the StructuralModel with a "
            "StructuralModelGeologyInspector and return the "
            "StructuralModelGeologyInspectionResult" );
    }
} // namespace geode
