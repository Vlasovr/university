# Скриншоты Quartus II и Vivado для лабораторной работы 1

## Что уже подготовлено

- Quartus project: `C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\mod11_counter.qpf`
- Quartus input waveform: `C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\mod11_counter_waveform.vwf`
- Quartus calculated waveform: `C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\mod11_counter_waveform_result.vwf`
- Vivado waveform Tcl: `C:\Users\user\Desktop\APCU_Lab1_Counter\vivado\run_vivado_waveform.tcl`
- Vivado synthesis Tcl: `C:\Users\user\Desktop\APCU_Lab1_Counter\vivado\open_synthesis_for_screenshot.tcl`

## Quartus II: синтез и RTL Viewer

1. Открой Quartus II.
2. `File -> Open Project...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\mod11_counter.qpf`.
4. Запусти `Processing -> Start Compilation`.
5. Сделай скрин `Compilation Report / Flow Summary` с успешной компиляцией.
6. Открой `Tools -> Netlist Viewers -> RTL Viewer`.
7. Сделай скрин схемы `mod11_counter_rtl_view_top`.

## Quartus II: временная диаграмма

1. В Quartus открой проект `mod11_counter.qpf`.
2. `File -> Open...`
3. Открой `C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\mod11_counter_waveform_result.vwf`.
4. Нажми `View -> Fit in Window` или приблизь участок `0...170 ns`.
5. На скрине должны быть `CLOCK`, `RST`, `EN`, `LOAD`, `Q3`, `Q2`, `Q1`, `Q0`, `TC`.
6. Главное показать счет `5 -> 6 -> ... -> 15 -> 5`, импульс `TC` на 15 и возврат к 5 по `LOAD`.

Если нужно пересчитать:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab1_Counter\quartus\run_quartus_waveform.ps1"
```

## Vivado: синтез, схема и временная диаграмма

Синтез:

1. Открой Vivado.
2. `Tools -> Run Tcl Script...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab1_Counter\vivado\open_synthesis_for_screenshot.tcl`.
4. Сделай скрин `Synthesis Completed`, `Utilization` и `Schematic`.

Временная диаграмма:

1. `Tools -> Run Tcl Script...`
2. Выбери `C:\Users\user\Desktop\APCU_Lab1_Counter\vivado\run_vivado_waveform.tcl`.
3. Скрипт добавит `clk`, `rst`, `en`, `load`, `q`, `tc` и выполнит `run 180 ns`.
4. На скрине покажи последовательность счета от 5 до 15, сброс на 5 и сигнал `tc`.

PowerShell-запуск GUI:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab1_Counter\vivado\run_vivado_waveform_gui.ps1"
```
