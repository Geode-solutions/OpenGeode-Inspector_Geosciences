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

"""The geosciences inspections release the GIL: the main thread runs while an
inspection is in progress in another thread."""

import os

import geode_test_env

import opengeode as og
import opengeode_geosciences as ogg
import opengeode_inspector_geosciences_py_inspection as inspection


def check(condition, message):
    if not condition:
        raise ValueError(f"[Test] {message}")


def data_dir():
    return os.path.abspath(
        os.path.join(os.path.dirname(__file__), "../../../../tests/data")
    )


def check_releases_gil(name, call):
    geode_test_env.assert_gil_released(call, name=name)


def test_structural_model():
    model = og.load_one(
        ogg.load_structural_model(data_dir() + "/modelA2.og_strm"),
        data_dir() + "/modelA2.og_strm",
    )
    check_releases_gil(
        "inspect_structural_model", lambda: inspection.inspect_structural_model(model)
    )
    check_releases_gil(
        "StructuralModelInspector.__init__",
        lambda: inspection.StructuralModelInspector(model),
    )
    inspector = inspection.StructuralModelInspector(model)
    check_releases_gil(
        "StructuralModelInspector.inspect_structural_model",
        inspector.inspect_structural_model,
    )
    check_releases_gil(
        "inspect_structural_model_geology",
        lambda: inspection.inspect_structural_model_geology(model),
    )
    geology = inspection.StructuralModelGeologyInspector(model)
    check_releases_gil(
        "structural_model_geology_is_valid",
        geology.structural_model_geology_is_valid,
    )
    check_releases_gil(
        "StructuralModelGeologyInspector.inspect_structural_model_geology",
        geology.inspect_structural_model_geology,
    )


def test_cross_section():
    cross_section = ogg.CrossSection()
    builder = ogg.CrossSectionBuilder(cross_section)
    for _ in range(5):
        builder.add_horizon()
    check_releases_gil(
        "inspect_cross_section", lambda: inspection.inspect_cross_section(cross_section)
    )
    check_releases_gil(
        "inspect_cross_section_geology",
        lambda: inspection.inspect_cross_section_geology(cross_section),
    )
    geology = inspection.CrossSectionGeologyInspector(cross_section)
    check_releases_gil(
        "cross_section_geology_is_valid", geology.cross_section_geology_is_valid
    )


if __name__ == "__main__":
    test_structural_model()
    test_cross_section()
