$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$src = Join-Path $root "src"
$tb = Join-Path $root "tb"
$sim = Join-Path $root "sim"
New-Item -ItemType Directory -Force -Path $sim | Out-Null
$stage = Join-Path $env:TEMP "apcu_lab1_counter_ghdl"
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stage
New-Item -ItemType Directory -Force -Path (Join-Path $stage "src"), (Join-Path $stage "tb") | Out-Null
Copy-Item (Join-Path $src "mod11_counter.vhd") (Join-Path $stage "src")
Copy-Item (Join-Path $tb "tb_mod11_counter.vhd") (Join-Path $stage "tb")
$log = Join-Path $sim "ghdl_run.log"
Set-Content -Path $log -Value "GHDL run log for mod11_counter`n" -Encoding UTF8

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
  Invoke-Ghdl @("-a", "--std=08", (Join-Path $stage "src\mod11_counter.vhd"))
  Invoke-Ghdl @("-a", "--std=08", (Join-Path $stage "tb\tb_mod11_counter.vhd"))
  Invoke-Ghdl @("-e", "--std=08", "tb_mod11_counter")
  Invoke-Ghdl @("-r", "--std=08", "tb_mod11_counter", "--vcd=mod11_counter.vcd", "--stop-time=180ns")
  Copy-Item (Join-Path $stage "mod11_counter.vcd") (Join-Path $sim "mod11_counter.vcd") -Force
}
finally {
  Pop-Location
}
