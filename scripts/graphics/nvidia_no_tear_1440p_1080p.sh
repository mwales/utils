#!/bin/bash

# Configures my GTX 1060 for a mode that reduces screen tearing

# Use arandr to get the display IDs easily (DP-2, DVI-D-0)

# --------
# |      |----
# | 1440 |   | 1080p
# |      |----
# --------

nvidia-settings --assign CurrentMetaMode="DP-2:2560x1440_60 +0+0 { ForceFullCompositionPipeline = On }, DVI-D-0:1920x1080_60 +2560+180 { ForceFullCompositionPipeline = On }"


