## Template constraints for the laboratory project.
## Pin numbers must be replaced with the target board pins before hardware programming.

create_clock -period 10.000 -name CLOCK [get_ports CLOCK]

set_property IOSTANDARD LVCMOS33 [get_ports {CLOCK RST START DONE BUSY OK BUS_D0 BUS_D1 BUS_D2 BUS_D3 BUS_D4 BUS_D5 BUS_D6 BUS_D7}]
