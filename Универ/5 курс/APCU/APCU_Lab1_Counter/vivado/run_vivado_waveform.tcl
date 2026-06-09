set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "mod11_counter_vivado"]

if {![file exists [file join $project_dir "mod11_counter.xpr"]]} {
  create_project mod11_counter $project_dir -part xc7a35tcpg236-1 -force
  set_property target_language VHDL [current_project]

  add_files [file join $root_dir "src" "mod11_counter.vhd"]
  add_files [file join $root_dir "src" "mod11_counter_rtl_view_top.vhd"]
  add_files -fileset sim_1 [file join $root_dir "tb" "tb_mod11_counter.vhd"]
  add_files -fileset constrs_1 [file join $script_dir "mod11_counter.xdc"]
} else {
  open_project [file join $project_dir "mod11_counter.xpr"]
}

set_property top mod11_counter_rtl_view_top [current_fileset]
set_property top tb_mod11_counter [get_filesets sim_1]
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_simulation -simset sim_1 -mode behavioral

if {[llength [get_waves -quiet *]] > 0} {
  delete_wave *
}

add_wave -divider "Inputs"
add_wave -radix binary /tb_mod11_counter/clk
add_wave -radix binary /tb_mod11_counter/rst
add_wave -radix binary /tb_mod11_counter/en
add_wave -radix binary /tb_mod11_counter/load

add_wave -divider "Counter output"
add_wave -radix unsigned /tb_mod11_counter/q
add_wave -radix binary /tb_mod11_counter/tc

run 180 ns
wave zoom full

set wave_dir [file join $root_dir "sim"]
file mkdir $wave_dir
save_wave_config [file join $wave_dir "vivado_mod11_counter_waveform.wcfg"]
