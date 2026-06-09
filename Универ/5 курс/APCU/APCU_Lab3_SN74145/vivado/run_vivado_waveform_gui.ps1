$ErrorActionPreference = "Stop"

function Find-Vivado {
  $cmd = Get-Command vivado -ErrorAction SilentlyContinue
  if ($cmd) { return $cmd.Source }

  $known = @(
    "D:\Vivado\2022.1\bin\vivado.bat",
    "C:\Xilinx\Vivado\2022.1\bin\vivado.bat",
    "D:\Xilinx\Vivado\2022.1\bin\vivado.bat"
  )

  foreach ($path in $known) {
    if (Test-Path $path) { return $path }
  }

  throw "Vivado was not found. Open Vivado manually and run run_vivado_waveform.tcl from Tools -> Run Tcl Script."
}

$vivado = Find-Vivado
& $vivado -mode gui -source (Join-Path $PSScriptRoot "run_vivado_waveform.tcl")
