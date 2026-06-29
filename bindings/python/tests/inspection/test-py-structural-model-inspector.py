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
import sys
import platform
if sys.version_info >= (3, 8, 0) and platform.system() == "Windows":
    for path in [x.strip() for x in os.environ['PATH'].split(';') if x]:
        os.add_dll_directory(path)
        
import opengeode as og
import opengeode_geosciences as ogg
import opengeode_inspector_geosciences_py_inspection as inspection


def data_dir():
    test_dir = os.path.dirname(__file__)
    return os.path.abspath(os.path.join(test_dir, "../../../../tests/data"))

def check_model_A2(model):
    inspector = inspection.StructuralModelInspector( model )
    result = inspector.inspect_structural_model()
    expected_nb_brep_issues = 87952
    if result.brep.nb_issues() != expected_nb_brep_issues:
        raise ValueError("[Test] modelA2 should have " + str(expected_nb_brep_issues) + " brep issues, not ", str(result.brep.nb_issues()) )
    expected_nb_geology_issues = 14
    if result.geology.nb_issues() != expected_nb_geology_issues:
        print( "[Test] Problematic issues:\n", result.geology.string() )
        raise ValueError("[Test] modelA2 should have "+ str(expected_nb_geology_issues)+ " geological issues, not "+ str(result.geology.nb_issues()) )
    

def check_implicit_model_v0(model_v0):
    v0_inspector = inspection.StructuralModelInspector( model_v0 )
    v0_result = v0_inspector.inspect_structural_model()
    if v0_result.brep.nb_issues() != 0:
        raise ValueError("[Test] mss_from_implicit_modeling_v0 should have no brep issues" )
    expected_nb_v0_issues= 10
    if v0_result.geology.nb_issues() != expected_nb_v0_issues:
        print( "[Test] Problematic issues:\n", v0_result.geology.string() )
    if v0_result.geology.nb_issues() != expected_nb_v0_issues:
        raise ValueError("[Test] mss_from_implicit_modeling_v0 should have "+expected_nb_v0_issues+ " geological issues, not "+v0_result.geology.nb_issues() )
    if v0_result.geology.components_not_part_of_geology.nb_issues() != expected_nb_v0_issues:
        raise ValueError("[Test] mss_from_implicit_modeling_v0 issues should be surfaces not part of any geological components, not:\n"+ v0_result.geology.string() )

def check_implicit_model_v1(model_v1):
    v1_inspector = inspection.StructuralModelInspector( model_v1 )
    v1_result = v1_inspector.inspect_structural_model()
    if v1_result.brep.nb_issues() != 0:
        raise ValueError("[Test] mss_from_implicit_modeling_v1 should have no brep issues" )
    expected_nb_v1_issues =  0
    if v1_result.geology.nb_issues() != expected_nb_v1_issues:
        print( "[Test] Problematic issues:\n", v1_result.geology.string() )
    if v1_result.geology.nb_issues() != expected_nb_v1_issues:
        raise ValueError("[Test] mss_from_implicit_modeling_v1 should have "+ expected_nb_v1_issues+ " geological issues, not "+ v1_result.geology.nb_issues() )

if __name__ == '__main__':
    modelA2=ogg.load_structural_model(data_dir() + "/modelA2.og_strm" )
    check_model_A2(modelA2)
    model_v0 = ogg.load_implicit_structural_model( data_dir()+ "/mss_from_implicit_modeling_v0.og_istrm" )
    check_implicit_model_v0(model_v0)
    model_v1 = ogg.load_implicit_structural_model( data_dir()+ "/mss_from_implicit_modeling_v1.og_istrm" )
    check_implicit_model_v1(model_v1)