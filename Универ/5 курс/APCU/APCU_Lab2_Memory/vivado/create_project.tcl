set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "memory_transfer_vivado"]

create_project memory_transfer $project_dir -part xc7a35tcpg236-1 -force
set_property target_language VHDL [current_project]

add_files [file join $root_dir "src" "lpm_rom_model.vhd"]
add_files [file join $root_dir "src" "lpm_ram_io_model.vhd"]
add_files [file join $root_dir "src" "memory_transfer.vhd"]
add_files [file join $root_dir "src" "memory_transfer_rtl_view_top.vhd"]
add_files -fileset sim_1 [file join $root_dir "tb" "tb_memory_transfer.vhd"]
add_files -fileset constrs_1 [file join $script_dir "memory_transfer.xdc"]

set_property top memory_transfer_rtl_view_top [current_fileset]
set_property top tb_memory_transfer [get_filesets sim_1]

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_runs synth_1
wait_on_run synth_1
open_run synth_1 -name synth_1
write_checkpoint -force [file join $project_dir "memory_transfer_synth.dcp"]
