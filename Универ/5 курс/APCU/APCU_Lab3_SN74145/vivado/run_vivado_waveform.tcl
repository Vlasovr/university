set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "sn74145_vivado"]

if {![file exists [file join $project_dir "sn74145.xpr"]]} {
  create_project sn74145 $project_dir -part xc7a35tcpg236-1 -force
  set_property target_language VHDL [current_project]

  add_files [file join $root_dir "src" "sn74145_concurrent.vhd"]
  add_files [file join $root_dir "src" "sn74145_sequential.vhd"]
  add_files [file join $root_dir "src" "sn74145_compare_top.vhd"]
  add_files -fileset sim_1 [file join $root_dir "tb" "tb_sn74145.vhd"]
  add_files -fileset constrs_1 [file join $script_dir "sn74145.xdc"]
} else {
  open_project [file join $project_dir "sn74145.xpr"]
}

set_property top sn74145_compare_top [current_fileset]
set_property top tb_sn74145 [get_filesets sim_1]
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_simulation -simset sim_1 -mode behavioral

if {[llength [get_waves -quiet *]] > 0} {
  delete_wave *
}

add_wave -divider "BCD inputs"
add_wave -radix binary /tb_sn74145/d
add_wave -radix binary /tb_sn74145/c
add_wave -radix binary /tb_sn74145/b
add_wave -radix binary /tb_sn74145/a

add_wave -divider "Decoder outputs"
add_wave -radix binary /tb_sn74145/y_logic
add_wave -radix binary /tb_sn74145/y_seq

run 170 ns
wave zoom full

set wave_dir [file join $root_dir "sim"]
file mkdir $wave_dir
save_wave_config [file join $wave_dir "vivado_sn74145_waveform.wcfg"]
