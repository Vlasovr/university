# Скриншоты Quartus II и Vivado для лабораторной работы 3

## Что уже подготовлено

- Quartus project: `C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\sn74145.qpf`
- Quartus input waveform: `C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\sn74145_waveform.vwf`
- Quartus calculated waveform: `C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\sn74145_waveform_result.vwf`
- Vivado waveform Tcl: `C:\Users\user\Desktop\APCU_Lab3_SN74145\vivado\run_vivado_waveform.tcl`
- Vivado synthesis Tcl: `C:\Users\user\Desktop\APCU_Lab3_SN74145\vivado\open_synthesis_for_screenshot.tcl`

## Quartus II: синтез и RTL Viewer

1. Открой Quartus II.
2. `File -> Open Project...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\sn74145.qpf`.
4. Запусти `Processing -> Start Compilation`.
5. Сделай скрин успешной компиляции в `Compilation Report / Flow Summary`.
6. Открой `Tools -> Netlist Viewers -> RTL Viewer`.
7. Сделай скрин схемы `sn74145_compare_top`: должны быть два блока реализации и сигнал `MATCH`.

## Quartus II: временная диаграмма

1. В Quartus открой проект `sn74145.qpf`.
2. `File -> Open...`
3. Открой `C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\sn74145_waveform_result.vwf`.
4. Нажми `View -> Fit in Window` или приблизь участок `0...170 ns`.
5. На скрине должны быть входы `D`, `C`, `B`, `A`, сигнал `MATCH`, выходы `YL9...YL0` и `YS9...YS0`.
6. Главное показать полный перебор входов `0000...1111`: для кодов `0...9` один выход активен нулем, для `10...15` все выходы единицы, `MATCH = 1`.

Если нужно пересчитать:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab3_SN74145\quartus\run_quartus_waveform.ps1"
```

## Vivado: синтез, схема и временная диаграмма

Синтез:

1. Открой Vivado.
2. `Tools -> Run Tcl Script...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab3_SN74145\vivado\open_synthesis_for_screenshot.tcl`.
4. Сделай скрин `Synthesis Completed`, `Utilization` и `Schematic`.

Временная диаграмма:

1. `Tools -> Run Tcl Script...`
2. Выбери `C:\Users\user\Desktop\APCU_Lab3_SN74145\vivado\run_vivado_waveform.tcl`.
3. Скрипт добавит `d`, `c`, `b`, `a`, `y_logic`, `y_seq` и выполнит `run 170 ns`.
4. На скрине покажи полный перебор входных кодов и совпадение двух выходных векторов.

PowerShell-запуск GUI:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab3_SN74145\vivado\run_vivado_waveform_gui.ps1"
```
