help(
[[
The binary is called showTip.  You can do:

   $ showTip -n 14

to see tip 14 again.

To show all tips do:

   $ showTip -a

To disable tips at login do:

   $ touch ~/.no.tips

Version 1.0
]]
)

whatis("Name: Tacc Tips")
whatis("Version: 1.0")
whatis("Category: User training")
whatis("Keywords: Training ")
whatis("URL: http://tacc.utexas.edu")
whatis("Description: Tips generated at each login.")


prepend_path("PATH",          "/opt/apps/HPC_Tips/HPC_Tips/bin")
setenv (     "TACC_TIPS_DIR", "/opt/apps/HPC_Tips/HPC_Tips/")
setenv (     "TACC_TIPS_BIN", "/opt/apps/HPC_Tips/HPC_Tips/bin")
