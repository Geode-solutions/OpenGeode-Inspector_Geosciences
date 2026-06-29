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

#pragma once

#include <geode/basic/pimpl.hpp>

#include <geode/model/mixin/core/component_type.hpp>

#include <geode/inspector/inspection/information.hpp>

#include <geode/inspector_geosciences/inspection/common.hpp>

namespace geode
{
    class StructuralModel;
} // namespace geode

namespace geode
{
    struct
        opengeode_inspector_geosciences_inspection_api StructuralModelGeologyInspectionResult
    {
        InspectionIssues< ComponentID > components_not_part_of_geology{
            "Geometrical components not part of any geological components"
        };
        InspectionIssues< ComponentID > empty_geological_components{
            "Geological components without any geometrical components"
        };
        InspectionIssues< uuid > invalid_horizons{
            "Horizons with invalid surfaces"
        };

        [[nodiscard]] index_t nb_issues() const;

        [[nodiscard]] std::string string() const;

        [[nodiscard]] std::string inspection_type() const;
    };

    /*!
     * Class for inspecting the geology of a StructuralModel model corners
     */
    class opengeode_inspector_inspection_api StructuralModelGeologyInspector
    {
        OPENGEODE_DISABLE_COPY( StructuralModelGeologyInspector );

    public:
        explicit StructuralModelGeologyInspector(
            const StructuralModel& model );

        ~StructuralModelGeologyInspector();

        /*!
         * Checks if the model is geologically valid
         */
        [[nodiscard]] bool structural_model_geology_is_valid() const;

        [[nodiscard]] bool geometrical_components_are_linked_to_geology() const;

        [[nodiscard]] bool geological_components_are_linked_to_geometry() const;

        [[nodiscard]] bool horizons_are_valid() const;

        [[nodiscard]] StructuralModelGeologyInspectionResult
            inspect_structural_model_geology() const;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
} // namespace geode