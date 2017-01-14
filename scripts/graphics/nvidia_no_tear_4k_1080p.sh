#!/bin/bash

# Configures my GTX 1060 for a mode that reduces screen tearing
#
# Use arandr to get the display IDs easily (DP-2, DVI-D-0)
#
# --------
# |      |----
# |  4k  |   | 1080p
# |      |----
# --------

nvidia-settings --assign CurrentMetaMode="DP-2:3840x2160_60 +0+0 { ForceFullCompositionPipeline = On }, DVI-D-0:1920x1080_60 +3840+540 { ForceFullCompositionPipeline = On }"


