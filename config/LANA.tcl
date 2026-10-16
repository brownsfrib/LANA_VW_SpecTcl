#  SpecTclGUI save file created Thu Aug 13 11:50:58 EST 2026
#  SpecTclGui Version: 1.0
#      Author: Ron Fox (fox@nscl.msu.edu)

#Tree params:


# Pseudo parameter definitions


# Tree variable definitions:

treevariable -set offset.00 1000 {}
treevariable -set offset.01 1000 {}
treevariable -set offset.02 1000 {}
treevariable -set offset.03 1000 {}
treevariable -set offset.04 1000 {}
treevariable -set offset.05 1000 {}
treevariable -set offset.06 1000 {}
treevariable -set offset.07 1000 {}
treevariable -set offset.08 1000 {}
treevariable -set offset.09 1000 {}
treevariable -set offset.10 1000 {}
treevariable -set offset.11 1000 {}
treevariable -set offset.12 1000 {}
treevariable -set offset.13 1000 {}
treevariable -set offset.14 1000 {}
treevariable -set offset.15 1000 {}
treevariable -set slope.00 0.5 {}
treevariable -set slope.01 0.5 {}
treevariable -set slope.02 0.5 {}
treevariable -set slope.03 0.5 {}
treevariable -set slope.04 0.5 {}
treevariable -set slope.05 0.5 {}
treevariable -set slope.06 0.5 {}
treevariable -set slope.07 0.5 {}
treevariable -set slope.08 0.5 {}
treevariable -set slope.09 0.5 {}
treevariable -set slope.10 0.5 {}
treevariable -set slope.11 0.5 {}
treevariable -set slope.12 0.5 {}
treevariable -set slope.13 0.5 {}
treevariable -set slope.14 0.5 {}
treevariable -set slope.15 0.5 {}

# Gate definitions in reverse dependency order
 

# Spectrum Definitions

#spectrum NWB.00.timeDiff 1 NWA.00.timeDiff {{-30.000000 30.000000 2048}} long
# fuck tcl this shit sucks
# we have to start at 6, because, remember when I dropped a ladder on nwa
# to be fair other people broke some bars but like
# dropping a ladder on them is really bad
# the only thing that made it kinda redeeming is that it happened on 9/11? 
# so I guess not the worst thing to happen on that date
# I also broke two bars when the ladder dropped so
# you know there's a joke in there somewhere

# Setting NWA time idfference spectra
for {set i 6} {$i <= 21} {incr i} {
	set b [format %02d $i]
	spectrum NWA.$b.timeDiff 1 NWA.$b.timeDiff {{-30.00 30.00 2048}}
}

# setting NWB time difference spectra
for {set i 0} {$i <= 24} {incr i} {
	set b [format %02d $i]
	spectrum NWB.$b.timeDiff 1 NWB.$b.timeDiff {{-30.00 30.00 2048}}
}
sbind -all

# Gate Applications: 


#  filter definitions: ALL FILTERS ARE DISABLED!!!!!!!

# Write metadata

#  - Parameter tab layout: 

set parameter(select) 1
set parameter(Array)  false

#-- Variable tab layout

set variable(select) 1
set variable(Array)  0
