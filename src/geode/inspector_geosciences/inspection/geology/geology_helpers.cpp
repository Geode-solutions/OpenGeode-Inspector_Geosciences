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

#include <geode/inspector_geosciences/inspection/geology/geology_helpers.hpp>

#include <geode/model/mixin/core/block.hpp>
#include <geode/model/mixin/core/corner.hpp>
#include <geode/model/mixin/core/line.hpp>
#include <geode/model/mixin/core/model_boundary.hpp>
#include <geode/model/mixin/core/surface.hpp>

#include <geode/geosciences/explicit/mixin/core/fault.hpp>
#include <geode/geosciences/explicit/mixin/core/fault_block.hpp>
#include <geode/geosciences/explicit/mixin/core/horizon.hpp>
#include <geode/geosciences/explicit/mixin/core/stratigraphic_unit.hpp>
#include <geode/geosciences/explicit/representation/core/cross_section.hpp>
#include <geode/geosciences/explicit/representation/core/structural_model.hpp>

namespace
{
    template < typename Model >
    [[nodiscard]] bool component_is_linked_to_geology(
        const Model& model, const geode::uuid& component_id )
    {
        for( const auto& collection : model.collections( component_id ) )
        {
            if( geode::internal::is_geological_collection< Model::dim >(
                    collection.type() )
                || collection.type()
                       == geode::ModelBoundary<
                           Model::dim >::component_type_static() )
            {
                return true;
            }
        }
        return false;
    }

    template < typename Model, typename ComponentRange >
    [[nodiscard]] bool components_are_linked_to_geology(
        const Model& model, ComponentRange component_range )
    {
        for( const auto& component : component_range )
        {
            if( !component_is_linked_to_geology( model, component.id() ) )
            {
                return false;
            }
        }
        return true;
    }

    template < typename Model >
    [[nodiscard]] bool components_are_linked_to_geology_base(
        const Model& model )
    {
        return components_are_linked_to_geology( model, model.corners() )
               && components_are_linked_to_geology( model, model.lines() )
               && components_are_linked_to_geology( model, model.surfaces() );
    }

    template < typename Model, typename ComponentRange >
    void add_components_not_linked_to_geology( const Model& model,
        geode::InspectionIssues< geode::ComponentID >& issues,
        ComponentRange component_range )
    {
        for( const auto& component : component_range )
        {
            if( !component_is_linked_to_geology( model, component.id() ) )
            {
                issues.add_issue( component.component_id(),
                    absl::StrCat( component.component_type().get(), " ",
                        geode::internal::component_identification_to_string(
                            component ),
                        " is not linked to any geological collection" ) );
            }
        }
    }
} // namespace

namespace geode
{
    namespace internal
    {
        template <>
        bool opengeode_inspector_geosciences_inspection_api
            model_geometrical_components_are_linked_to_geology(
                const CrossSection& model )
        {
            return components_are_linked_to_geology_base( model );
        }

        template <>
        bool opengeode_inspector_geosciences_inspection_api
            model_geometrical_components_are_linked_to_geology(
                const StructuralModel& model )
        {
            return ::components_are_linked_to_geology_base( model )
                   && ::components_are_linked_to_geology(
                       model, model.blocks() );
        }

        template <>
        void opengeode_inspector_geosciences_inspection_api
            add_model_geometrical_components_not_linked_to_geology(
                const CrossSection& model,
                InspectionIssues< ComponentID >& issues )
        {
            add_components_not_linked_to_geology(
                model, issues, model.lines() );
            add_components_not_linked_to_geology(
                model, issues, model.surfaces() );
        }

        template <>
        void opengeode_inspector_geosciences_inspection_api
            add_model_geometrical_components_not_linked_to_geology(
                const StructuralModel& model,
                InspectionIssues< ComponentID >& issues )
        {
            add_components_not_linked_to_geology(
                model, issues, model.surfaces() );
            ::add_components_not_linked_to_geology(
                model, issues, model.blocks() );
        }

