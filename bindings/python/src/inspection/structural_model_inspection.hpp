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

#include <geode/inspector_geosciences/inspection/structural_model_inspection.hpp>

namespace geode
{
    void define_structural_model_inspector( pybind11::module& module )
    {
        pybind11::class_< StructuralModelInspectionResult >(
            module, "StructuralModelInspectionResult" )
            .def( pybind11::init<>() )
            .def_readwrite( "brep", &StructuralModelInspectionResult::brep )
            .def_readwrite(
                "geology", &StructuralModelInspectionResult::geology )
            .def( "nb_issues", &StructuralModelInspectionResult::nb_issues )
            .def( "string", &StructuralModelInspectionResult::string )
            .def( "inspection_type",
                &StructuralModelInspectionResult::inspection_type );

        pybind11::class_< StructuralModelInspector, BRepInspector,
            StructuralModelGeologyInspector >(
            module, "StructuralModelInspector" )
            .def( pybind11::init< const StructuralModel& >() )
            .def( "inspect_structural_model",
                &StructuralModelInspector::inspect_structural_model );

        module.def(
            "inspect_structural_model", []( const StructuralModel& model ) {
                StructuralModelInspector inspector{ model };
                return inspector.inspect_structural_model();
            } );
    }
} // namespace geode
