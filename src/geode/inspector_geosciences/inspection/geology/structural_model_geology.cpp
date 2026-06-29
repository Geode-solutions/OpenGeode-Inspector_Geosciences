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

#include <geode/inspector_geosciences/inspection/geology/structural_model_geology.hpp>

#include <async++.h>

#include <geode/basic/logger.hpp>
#include <geode/basic/pimpl_impl.hpp>

#include <geode/model/mixin/core/block.hpp>
#include <geode/model/mixin/core/corner.hpp>
#include <geode/model/mixin/core/line.hpp>
#include <geode/model/mixin/core/surface.hpp>

#include <geode/geosciences/explicit/representation/core/structural_model.hpp>

#include <geode/inspector_geosciences/inspection/geology/geology_helpers.hpp>

namespace geode
{
    index_t StructuralModelGeologyInspectionResult::nb_issues() const
    {
        return components_not_part_of_geology.nb_issues()
               + empty_geological_components.nb_issues()
               + invalid_horizons.nb_issues();
    }

    std::string StructuralModelGeologyInspectionResult::string() const
    {
        return absl::StrCat( components_not_part_of_geology.string(),
            empty_geological_components.string(), invalid_horizons.string() );
    }

    std::string StructuralModelGeologyInspectionResult::inspection_type() const
    {
        return "Model geology inspection";
    }

    class StructuralModelGeologyInspector::Impl
    {
    public:
        Impl( const StructuralModel& model ) : model_( model ) {}

        [[nodiscard]] bool structural_model_geology_is_valid() const
        {
            return geometrical_components_are_linked_to_geology()
                   && geological_components_are_linked_to_geometry()
                   && horizons_are_valid();
        }

        [[nodiscard]] bool geometrical_components_are_linked_to_geology() const
        {
            return internal::model_geometrical_components_are_linked_to_geology(
                model_ );
        }

        [[nodiscard]] bool geological_components_are_linked_to_geometry() const
        {
            return internal::model_geological_components_are_linked_to_geometry(
                model_ );
        }

        [[nodiscard]] bool horizons_are_valid() const
        {
            for( const auto& horizon : model_.horizons() )
            {
                if( horizon_invalidity( horizon ) )
                {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] StructuralModelGeologyInspectionResult
            inspect_structural_model_geology() const
        {
            StructuralModelGeologyInspectionResult result;
            internal::add_model_geometrical_components_not_linked_to_geology(
                model_, result.components_not_part_of_geology );
            internal::add_model_geological_components_not_linked_to_geometry(
                model_, result.empty_geological_components );
            for( const auto& horizon : model_.horizons() )
            {
                if( auto invalidity = horizon_invalidity( horizon ) )
                {
                    result.invalid_horizons.add_issue(
                        horizon.id(), invalidity.value() );
                }
            }
            return result;
        }

    private:
        std::optional< std::string > horizon_invalidity(
            const Horizon3D& horizon ) const
        {
            for( const auto& surface : model_.horizon_items( horizon ) )
            {
                if( model_.is_closed( surface ) )
                {
                    return absl::StrCat( "Horizon ",
                        internal::component_identification_to_string( horizon ),
                        " is invalid: it contains Surface ",
                        internal::component_identification_to_string( surface ),
                        " which is closed" );
                }
                /// Surface has no free borders
                for( const auto& line : model_.boundaries( surface ) )
                {
                    if( model_.nb_incidences( line.id() ) == 1 )
                    {
                        return absl::StrCat( "Horizon ",
                            internal::component_identification_to_string(
                                horizon ),
                            " is invalid: it contains Surface ",
                            internal::component_identification_to_string(
                                surface ),
                            " which has a free border on Line ",
                            internal::component_identification_to_string(
                                line ) );
                    }
                }
            }
            return std::nullopt;
        }

    private:
        const StructuralModel& model_;
    };

    StructuralModelGeologyInspector::StructuralModelGeologyInspector(
        const StructuralModel& model )
        : impl_( model )
    {
    }

    StructuralModelGeologyInspector::~StructuralModelGeologyInspector() =
        default;

    bool StructuralModelGeologyInspector::structural_model_geology_is_valid()
        const
    {
        return impl_->structural_model_geology_is_valid();
    }

    bool StructuralModelGeologyInspector::
        geometrical_components_are_linked_to_geology() const
    {
        return impl_->geometrical_components_are_linked_to_geology();
    }

    bool StructuralModelGeologyInspector::
        geological_components_are_linked_to_geometry() const
    {
        return impl_->geological_components_are_linked_to_geometry();
    }

    bool StructuralModelGeologyInspector::horizons_are_valid() const
    {
        return impl_->horizons_are_valid();
    }

    StructuralModelGeologyInspectionResult
        StructuralModelGeologyInspector::inspect_structural_model_geology()
            const
    {
        return impl_->inspect_structural_model_geology();
    }
} // namespace geode
