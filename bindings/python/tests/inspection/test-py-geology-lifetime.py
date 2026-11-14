# Copyright (c) 2019 - 2026 Geode-solutions
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

"""The inspectors keep their model alive, and the model inspectors check the
unique vertices and the components given to the methods they inherit from the
BRep and Section inspectors."""

import gc
import weakref

import geode_test_env

import opengeode as og
import opengeode_geosciences as ogg
import opengeode_inspector_geosciences_py_inspection as inspection

MODEL_INSPECTORS = [
    ("StructuralModelGeologyInspector", ogg.StructuralModel),
    ("StructuralModelInspector", ogg.StructuralModel),
    ("CrossSectionGeologyInspector", ogg.CrossSection),
    ("CrossSectionInspector", ogg.CrossSection),
]
# Inherited methods taking only a unique vertex index
UNIQUE_VERTEX_METHODS = [
    (
        "StructuralModelInspector",
        ogg.StructuralModel,
        [
            "brep_corner_topology_is_valid",
            "unique_vertex_has_multiple_corners",
            "corner_is_multiply_embedded",
            "corner_is_not_internal_nor_boundary",
            "corner_is_part_of_line_but_not_boundary",
        ],
    ),
    (
        "CrossSectionInspector",
        ogg.CrossSection,
        [
            "section_corner_topology_is_valid",
            "unique_vertex_has_multiple_corners",
            "corner_has_multiple_embeddings",
            "corner_is_not_internal_nor_boundary",
            "vertex_is_part_of_invalid_single_line",
        ],
    ),
]


def check(condition, message):
    if not condition:
        raise ValueError(f"[Test] {message}")


def test_lifetime():
    for name, model_type in MODEL_INSPECTORS:
        model = model_type()
        reference = weakref.ref(model)
        instance = getattr(inspection, name)(model)
        del model
        gc.collect()
        check(reference() is not None, f"{name} does not keep its model alive")
        del instance
        gc.collect()
        check(reference() is None, f"{name} still holds its model when destroyed")


def test_unique_vertex_check():
    for name, model_type, methods in UNIQUE_VERTEX_METHODS:
        inspector = getattr(inspection, name)(model_type())
        for method in methods:
            for index in [0, 1, 4294967295]:
                try:
                    getattr(inspector, method)(index)
                except IndexError:
                    continue
                raise ValueError(
                    f"[Test] {name}.{method}({index}) on an empty model "
                    "should raise IndexError"
                )


# Inherited methods taking a component of the model
COMPONENT_METHODS = [
    ("StructuralModelInspector", ogg.StructuralModel, ogg.StructuralModelBuilder),
    ("CrossSectionInspector", ogg.CrossSection, ogg.CrossSectionBuilder),
]


def test_foreign_component_check():
    for name, model_type, builder_type in COMPONENT_METHODS:
        model = model_type()
        builder = builder_type(model)
        own = model.line(builder.add_line())
        other = model_type()
        foreign = other.line(builder_type(other).add_line())
        inspector = getattr(inspection, name)(model)
        check(inspector.line_is_meshed(own) is False, f"{name}: own Line")
        for method in ["line_is_meshed", "line_vertices_are_associated_to_unique_vertices"]:
            try:
                getattr(inspector, method)(foreign)
            except ValueError as error:
                check(
                    "not a component of the inspected model" in str(error),
                    f"{name}.{method}: {error}",
                )
                continue
            raise ValueError(
                f"[Test] {name}.{method} should reject a Line of another model"
            )


if __name__ == "__main__":
    test_lifetime()
    test_unique_vertex_check()
    test_foreign_component_check()
