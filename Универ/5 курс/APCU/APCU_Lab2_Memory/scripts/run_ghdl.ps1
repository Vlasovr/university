$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$src = Join-Path $root "src"
$tb = Join-Path $root "tb"
$sim = Join-Path $root "sim"
New-Item -ItemType Directory -Force -Path $sim | Out-Null

$stage = Join-Path $env:TEMP "apcu_lab2_memory_ghdl"
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stage
New-Item -ItemType Directory -Force -Path (Join-Path $stage "src"), (Join-Path $stage "tb") | Out-Null
Copy-Item (Join-Path $src "*.vhd") (Join-Path $stage "src")
Copy-Item (Join-Path $tb "*.vhd") (Join-Path $stage "tb")

$log = Join-Path $sim "ghdl_run.log"
Set-Content -Path $log -Value "GHDL run log for APCU Lab 2 memory transfer, variant 4`n" -Encoding UTF8

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
  Invoke-Ghdl @("-a", "--std=08", "src\lpm_rom_model.vhd", "src\lpm_ram_io_model.vhd", "src\memory_transfer.vhd", "tb\tb_memory_transfer.vhd")
  Invoke-Ghdl @("-e", "--std=08", "tb_memory_transfer")
  Invoke-Ghdl @("-r", "--std=08", "tb_memory_transfer", "--vcd=memory_transfer.vcd", "--stop-time=130ns")
  Copy-Item (Join-Path $stage "memory_transfer.vcd") (Join-Path $sim "memory_transfer.vcd") -Force
}
finally {
  Pop-Location
}
