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

#include <geode/inspector_geosciences/inspection/structural_model_inspection.hpp>

#include <async++.h>

#include <geode/basic/logger.hpp>

namespace geode
{
    index_t StructuralModelInspectionResult::nb_issues() const
    {
        return brep.nb_issues() + geology.nb_issues();
    }

    std::string StructuralModelInspectionResult::string() const
    {
        return absl::StrCat( brep.string(), geology.string() );
    }

    std::string StructuralModelInspectionResult::inspection_type() const
    {
        return "Structural Model inspection";
    }

    StructuralModelInspector::StructuralModelInspector(
        const StructuralModel& model )
        : AddInspectors< StructuralModel,
              BRepInspector,
              StructuralModelGeologyInspector >{ model }
    {
    }

    StructuralModelInspectionResult
        StructuralModelInspector::inspect_structural_model() const
    {
        StructuralModelInspectionResult result;
        async::parallel_invoke(
            [&result, this] {
                result.brep = this->inspect_brep();
            },
            [&result, this] {
                result.geology = this->inspect_structural_model_geology();
            } );
        return result;
    }
} // namespace geode
