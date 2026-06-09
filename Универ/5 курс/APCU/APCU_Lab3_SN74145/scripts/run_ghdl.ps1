$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$src = Join-Path $root "src"
$tb = Join-Path $root "tb"
$sim = Join-Path $root "sim"
New-Item -ItemType Directory -Force -Path $sim | Out-Null

$stage = Join-Path $env:TEMP "apcu_lab3_sn74145_ghdl"
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stage
New-Item -ItemType Directory -Force -Path (Join-Path $stage "src"), (Join-Path $stage "tb") | Out-Null
Copy-Item (Join-Path $src "*.vhd") (Join-Path $stage "src")
Copy-Item (Join-Path $tb "*.vhd") (Join-Path $stage "tb")

$log = Join-Path $sim "ghdl_run.log"
Set-Content -Path $log -Value "GHDL run log for APCU combination lab, variant 4, SN74145`n" -Encoding UTF8

function Invoke-Ghdl {
  param([string[]]$Arguments)
  $output = & ghdl @Arguments 2>&1
  $text = (($output | ForEach-Object { $_.ToString() }) -join [Environment]::NewLine) -replace "`0", ""
  if ($text.Length -gt 0) {
    Add-Content -Path $log -Value $text -Encoding UTF8
    Write-Output $text
  }
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

Push-Location $stage
try {
  Invoke-Ghdl @("--clean")
  Invoke-Ghdl @("-a", "--std=08", "src\sn74145_concurrent.vhd", "src\sn74145_sequential.vhd", "src\sn74145_compare_top.vhd", "tb\tb_sn74145.vhd")
  Invoke-Ghdl @("-e", "--std=08", "tb_sn74145")
  Invoke-Ghdl @("-r", "--std=08", "tb_sn74145", "--vcd=sn74145.vcd", "--stop-time=180ns")
  Copy-Item (Join-Path $stage "sn74145.vcd") (Join-Path $sim "sn74145.vcd") -Force
}
finally {
  Pop-Location
}
