#
# Copyright (c) 2019 - 2025 Geode-solutions. All rights reserved.
#

"""OpenGeode-Inspector_Geosciences inspection: inspectors checking the geology
of the structural models and cross sections of OpenGeode-Geosciences (links
between geometrical and geological components, horizons), together with the
inspection of their BRep or Section, returning the issues found. Importing this
module initializes the library."""

import opengeode
import opengeode_geosciences
import opengeode_inspector

from opengeode_inspector_geosciences_py_inspection import *
OpenGeodeInspectorGeosciencesInspectionLibrary.initialize()
