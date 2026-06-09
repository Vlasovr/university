set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "memory_transfer_vivado"]

if {![file exists [file join $project_dir "memory_transfer.xpr"]]} {
  create_project memory_transfer $project_dir -part xc7a35tcpg236-1 -force
  set_property target_language VHDL [current_project]

  add_files [file join $root_dir "src" "lpm_rom_model.vhd"]
  add_files [file join $root_dir "src" "lpm_ram_io_model.vhd"]
  add_files [file join $root_dir "src" "memory_transfer.vhd"]
  add_files [file join $root_dir "src" "memory_transfer_rtl_view_top.vhd"]
  add_files -fileset sim_1 [file join $root_dir "tb" "tb_memory_transfer.vhd"]
  add_files -fileset constrs_1 [file join $script_dir "memory_transfer.xdc"]
} else {
  open_project [file join $project_dir "memory_transfer.xpr"]
}

set_property top memory_transfer_rtl_view_top [current_fileset]
set_property top tb_memory_transfer [get_filesets sim_1]
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_simulation -simset sim_1 -mode behavioral

if {[llength [get_waves -quiet *]] > 0} {
  delete_wave *
}

add_wave -divider "Inputs"
add_wave -radix binary /tb_memory_transfer/clk
add_wave -radix binary /tb_memory_transfer/rst
add_wave -radix binary /tb_memory_transfer/start

add_wave -divider "Status"
add_wave -radix binary /tb_memory_transfer/done
add_wave -radix binary /tb_memory_transfer/busy
add_wave -radix binary /tb_memory_transfer/verify_ok
add_wave -radix unsigned /tb_memory_transfer/state

add_wave -divider "Variant 4 transfer"
add_wave -radix hexadecimal /tb_memory_transfer/rom_addr
add_wave -radix hexadecimal /tb_memory_transfer/ram_addr
add_wave -radix hexadecimal /tb_memory_transfer/rom_q
add_wave -radix hexadecimal /tb_memory_transfer/reg_q
add_wave -radix hexadecimal /tb_memory_transfer/ram_q
add_wave -radix hexadecimal /tb_memory_transfer/data_bus

run 130 ns
wave zoom full

set wave_dir [file join $root_dir "sim"]
file mkdir $wave_dir
save_wave_config [file join $wave_dir "vivado_memory_transfer_waveform.wcfg"]
