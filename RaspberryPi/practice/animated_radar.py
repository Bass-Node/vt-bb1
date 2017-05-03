import numpy as np
from matplotlib.pylab import *
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.axes_grid1 import host_subplot
import mpl_toolkits.axisartist.angle_helper as angle_helper
from matplotlib.projections import PolarAxes
from matplotlib.transforms import Affine2D
from mpl_toolkits.axisartist import SubplotHost
from mpl_toolkits.axisartist import GridHelperCurveLinear


#--- Setup radar display ---
global ax1

# rotate a bit for better orientation
tr_rotate = Affine2D().translate(90, 0)

# see demo_curvelinear_grid.py for details
tr = tr_rotate + Affine2D().scale(np.pi / 180., 1.) + PolarAxes.PolarTransform()

extreme_finder = angle_helper.ExtremeFinderCycle(20,
                                                 20,
                                                 lon_cycle=360,
                                                 lat_cycle=None,
                                                 lon_minmax=None,
                                                 lat_minmax=(0, np.inf),
                                                 )


grid_locator1 = angle_helper.LocatorDMS(12)

tick_formatter1 = angle_helper.FormatterDMS()

grid_helper = GridHelperCurveLinear(tr,
                                    extreme_finder=extreme_finder,
                                    grid_locator1=grid_locator1,
                                    tick_formatter1=tick_formatter1,
                                    )

ax1 = SubplotHost(fig, 1, 1, 1, grid_helper=grid_helper)

fig.add_subplot(ax1)

# Now creates floating axis

# floating axis whose first coordinate (theta) is fixed at 0
ax1.axis["lat"] = axis = ax1.new_floating_axis(0, 0)
# axis.label.set_text(r"$\theta = 60^{\circ}$")
axis.major_ticklabels.set_axis_direction("right")
axis.label.set_visible(True)

# floating axis whose second coordinate (r) is fixed at 6
ax1.axis["top"].major_ticklabels.set_visible(True)

ax1.set_aspect(1.)
ax1.set_xlim(-15, 15)
ax1.set_ylim(0, 20)
ax1.grid(True)