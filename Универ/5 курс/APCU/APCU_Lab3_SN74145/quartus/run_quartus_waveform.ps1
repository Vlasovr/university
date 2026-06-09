$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$stage = Join-Path $env:TEMP "apcu_lab3_sn74145_quartus_waveform"
$stageQuartus = Join-Path $stage "quartus"
$stageSrc = Join-Path $stage "src"
$outDir = Join-Path $PSScriptRoot "waveform_output"

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stage
New-Item -ItemType Directory -Force -Path $stageQuartus, $stageSrc, $outDir | Out-Null
Copy-Item (Join-Path $root "src\*.vhd") $stageSrc
Copy-Item (Join-Path $PSScriptRoot "sn74145.qpf") $stageQuartus
Copy-Item (Join-Path $PSScriptRoot "sn74145.qsf") $stageQuartus
Copy-Item (Join-Path $PSScriptRoot "sn74145_waveform.vwf") $stageQuartus

Push-Location $stageQuartus
try {
  $mapLog = Join-Path $outDir "quartus_waveform_map.log"
  $simLog = Join-Path $outDir "quartus_waveform_sim.log"

  & quartus_map sn74145 --generate_functional_sim_netlist 2>&1 | Tee-Object -FilePath $mapLog
  if ($LASTEXITCODE -ne 0) {
    throw "quartus_map functional netlist generation failed"
  }

  & quartus_sim sn74145 --mode=functional --vector_source=sn74145_waveform.vwf --simulation_results_format=VWF --overwrite_waveform=on 2>&1 | Tee-Object -FilePath $simLog
  if ($LASTEXITCODE -ne 0) {
    throw "quartus_sim waveform run failed"
  }

  Copy-Item -ErrorAction SilentlyContinue (Join-Path $stageQuartus "sn74145_waveform*") $outDir
}
finally {
  Pop-Location
}
