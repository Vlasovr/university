set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "mod11_counter_vivado"]

create_project mod11_counter $project_dir -part xc7a35tcpg236-1 -force
set_property target_language VHDL [current_project]

add_files [file join $root_dir "src" "mod11_counter.vhd"]
add_files [file join $root_dir "src" "mod11_counter_rtl_view_top.vhd"]
add_files -fileset sim_1 [file join $root_dir "tb" "tb_mod11_counter.vhd"]
add_files -fileset constrs_1 [file join $script_dir "mod11_counter.xdc"]

set_property top mod11_counter_rtl_view_top [current_fileset]
set_property top tb_mod11_counter [get_filesets sim_1]

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_runs synth_1
wait_on_run synth_1
open_run synth_1 -name synth_1
write_checkpoint -force [file join $project_dir "mod11_counter_synth.dcp"]
