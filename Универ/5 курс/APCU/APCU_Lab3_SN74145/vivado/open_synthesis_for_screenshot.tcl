set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "sn74145_vivado"]

if {![file exists [file join $project_dir "sn74145.xpr"]]} {
  source [file join $script_dir "create_project.tcl"]
} else {
  open_project [file join $project_dir "sn74145.xpr"]
}

set_property top sn74145_compare_top [current_fileset]
update_compile_order -fileset sources_1

if {[get_property PROGRESS [get_runs synth_1]] != "100%"} {
  launch_runs synth_1
  wait_on_run synth_1
}

open_run synth_1 -name synth_1
report_utilization -file [file join $script_dir "vivado_output" "sn74145_screenshot_utilization.rpt"] -force
