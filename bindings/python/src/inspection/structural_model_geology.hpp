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

#include <geode/model/mixin/core/component_type.hpp>

#include <geode/inspector_geosciences/inspection/geology/structural_model_geology.hpp>

namespace geode
{
    void define_structural_model_geology_inspector( pybind11::module& module )
    {
        pybind11::class_< InspectionIssues< ComponentID > >(
            module, "InspectionIssuesComponentID" )
            .def( "set_description",
                &InspectionIssues< ComponentID >::set_description )
            .def( "description", &InspectionIssues< ComponentID >::description )
            .def( "nb_issues", &InspectionIssues< ComponentID >::nb_issues )
            .def( "issues", &InspectionIssues< ComponentID >::issues )
            .def( "string", &InspectionIssues< ComponentID >::string );

        pybind11::class_< StructuralModelGeologyInspectionResult >(
            module, "StructuralModelGeologyInspectionResult" )
            .def( pybind11::init<>() )
            .def_readwrite( "components_not_part_of_geology",
                &StructuralModelGeologyInspectionResult::
                    components_not_part_of_geology )
            .def_readwrite( "empty_geological_components",
                &StructuralModelGeologyInspectionResult::
                    empty_geological_components )
            .def_readwrite( "invalid_horizons",
                &StructuralModelGeologyInspectionResult::invalid_horizons )
            .def( "nb_issues",
                &StructuralModelGeologyInspectionResult::nb_issues )
            .def( "string", &StructuralModelGeologyInspectionResult::string )
            .def( "inspection_type",
                &StructuralModelGeologyInspectionResult::inspection_type );

        pybind11::class_< StructuralModelGeologyInspector >(
            module, "StructuralModelGeologyInspector" )
            .def( pybind11::init< const StructuralModel& >() )
            .def( "inspect_structural_model_geology",
                &StructuralModelGeologyInspector::
                    inspect_structural_model_geology );

        module.def( "inspect_structural_model_geology",
            []( const StructuralModel& model ) {
                StructuralModelGeologyInspector inspector{ model };
                return inspector.inspect_structural_model_geology();
            } );
    }
} // namespace geode
