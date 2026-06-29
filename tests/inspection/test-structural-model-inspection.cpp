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

#include <geode/tests_config.hpp>

#include <geode/basic/assert.hpp>
#include <geode/basic/logger.hpp>

#include <geode/geosciences/explicit/representation/core/structural_model.hpp>
#include <geode/geosciences/explicit/representation/io/structural_model_input.hpp>
#include <geode/geosciences/implicit/representation/core/implicit_structural_model.hpp>
#include <geode/geosciences/implicit/representation/io/implicit_structural_model_input.hpp>

#include <geode/inspector_geosciences/inspection/structural_model_inspection.hpp>

void check_model_A2()
{
    const auto model = geode::load_structural_model(
        absl::StrCat( geode::DATA_PATH, "modelA2.og_strm" ) );
    const geode::StructuralModelInspector inspector{ model };
    const auto result = inspector.inspect_structural_model();
    const geode::index_t expected_nb_brep_issues{ 87952 };
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        result.brep.nb_issues() == expected_nb_brep_issues,
        "[Test] modelA2 should have ", expected_nb_brep_issues,
        " brep issues, not ", result.brep.nb_issues() );
    const geode::index_t expected_nb_geology_issues{ 14 };
    if( result.geology.nb_issues() != expected_nb_geology_issues )
    {
        geode::Logger::debug(
            "[Test] Problematic issues:\n", result.geology.string() );
    }
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        result.geology.nb_issues() == expected_nb_geology_issues,
        "[Test] modelA2 should have ", expected_nb_geology_issues,
        " geological issues, not ", result.geology.nb_issues() );
}

void check_implicit_model()
{
    const auto model_v0 = geode::load_implicit_structural_model( absl::StrCat(
        geode::DATA_PATH, "mss_from_implicit_modeling_v0.og_istrm" ) );
    const geode::StructuralModelInspector v0_inspector{ model_v0 };
    const auto v0_result = v0_inspector.inspect_structural_model();
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        v0_result.brep.nb_issues() == 0,
        "[Test] mss_from_implicit_modeling_v0 should have no brep issues" );
    const geode::index_t expected_nb_v0_issues{ 10 };
    if( v0_result.geology.nb_issues() != expected_nb_v0_issues )
    {
        geode::Logger::debug(
            "[Test] Problematic issues:\n", v0_result.geology.string() );
    }
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        v0_result.geology.nb_issues() == expected_nb_v0_issues,
        "[Test] mss_from_implicit_modeling_v0 should have ",
        expected_nb_v0_issues, " geological issues, not ",
        v0_result.geology.nb_issues() );
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        v0_result.geology.components_not_part_of_geology.nb_issues()
            == expected_nb_v0_issues,
        "[Test] mss_from_implicit_modeling_v0 issues should be surfaces not "
        "part of any geological components, not:\n",
        v0_result.geology.string() );
    const auto model_v1 = geode::load_implicit_structural_model( absl::StrCat(
        geode::DATA_PATH, "mss_from_implicit_modeling_v1.og_istrm" ) );
    const geode::StructuralModelInspector v1_inspector{ model_v1 };
    const auto v1_result = v1_inspector.inspect_structural_model();
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        v1_result.brep.nb_issues() == 0,
        "[Test] mss_from_implicit_modeling_v1 should have no brep issues" );
    const geode::index_t expected_nb_v1_issues{ 0 };
    if( v1_result.geology.nb_issues() != expected_nb_v1_issues )
    {
        geode::Logger::debug(
            "[Test] Problematic issues:\n", v1_result.geology.string() );
    }
    geode::OpenGeodeInspectorGeosciencesInspectionException::test(
        v1_result.geology.nb_issues() == expected_nb_v1_issues,
        "[Test] mss_from_implicit_modeling_v1 should have ",
        expected_nb_v1_issues, " geological issues, not ",
        v1_result.geology.nb_issues() );
}

int main()
{
    try
    {
        geode::OpenGeodeInspectorGeosciencesInspectionLibrary::initialize();
        geode::Logger::set_level( geode::Logger::LEVEL::debug );

        check_model_A2();
        check_implicit_model();

        geode::Logger::info( "TEST SUCCESS" );
        return 0;
    }
    catch( ... )
    {
        return geode::geode_lippincott();
    }
}
