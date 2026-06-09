$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$stage = Join-Path $env:TEMP "apcu_lab2_memory_quartus"
$stageQuartus = Join-Path $stage "quartus"
$stageSrc = Join-Path $stage "src"
$outDir = Join-Path $PSScriptRoot "compile_output"

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $stage
New-Item -ItemType Directory -Force -Path $stageQuartus, $stageSrc, $outDir | Out-Null
Copy-Item (Join-Path $root "src\*.vhd") $stageSrc
Copy-Item (Join-Path $PSScriptRoot "memory_transfer.qpf") $stageQuartus
Copy-Item (Join-Path $PSScriptRoot "memory_transfer.qsf") $stageQuartus

Push-Location $stageQuartus
try {
  $log = Join-Path $outDir "quartus_compile.log"
  $output = & quartus_sh --flow compile memory_transfer 2>&1
  $output | Tee-Object -FilePath $log
  if ($LASTEXITCODE -ne 0 -or ($output -join "`n") -match "unsuccessful|ERROR:|Error:") {
    Copy-Item -ErrorAction SilentlyContinue (Join-Path $stageQuartus "memory_transfer.*") $outDir
    exit 1
  }
  Copy-Item -ErrorAction SilentlyContinue (Join-Path $stageQuartus "memory_transfer.*") $outDir
  Copy-Item -Recurse -Force -ErrorAction SilentlyContinue (Join-Path $stageQuartus "db") $outDir
}
finally {
  Pop-Location
}
