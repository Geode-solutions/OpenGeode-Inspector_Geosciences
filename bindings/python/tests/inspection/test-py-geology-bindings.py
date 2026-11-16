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

"""Conventions of the geosciences inspection bindings: the module imports on
its own, the classes are CamelCase, every binding has named arguments and a
docstring."""

import inspect
import re
import subprocess
import sys

import geode_test_env

import opengeode_inspector_geosciences_py_inspection as inspection

GENERATED_ARG = re.compile(r"\barg\d+\b")
CAMEL_CASE = re.compile(r"^[A-Z][A-Za-z0-9]*$")
EXEMPT_DOC = {"__str__", "__repr__", "__init_subclass__", "__subclasshook__"}


def check(condition, message):
    if not condition:
        raise ValueError(f"[Test] {message}")


def test_module_import_alone():
    result = subprocess.run(
        [
            sys.executable,
            "-c",
            "import geode_test_env; "
            "import opengeode_inspector_geosciences_py_inspection as m; "
            "assert m.__doc__",
        ],
        capture_output=True,
        text=True,
    )
    check(
        result.returncode == 0,
        "module cannot be imported alone: " + result.stderr.strip()[-300:],
    )


def test_names():
    for name, member in vars(inspection).items():
        if name.startswith("_") or not inspect.isclass(member):
            continue
        check(CAMEL_CASE.match(name), f"class {name} should be CamelCase")


def check_doc(qualified_name, doc, problems, needs_description=True):
    doc = doc or ""
    for line in doc.splitlines():
        if GENERATED_ARG.search(line):
            problems.append(f"{qualified_name}: unnamed argument in {line.strip()}")
    description = [
        line for line in doc.splitlines() if line.strip() and "->" not in line
    ]
    if needs_description and not description:
        problems.append(f"{qualified_name}: no docstring")


def test_signatures_and_docstrings():
    problems = []
    check(inspection.__doc__, "no module docstring")
    for name, member in vars(inspection).items():
        if name.startswith("_"):
            continue
        qualified_name = f"{inspection.__name__}.{name}"
        if inspect.isclass(member):
            check_doc(qualified_name, member.__doc__, problems)
            for method_name, method in vars(member).items():
                if method_name.startswith("_") and not (
                    method_name.startswith("__") and method_name.endswith("__")
                ):
                    continue
                if isinstance(method, staticmethod):
                    method = method.__func__
                member_name = f"{qualified_name}.{method_name}"
                if isinstance(method, property):
                    check_doc(member_name, method.__doc__, problems)
                    for accessor in (method.fget, method.fset):
                        if accessor is not None:
                            check_doc(member_name, accessor.__doc__, problems, False)
                elif callable(method) and not inspect.isclass(method):
                    check_doc(
                        member_name,
                        getattr(method, "__doc__", ""),
                        problems,
                        method_name not in EXEMPT_DOC,
                    )
        elif callable(member):
            check_doc(qualified_name, member.__doc__, problems)
    check(not problems, "bad bindings:\n" + "\n".join(problems))


if __name__ == "__main__":
    test_module_import_alone()
    test_names()
    test_signatures_and_docstrings()
