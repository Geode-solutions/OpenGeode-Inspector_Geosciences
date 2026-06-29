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

#include <absl/types/span.h>

#include <geode/basic/algorithm.hpp>

#include <geode/model/mixin/core/component_type.hpp>

#include <geode/inspector/inspection/information.hpp>

#include <geode/inspector_geosciences/inspection/common.hpp>

namespace geode
{
    class StructuralModel;
    class CrossSection;
} // namespace geode

namespace geode
{
    namespace internal
    {
        template < typename Model >
        [[nodiscard]] bool model_geometrical_components_are_linked_to_geology(
            const Model& model );

        template < typename Model >
        [[nodiscard]] void
            add_model_geometrical_components_not_linked_to_geology(
                const Model& model, InspectionIssues< ComponentID >& issues );

        template < typename Model >
        [[nodiscard]] bool model_geological_components_are_linked_to_geometry(
            const Model& model );

        template < typename Model >
        [[nodiscard]] void
            add_model_geological_components_not_linked_to_geometry(
                const Model& model, InspectionIssues< ComponentID >& issues );

        template < index_t dimension >
        bool is_geological_collection( const ComponentType& component_type );

        template < typename Component >
        [[nodiscard]] std::string component_identification_to_string(
            const Component& component )
        {
            return absl::StrCat(
                component.name().value_or( component.id().string() ),
                component.name()
                    ? absl::StrCat( " (", component.id().string(), ")" )
                    : "" );
        }
    } // namespace internal
} // namespace geode