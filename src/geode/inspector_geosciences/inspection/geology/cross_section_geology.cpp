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

#include <geode/inspector_geosciences/inspection/geology/cross_section_geology.hpp>

#include <geode/basic/logger.hpp>
#include <geode/basic/pimpl_impl.hpp>

#include <geode/model/mixin/core/corner.hpp>
#include <geode/model/mixin/core/line.hpp>
#include <geode/model/mixin/core/surface.hpp>

#include <geode/geosciences/explicit/representation/core/cross_section.hpp>

#include <geode/inspector_geosciences/inspection/geology/geology_helpers.hpp>

namespace geode
{
    index_t CrossSectionGeologyInspectionResult::nb_issues() const
    {
        return components_not_part_of_geology.nb_issues()
               + empty_geological_components.nb_issues()
               + invalid_horizons.nb_issues();
    }

    std::string CrossSectionGeologyInspectionResult::string() const
    {
        return absl::StrCat( components_not_part_of_geology.string(),
            empty_geological_components.string(), invalid_horizons.string() );
    }

    std::string CrossSectionGeologyInspectionResult::inspection_type() const
    {
        return "Model geology inspection";
    }

    class CrossSectionGeologyInspector::Impl
    {
    public:
        Impl( const CrossSection& model ) : model_( model ) {}

        [[nodiscard]] bool cross_section_geology_is_valid() const
        {
            return geometrical_components_are_linked_to_geology()
                   && geological_components_are_linked_to_geometry()
                   && horizons_are_valid();
        }

        [[nodiscard]] bool geometrical_components_are_linked_to_geology() const
        {
            for( const auto& surface : model_.surfaces() )
            {
                if( !surface_is_in_a_fault_block( surface ) )
                {
                    return false;
                }
            }
            return internal::model_geometrical_components_are_linked_to_geology<
                CrossSection >( model_ );
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

        [[nodiscard]] CrossSectionGeologyInspectionResult
            inspect_cross_section_geology() const
        {
            CrossSectionGeologyInspectionResult result;
            internal::add_model_geometrical_components_not_linked_to_geology(
                model_, result.components_not_part_of_geology );
            for( const auto& surface : model_.surfaces() )
            {
                if( !surface_is_in_a_fault_block( surface ) )
                {
                    result.components_not_part_of_geology.add_issue(
                        surface.component_id(),
                        absl::StrCat( "Surface ",
                            internal::component_identification_to_string(
                                surface ),
                            " is not part of any FaultBlock " ) );
                }
            }
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
            const Horizon2D& horizon ) const
        {
            for( const auto& line : model_.horizon_items( horizon ) )
            {
                if( model_.is_closed( line ) )
                {
                    return absl::StrCat( "Horizon ",
                        internal::component_identification_to_string( horizon ),
                        " is invalid: it contains Line ",
                        internal::component_identification_to_string( line ),
                        " which is closed" );
                }
                /// Line has no free borders
                for( const auto& corner : model_.boundaries( line ) )
                {
                    if( model_.nb_incidences( corner.id() ) == 1 )
                    {
                        return absl::StrCat( "Horizon ",
                            internal::component_identification_to_string(
                                horizon ),
                            " is invalid: it contains Line ",
                            internal::component_identification_to_string(
                                line ),
                            " which has a free border on Corner ",
                            internal::component_identification_to_string(
                                corner ) );
                    }
                }
            }
            return std::nullopt;
        }

        bool surface_is_in_a_fault_block( const Surface2D& surface ) const
        {
            for( const auto& collection : model_.collections( surface.id() ) )
            {
                if( collection.type() == FaultBlock2D::component_type_static() )
                {
                    return true;
                }
            }
            return false;
        }

    private:
        const CrossSection& model_;
    };

    CrossSectionGeologyInspector::CrossSectionGeologyInspector(
        const CrossSection& model )
        : impl_( model )
    {
    }

    CrossSectionGeologyInspector::~CrossSectionGeologyInspector() = default;

    bool CrossSectionGeologyInspector::cross_section_geology_is_valid() const
    {
        return impl_->cross_section_geology_is_valid();
    }

    bool CrossSectionGeologyInspector::
        geometrical_components_are_linked_to_geology() const
    {
        return impl_->geometrical_components_are_linked_to_geology();
    }

    bool CrossSectionGeologyInspector::
        geological_components_are_linked_to_geometry() const
    {
        return impl_->geological_components_are_linked_to_geometry();
    }

    bool CrossSectionGeologyInspector::horizons_are_valid() const
    {
        return impl_->horizons_are_valid();
    }

    CrossSectionGeologyInspectionResult
        CrossSectionGeologyInspector::inspect_cross_section_geology() const
    {
        return impl_->inspect_cross_section_geology();
    }
} // namespace geode
