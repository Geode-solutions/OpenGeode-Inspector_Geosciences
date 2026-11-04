# -*- coding: utf-8 -*-
# Copyright (c) 2019 - 2025 Geode-solutions
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import geode_test_env
        
import opengeode as og
import opengeode_geosciences as ogg
import opengeode_inspector_geosciences_py_inspection as inspection


def data_dir():
    test_dir = os.path.dirname(__file__)
    return os.path.abspath(os.path.join(test_dir, "../../../../tests/data"))

def check_model_A2(model):
    inspector = inspection.StructuralModelInspector(model)
    result = inspector.inspect_structural_model()
    expected_nb_brep_issues = 99635
    if result.brep.nb_issues() != expected_nb_brep_issues:
        raise ValueError(
            f"[Test] modelA2 should have {expected_nb_brep_issues} brep issues, not {result.brep.nb_issues()}"
        )
    expected_nb_geology_issues = model.nb_blocks() * 2 + 2
    if result.geology.nb_issues() != expected_nb_geology_issues:
        print("[Test] Problematic issues:\n", result.geology.string())
        raise ValueError(
            f"[Test] modelA2 should have {expected_nb_geology_issues} geological issues, not {result.geology.nb_issues()}"
        )
    if str(result) != result.string():
        raise ValueError("[Test] str(result) should be the inspection report")
    expected_repr = f"<StructuralModelInspectionResult: {result.nb_issues()} issues>"
    if repr(result) != expected_repr:
        raise ValueError(f"[Test] repr(result) should be {expected_repr}, not {repr(result)}")
    expected_geology_repr = (
        f"<StructuralModelGeologyInspectionResult: {expected_nb_geology_issues} issues>"
    )
    if repr(result.geology) != expected_geology_repr:
        raise ValueError(
            f"[Test] repr(result.geology) should be {expected_geology_repr}, not {repr(result.geology)}"
        )
    if inspector.structural_model_geology_is_valid():
        raise ValueError("[Test] modelA2 geology should not be valid")


def check_implicit_model_v0(model_v0):
    v0_inspector = inspection.StructuralModelInspector(model_v0)
    v0_result = v0_inspector.inspect_structural_model()
    if v0_result.brep.nb_issues() != 0:
        raise ValueError("[Test] mss_from_implicit_modeling_v0 should have no brep issues")
    expected_nb_v0_issues = 10 + model_v0.nb_blocks()
    if v0_result.geology.nb_issues() != expected_nb_v0_issues:
        print("[Test] Problematic issues:\n", v0_result.geology.string())
        raise ValueError(
            f"[Test] mss_from_implicit_modeling_v0 should have {expected_nb_v0_issues} geological issues, not {v0_result.geology.nb_issues()}"
        )
    not_part_of_geology = v0_result.geology.components_not_part_of_geology
    if not_part_of_geology.nb_issues() != expected_nb_v0_issues:
        raise ValueError(
            "[Test] mss_from_implicit_modeling_v0 issues should be surfaces not part of any geological components, not:\n"
            + v0_result.geology.string()
        )
    issues = not_part_of_geology.issues()
    if len(issues) != expected_nb_v0_issues:
        raise ValueError(
            f"[Test] issues() should return {expected_nb_v0_issues} components, not {len(issues)}"
        )
    for component_id in issues:
        if component_id.type not in (
            og.Surface3D.component_type_static(),
            og.Block3D.component_type_static(),
        ):
            raise ValueError(
                f"[Test] Unexpected component type in issues: {component_id.type}"
            )
    if v0_inspector.geometrical_components_are_linked_to_geology():
        raise ValueError(
            "[Test] mss_from_implicit_modeling_v0 geometrical components should not be linked to geology"
        )
    if not v0_inspector.geological_components_are_linked_to_geometry():
        raise ValueError(
            "[Test] mss_from_implicit_modeling_v0 geological components should be linked to geometry"
        )
    if not v0_inspector.horizons_are_valid():
        raise ValueError("[Test] mss_from_implicit_modeling_v0 horizons should be valid")


