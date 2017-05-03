"""
===========
Radar chart practice
===========

Attempting to use matplotlib to continuously
update a radar chart
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.path import Path
from matplotlib.spines import Spine
from matplotlib.projections.polar import PolarAxes
from matplotlib.projections import register_projection

def radar_factory(num_vars, frame='circle'):
    """
    Create a radar chart, circular, with num_vars axes.
    
    Parameters
    ----------
    num_vars : int
        Number of variables for the radar chart
    frame : {'circle' | 'polygon'}
        Shape of frame surrounding axes.
    """
    # calculates evenly spaced axis angles
    theta = np.linspace(0, 2*np.pi, num_vars, endpoint=False)
    # rotate theta such that the first axis is at the top
    theta += np.pi/2
