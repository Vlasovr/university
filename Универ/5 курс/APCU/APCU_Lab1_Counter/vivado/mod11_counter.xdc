## Template constraints for the laboratory project.
## Pin numbers must be replaced with the target board pins before hardware programming.

create_clock -period 10.000 -name CLOCK [get_ports CLOCK]

set_property IOSTANDARD LVCMOS33 [get_ports {CLOCK RST EN LOAD Q0 Q1 Q2 Q3 TC}]
