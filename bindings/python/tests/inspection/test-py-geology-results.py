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

"""The inspection results of the geosciences inspections: construction, call
form of the repr, full report as str, and the static library initialization."""

import geode_test_env

import opengeode_inspector_geosciences_py_inspection as inspection

RESULTS = [
    "StructuralModelInspectionResult",
    "StructuralModelGeologyInspectionResult",
    "CrossSectionInspectionResult",
    "CrossSectionGeologyInspectionResult",
]


def check(condition, message):
    if not condition:
        raise ValueError(f"[Test] {message}")


def test_initialize_is_static():
    library = inspection.OpenGeodeInspectorGeosciencesInspectionLibrary
    check(
        isinstance(vars(library)["initialize"], staticmethod),
        "initialize should be static",
    )
    # No instance is needed, and calling it again has no effect
    library.initialize()
    library.initialize()


def test_results():
    for name in RESULTS:
        result = getattr(inspection, name)()
        check(result.nb_issues() == 0, f"{name}() should have no issue")
        check(repr(result) == f"{name}(nb_issues=0)", f"wrong repr {result!r}")
        check(str(result) == result.string(), f"{name} str is the report")
        check(result.inspection_type(), f"{name} has an inspection type")


if __name__ == "__main__":
    test_initialize_is_static()
    test_results()
