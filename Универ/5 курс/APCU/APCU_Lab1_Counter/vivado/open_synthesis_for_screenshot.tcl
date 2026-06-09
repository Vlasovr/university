set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ".."]]
set project_dir [file join $script_dir "mod11_counter_vivado"]

if {![file exists [file join $project_dir "mod11_counter.xpr"]]} {
  source [file join $script_dir "create_project.tcl"]
} else {
  open_project [file join $project_dir "mod11_counter.xpr"]
}

set_property top mod11_counter_rtl_view_top [current_fileset]
update_compile_order -fileset sources_1

if {[get_property PROGRESS [get_runs synth_1]] != "100%"} {
  launch_runs synth_1
  wait_on_run synth_1
}

open_run synth_1 -name synth_1
report_utilization -file [file join $script_dir "vivado_output" "mod11_counter_screenshot_utilization.rpt"] -force