def check_implicit_model_v1(model_v1):
    v1_inspector = inspection.StructuralModelInspector(model_v1)
    v1_result = v1_inspector.inspect_structural_model()
    if v1_result.brep.nb_issues() != 0:
        raise ValueError("[Test] mss_from_implicit_modeling_v1 should have no brep issues")
    expected_nb_v1_issues = model_v1.nb_blocks()
    if v1_result.geology.nb_issues() != expected_nb_v1_issues:
        print("[Test] Problematic issues:\n", v1_result.geology.string())
        raise ValueError(
            f"[Test] mss_from_implicit_modeling_v1 should have {expected_nb_v1_issues} geological issues, not {v1_result.geology.nb_issues()}"
        )

    builder = ogg.StructuralModelBuilder(model_v1)
    fault_blocks = {}
    for name in ("Region_1", "Region_2"):
        fault_block = model_v1.fault_block(builder.add_fault_block())
        builder.set_fault_block_name(fault_block, name)
        fault_blocks[name] = fault_block
    for block in model_v1.blocks():
        name = block.name()
        if name in fault_blocks:
            builder.add_block_in_fault_block(block, fault_blocks[name])
    fixed_result = inspection.inspect_structural_model_geology(model_v1)
    if fixed_result.nb_issues() != 0:
        raise ValueError(
            f"[Test] mss_from_implicit_modeling_v1 should have no geological issues after fix, not:\n{fixed_result}"
        )
    geology_inspector = inspection.StructuralModelGeologyInspector(model_v1)
    for inspector in (geology_inspector, v1_inspector):
        if not inspector.structural_model_geology_is_valid():
            raise ValueError("[Test] Fixed model geology should be valid")
        if not inspector.geometrical_components_are_linked_to_geology():
            raise ValueError("[Test] Fixed model geometrical components should be linked to geology")
        if not inspector.geological_components_are_linked_to_geometry():
            raise ValueError("[Test] Fixed model geological components should be linked to geometry")
        if not inspector.horizons_are_valid():
            raise ValueError("[Test] Fixed model horizons should be valid")

    empty_fault = builder.add_fault(ogg.Fault3D.FAULT_TYPE.NORMAL)
    if v1_inspector.structural_model_geology_is_valid():
        raise ValueError("[Test] Model with an empty fault should not be valid")
    if v1_inspector.geological_components_are_linked_to_geometry():
        raise ValueError("[Test] Empty fault should not be linked to geometry")
    empty_result = v1_inspector.inspect_structural_model_geology()
    issues = empty_result.empty_geological_components.issues()
    if len(issues) != 1 or issues[0].id.string() != empty_fault.string():
        raise ValueError(
            f"[Test] Empty fault should be the only empty geological component, not:\n{empty_result}"
        )


def check_cross_section():
    cross_section = ogg.CrossSection()
    result = inspection.inspect_cross_section(cross_section)
    if result.nb_issues() != 0:
        raise ValueError(f"[Test] Empty CrossSection should have no issues, not:\n{result}")
    if result.section.nb_issues() != 0 or result.geology.nb_issues() != 0:
        raise ValueError(f"[Test] Empty CrossSection should have no section or geology issues, not:\n{result}")
    if repr(result) != "<CrossSectionInspectionResult: 0 issues>":
        raise ValueError(f"[Test] Wrong CrossSectionInspectionResult repr: {repr(result)}")
    inspector = inspection.CrossSectionInspector(cross_section)
    if not inspector.cross_section_geology_is_valid():
        raise ValueError("[Test] Empty CrossSection geology should be valid")

    builder = ogg.CrossSectionBuilder(cross_section)
    horizon = builder.add_horizon()
    line = builder.add_line()
    geology_inspector = inspection.CrossSectionGeologyInspector(cross_section)
    if geology_inspector.geological_components_are_linked_to_geometry():
        raise ValueError("[Test] Empty horizon should not be linked to geometry")
    if geology_inspector.geometrical_components_are_linked_to_geology():
        raise ValueError("[Test] Line should not be linked to geology")
    geology_result = inspection.inspect_cross_section_geology(cross_section)
    if geology_result.nb_issues() != 2:
        raise ValueError(f"[Test] CrossSection should have 2 geological issues, not:\n{geology_result}")
    empty = geology_result.empty_geological_components.issues()
    if len(empty) != 1 or empty[0].id.string() != horizon.string():
        raise ValueError(f"[Test] Horizon should be reported as empty, not:\n{geology_result}")
    not_part = geology_result.components_not_part_of_geology.issues()
    if len(not_part) != 1 or not_part[0].id.string() != line.string():
        raise ValueError(f"[Test] Line should be reported as not part of geology, not:\n{geology_result}")

    builder.add_line_in_horizon(cross_section.line(line), cross_section.horizon(horizon))
    surface = builder.add_surface()
    fault_block = builder.add_fault_block()
    builder.add_surface_in_fault_block(
        cross_section.surface(surface), cross_section.fault_block(fault_block)
    )
    builder.add_corner()
    if not geology_inspector.geometrical_components_are_linked_to_geology():
        raise ValueError("[Test] Corners are not required to be linked to geology")
    linked_result = inspection.inspect_cross_section_geology(cross_section)
    if linked_result.components_not_part_of_geology.nb_issues() != 0:
        raise ValueError(f"[Test] No component should be reported as not part of geology, not:\n{linked_result}")


if __name__ == "__main__":
    modelA2 = ogg.load_structural_model(data_dir() + "/modelA2.og_strm")
    check_model_A2(modelA2)
    model_v0 = ogg.load_implicit_structural_model(
        data_dir() + "/mss_from_implicit_modeling_v0.og_istrm"
    )
    check_implicit_model_v0(model_v0)
    model_v1 = ogg.load_implicit_structural_model(
        data_dir() + "/mss_from_implicit_modeling_v1.og_istrm"
    )
    check_implicit_model_v1(model_v1)
    check_cross_section()
