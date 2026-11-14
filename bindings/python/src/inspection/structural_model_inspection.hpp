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

#include "../inspection_helpers.hpp"

namespace geode
{
    void define_structural_model_inspector( pybind11::module& module )
    {
        auto result =
            define_inspection_result< StructuralModelInspectionResult >( module,
                "StructuralModelInspectionResult",
                "Result of the inspection of a StructuralModel: the "
                "inspection of its BRep and of its geology" );
        INSPECTION_RESULT_MEMBER( result, StructuralModelInspectionResult, brep,
            "BRepInspectionResult: the inspection of the meshes and of the "
            "topology of the model" );
        INSPECTION_RESULT_MEMBER( result, StructuralModelInspectionResult,
            geology,
            "StructuralModelGeologyInspectionResult: the inspection of the "
            "geology of the model" );

        pybind11::class_< StructuralModelInspector, BRepInspector,
            StructuralModelGeologyInspector >( module,
            "StructuralModelInspector", pybind11::dynamic_attr(),
            "Complete inspection of a StructuralModel: the inspection of its "
            "BRep (BRepInspector) and of its geology "
            "(StructuralModelGeologyInspector), whose methods it also has" )
            .def( pybind11::init< const StructuralModel& >(),
                pybind11::arg( "model" ), pybind11::keep_alive< 1, 2 >(),
                remember_model(), ReleaseGil(),
                "Create the inspector of this StructuralModel. The inspector "
                "keeps the model alive" )
            .def( "inspect_structural_model",
                &StructuralModelInspector::inspect_structural_model,
                ReleaseGil(),
                "Return the StructuralModelInspectionResult: the inspection "
                "of the BRep and of the geology of the model" );

        module.def(
            "inspect_structural_model",
            []( const StructuralModel& model ) {
                const StructuralModelInspector inspector{ model };
                return inspector.inspect_structural_model();
            },
            pybind11::arg( "model" ), ReleaseGil(),
            "Inspect the StructuralModel with a StructuralModelInspector and "
            "return the StructuralModelInspectionResult" );
    }
} // namespace geode
