set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "sn74145_vivado"]

create_project sn74145 $project_dir -part xc7a35tcpg236-1 -force
set_property target_language VHDL [current_project]

add_files [file join $root_dir "src" "sn74145_concurrent.vhd"]
add_files [file join $root_dir "src" "sn74145_sequential.vhd"]
add_files [file join $root_dir "src" "sn74145_compare_top.vhd"]
add_files -fileset sim_1 [file join $root_dir "tb" "tb_sn74145.vhd"]
add_files -fileset constrs_1 [file join $script_dir "sn74145.xdc"]

set_property top sn74145_compare_top [current_fileset]
set_property top tb_sn74145 [get_filesets sim_1]

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

launch_runs synth_1
wait_on_run synth_1
open_run synth_1 -name synth_1
write_checkpoint -force [file join $project_dir "sn74145_synth.dcp"]
