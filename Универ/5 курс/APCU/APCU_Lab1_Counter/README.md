# Лабораторная работа 1: счетчик с принудительным порядком счета

Объект: синхронный счетчик по модулю 11 с последовательностью счета
`5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 5`.

## Состав

- `src/mod11_counter.vhd` - основной VHDL-модуль счетчика.
- `src/mod11_counter_rtl_view_top.vhd` - верхний уровень для RTL Viewer.
- `tb/tb_mod11_counter.vhd` - тестбенч для функциональной проверки.
- `quartus/` - проект Quartus II 9.1.
- `vivado/` - TCL-скрипт и шаблон XDC для проекта Xilinx Vivado.
- `scripts/run_ghdl.ps1` - запуск моделирования GHDL и формирование VCD.
- `report/` - отчет по лабораторной работе.

## Проверка

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run_ghdl.ps1
powershell -ExecutionPolicy Bypass -File .\quartus\run_quartus_compile.ps1
```

Фактически проверено на этой машине:

- GHDL 6.0.0: тестбенч завершился сообщением `tb_mod11_counter: TEST PASSED`.
- Quartus II 9.1: Full Compilation завершена успешно, `0 errors, 5 warnings`.
- Vivado ML Standard 2022.1: synthesis завершен успешно, `0 errors, 0 critical warnings, 1 warning`.
- Важно: Vivado 2022.1 падает при запуске проекта прямо из пути с кириллицей `Универ`. Для надежной проверки используйте wrapper `vivado/run_vivado_synth.ps1`, он запускает проект из временной ASCII-папки `D:\vivado_stage`.

Vivado-проект создается командой:

```powershell
vivado -mode batch -source .\vivado\create_project.tcl
```

Рекомендуемая проверка Vivado на этой машине:

```powershell
powershell -ExecutionPolicy Bypass -File .\vivado\run_vivado_synth.ps1
```
