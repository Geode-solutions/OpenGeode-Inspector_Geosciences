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

#pragma once

#include <geode/inspector/inspection/brep_inspector.hpp>

#include <geode/inspector_geosciences/inspection/common.hpp>
#include <geode/inspector_geosciences/inspection/geology/structural_model_geology.hpp>

namespace geode
{
    class StructuralModel;
}

namespace geode
{
    struct
        opengeode_inspector_geosciences_inspection_api StructuralModelInspectionResult
    {
        BRepInspectionResult brep;
        StructuralModelGeologyInspectionResult geology;

        [[nodiscard]] index_t nb_issues() const;

        [[nodiscard]] std::string string() const;

        [[nodiscard]] std::string inspection_type() const;
    };
    /*!
     * Class for inspecting a StructuralModel
     * @extends StructuralModelTopologyInspector
     */
    class opengeode_inspector_geosciences_inspection_api
        StructuralModelInspector : public AddInspectors< StructuralModel,
                                       BRepInspector,
                                       StructuralModelGeologyInspector >
    {
        OPENGEODE_DISABLE_COPY( StructuralModelInspector );

    public:
        explicit StructuralModelInspector( const StructuralModel& model );

        [[nodiscard]] StructuralModelInspectionResult
            inspect_structural_model() const;
    };
} // namespace geode
