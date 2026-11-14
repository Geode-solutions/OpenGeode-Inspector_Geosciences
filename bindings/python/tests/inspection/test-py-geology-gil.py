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

"""The geosciences inspections release the GIL: another Python thread runs while
an inspection of a large model is in progress."""

import os
import sys
import threading

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


# The checks are deterministic thanks to a huge interpreter switch interval:
# the interpreter never forces a GIL hand-over, so a thread keeps the GIL until
# it blocks or exits. If a bound call holds the GIL, the worker thread runs the
# whole work (and `finished.set()`) before the main thread can run. If the call
# releases the GIL, the main thread is scheduled as soon as the call is inside
# C++ and sees the work still running. Holding the GIL can therefore never
# pass; the only possible flakiness is the work finishing before the main
# thread wakes up, hence the work repeats the call and ATTEMPTS retries are
# allowed.
TIMEOUT = 120
ATTEMPTS = 6
REPETITIONS = 5


class HugeSwitchInterval:
    def __enter__(self):
        self.previous = sys.getswitchinterval()
        sys.setswitchinterval(1000)

    def __exit__(self, *arguments):
        sys.setswitchinterval(self.previous)


def run_in_worker(work):
    """Run work in a thread, return True if the main thread could run before
    the work finished"""
    ready = threading.Event()
    finished = threading.Event()
    box = {}

    def target():
        ready.set()
        try:
            work()
        except BaseException as error:
            box["error"] = error
        finished.set()

    thread = threading.Thread(target=target)
    thread.start()
    check(ready.wait(TIMEOUT), "the worker thread did not start")
    main_ran_during_work = not finished.is_set()
    thread.join(TIMEOUT)
    if "error" in box:
        raise box["error"]
    return main_ran_during_work


def check_releases_gil(name, call):
    def work():
        for _ in range(REPETITIONS):
            call()

    with HugeSwitchInterval():
        for _ in range(ATTEMPTS):
            if run_in_worker(work):
                return
    raise ValueError(f"[Test] {name} kept the GIL during the whole call")


def test_structural_model():
    model = og.load_one(
        ogg.load_structural_model(data_dir() + "/modelA2.og_strm"),
        data_dir() + "/modelA2.og_strm",
    )
    check_releases_gil(
        "inspect_structural_model", lambda: inspection.inspect_structural_model(model)
    )
    check_releases_gil(
        "StructuralModelInspector",
        lambda: inspection.StructuralModelInspector(model).inspect_structural_model(),
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
    for _ in range(2000):
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
