/*
 * Copyright (c) 2019 - 2025 Geode-solutions
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

#include <pybind11/pybind11.h>

#include <geode/bindings/python/common.hpp>

#include <geode/inspector/bindings/python/inspection_helpers.hpp>

#include "cross_section_geology.hpp"
#include "cross_section_inspection.hpp"
#include "structural_model_geology.hpp"
#include "structural_model_inspection.hpp"

PYBIND11_MODULE( opengeode_inspector_geosciences_py_inspection, module )
{
    module.doc() =
        "Inspection of the geology of the structural models and cross "
        "sections of OpenGeode-Geosciences: the geology inspectors "
        "(StructuralModelGeologyInspector, CrossSectionGeologyInspector) "
        "check the links between the geometrical and the geological "
        "components and the horizons, and the model inspectors "
        "(StructuralModelInspector, CrossSectionInspector) add the "
        "inspection of the BRep or of the Section; inspect_structural_model, "
        "inspect_cross_section... do it in one call. Call "
        "OpenGeodeInspectorGeosciencesInspectionLibrary.initialize() before "
        "using the module.";
    geode::import_binding_dependency( module, "opengeode" );
    geode::import_binding_dependency( module, "opengeode_geosciences" );
    geode::import_binding_dependency( module, "opengeode_inspector" );
    pybind11::class_< geode::OpenGeodeInspectorGeosciencesInspectionLibrary >(
        module, "OpenGeodeInspectorGeosciencesInspectionLibrary",
        "Initialization of the geosciences inspection module" )
        .def_static( "initialize",
            &geode::OpenGeodeInspectorGeosciencesInspectionLibrary::initialize,
            "Initialize the geosciences inspection library, and the "
            "libraries it depends on. Calling it again has no effect" );
    geode::define_structural_model_geology_inspector( module );
    geode::define_structural_model_inspector( module );
    geode::define_cross_section_geology_inspector( module );
    geode::define_cross_section_inspector( module );
    geode::document_missing_constructors( module );
}