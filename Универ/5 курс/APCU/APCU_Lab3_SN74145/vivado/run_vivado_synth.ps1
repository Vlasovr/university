$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$stageRoot = "D:\vivado_stage"
$stage = Join-Path $stageRoot "apcu_lab3_sn74145"
$outDir = Join-Path $PSScriptRoot "vivado_output"

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

  throw "Vivado was not found. Add vivado.bat to PATH or update run_vivado_synth.ps1."
}

$vivado = Find-Vivado

New-Item -ItemType Directory -Force -Path $stageRoot | Out-Null
$resolvedStageRoot = (Resolve-Path $stageRoot).Path
$stageFull = [System.IO.Path]::GetFullPath($stage)
if (-not $stageFull.StartsWith($resolvedStageRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
  throw "Refusing to clean stage outside $resolvedStageRoot"
}

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stageFull
New-Item -ItemType Directory -Force -Path "$stageFull\src", "$stageFull\tb", "$stageFull\vivado" | Out-Null
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

Copy-Item (Join-Path $root "src\*.vhd") "$stageFull\src"
Copy-Item (Join-Path $root "tb\*.vhd") "$stageFull\tb"
Copy-Item (Join-Path $PSScriptRoot "create_project.tcl"), (Join-Path $PSScriptRoot "sn74145.xdc") "$stageFull\vivado"

$log = Join-Path $stageFull "vivado\vivado_ascii_run.log"
$jou = Join-Path $stageFull "vivado\vivado_ascii_run.jou"

& $vivado -mode batch -source "$stageFull\vivado\create_project.tcl" -journal $jou -log $log
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Copy-Item -Force $log, $jou $outDir
$runDir = Join-Path $stageFull "vivado\sn74145_vivado\sn74145.runs\synth_1"
Copy-Item -Force (Join-Path $runDir "runme.log") $outDir
Copy-Item -Force (Join-Path $runDir "sn74145_compare_top_utilization_synth.rpt") $outDir
Copy-Item -Force (Join-Path $runDir "sn74145_compare_top.dcp") $outDir
Copy-Item -Force (Join-Path $stageFull "vivado\sn74145_vivado\sn74145_synth.dcp") $outDir

Write-Host "Vivado synthesis output copied to $outDir"