        template < typename Model >
        bool opengeode_inspector_geosciences_inspection_api
            model_geological_components_are_linked_to_geometry(
                const Model& model )
        {
            for( const auto& fault_block : model.fault_blocks() )
            {
                if( model.nb_items( fault_block.id() ) == 0 )
                {
                    return false;
                }
            }
            for( const auto& fault : model.faults() )
            {
                if( model.nb_items( fault.id() ) == 0 )
                {
                    return false;
                }
            }
            for( const auto& horizon : model.horizons() )
            {
                if( model.nb_items( horizon.id() ) == 0 )
                {
                    return false;
                }
            }
            for( const auto& stratigraphic_unit : model.stratigraphic_units() )
            {
                if( model.nb_items( stratigraphic_unit.id() ) == 0 )
                {
                    return false;
                }
            }
            return true;
        }

        template < typename Model >
        void opengeode_inspector_geosciences_inspection_api
            add_model_geological_components_not_linked_to_geometry(
                const Model& model, InspectionIssues< ComponentID >& issues )
        {
            for( const auto& fault_block : model.fault_blocks() )
            {
                if( model.nb_items( fault_block.id() ) == 0 )
                {
                    issues.add_issue( fault_block.component_id(),
                        absl::StrCat( "FaultBlock ",
                            component_identification_to_string( fault_block ),
                            " is not linked to any geometrical collection" ) );
                }
            }
            for( const auto& fault : model.faults() )
            {
                if( model.nb_items( fault.id() ) == 0 )
                {
                    issues.add_issue( fault.component_id(),
                        absl::StrCat( "Fault ",
                            component_identification_to_string( fault ),
                            " is not linked to any geometrical collection" ) );
                }
            }
            for( const auto& horizon : model.horizons() )
            {
                if( model.nb_items( horizon.id() ) == 0 )
                {
                    issues.add_issue( horizon.component_id(),
                        absl::StrCat( "Horizon ",
                            component_identification_to_string( horizon ),
                            " is not linked to any geometrical collection" ) );
                }
            }
            for( const auto& stratigraphic_unit : model.stratigraphic_units() )
            {
                if( model.nb_items( stratigraphic_unit.id() ) == 0 )
                {
                    issues.add_issue( stratigraphic_unit.component_id(),
                        absl::StrCat( "StratigraphicUnit ",
                            component_identification_to_string(
                                stratigraphic_unit ),
                            " is not linked to any geometrical collection" ) );
                }
            }
        }

        template < index_t dimension >
        bool is_geological_collection( const ComponentType& component_type )
        {
            return component_type == Fault< dimension >::component_type_static()
                   || component_type
                          == FaultBlock< dimension >::component_type_static()
                   || component_type
                          == StratigraphicUnit<
                              dimension >::component_type_static()
                   || component_type
                          == Horizon< dimension >::component_type_static();
        }

        template bool opengeode_inspector_geosciences_inspection_api
            is_geological_collection< 2 >(
                const ComponentType& component_type );
        template bool opengeode_inspector_geosciences_inspection_api
            is_geological_collection< 3 >(
                const ComponentType& component_type );

        template bool opengeode_inspector_geosciences_inspection_api
            model_geological_components_are_linked_to_geometry(
                const StructuralModel& model );
        template bool opengeode_inspector_geosciences_inspection_api
            model_geological_components_are_linked_to_geometry(
                const CrossSection& model );
        template void opengeode_inspector_geosciences_inspection_api
            add_model_geological_components_not_linked_to_geometry(
                const StructuralModel& model,
                InspectionIssues< ComponentID >& issues );
        template void opengeode_inspector_geosciences_inspection_api
            add_model_geological_components_not_linked_to_geometry(
                const CrossSection& model,
                InspectionIssues< ComponentID >& issues );
    } // namespace internal
} // namespace geode