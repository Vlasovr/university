# Скриншоты Quartus II и Vivado для лабораторной работы 2

## Что уже подготовлено

- Quartus project: `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\memory_transfer.qpf`
- Quartus input waveform: `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\memory_transfer_waveform.vwf`
- Quartus calculated waveform: `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\memory_transfer_waveform_result.vwf`
- Quartus simulation logs: `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\waveform_output`
- Vivado waveform Tcl: `C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\run_vivado_waveform.tcl`
- Vivado GUI launcher: `C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\run_vivado_waveform_gui.ps1`
- Vivado synthesis Tcl: `C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\open_synthesis_for_screenshot.tcl`

## Quartus II: синтез и RTL Viewer

1. Открой Quartus II.
2. `File -> Open Project...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\memory_transfer.qpf`.
4. Запусти компиляцию: `Processing -> Start Compilation`.
5. Когда будет `Full Compilation was successful`, сделай скрин окна `Compilation Report / Flow Summary`.
6. Открой RTL-схему: `Tools -> Netlist Viewers -> RTL Viewer`.
7. Дождись окна схемы и сделай скрин иерархии `memory_transfer_rtl_view_top`.

## Quartus II: временная диаграмма

Самый быстрый путь:

1. В Quartus открой проект `memory_transfer.qpf`.
2. `File -> Open...`
3. Открой файл `C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\memory_transfer_waveform_result.vwf`.
4. Если диаграмма открылась слишком мелко, нажми `View -> Fit in Window` или приблизь участок `0...90 ns`.
5. На скрине должны быть видны `CLOCK`, `RST`, `START`, `DONE`, `BUSY`, `OK`, `BUS_D7...BUS_D0`.
6. Главное показать момент, где на `BUS_D7...BUS_D0` получается `01011010`, то есть `0x5A`, а `OK` и `DONE` становятся `1`.

Если надо пересчитать времянку прямо в Quartus:

1. Открой `memory_transfer_waveform.vwf`.
2. `Processing -> Generate Functional Simulation Netlist`.
3. Потом `Processing -> Start Simulation`.
4. После симуляции Quartus перезапишет waveform результатами. Сделай скрин.

Через PowerShell можно пересчитать так:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab2_Memory\quartus\run_quartus_waveform.ps1"
```

## Vivado: синтез и RTL/schematic

Вариант через GUI:

1. Открой Vivado.
2. `Tools -> Run Tcl Script...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\open_synthesis_for_screenshot.tcl`.
4. Дождись окончания `synth_1`.
5. Сделай скрин окна `Project Summary / Synthesis Completed`.
6. В левой панели открой `Open Synthesized Design`.
7. Для схемы: `Schematic` или `Tools -> Schematic`.
8. Сделай скрин схемы top-level `memory_transfer_rtl_view_top`.

## Vivado: временная диаграмма

Вариант через GUI:

1. Открой Vivado.
2. `Tools -> Run Tcl Script...`
3. Выбери `C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\run_vivado_waveform.tcl`.
4. Скрипт откроет behavioral simulation, добавит нужные сигналы и выполнит `run 130 ns`.
5. В окне Wave должны быть сигналы:
   `clk`, `rst`, `start`, `done`, `busy`, `verify_ok`, `state`, `rom_addr`, `ram_addr`, `rom_q`, `reg_q`, `ram_q`, `data_bus`.
6. Для красивого скрина выставь radix `Hexadecimal` у `rom_q`, `reg_q`, `ram_q`, `data_bus`.
7. Покажи участок, где `rom_addr = 4`, `ram_addr = 5`, `rom_q/reg_q/ram_q/data_bus = 5A`, а `verify_ok = 1`.

Можно запустить Vivado-времянку из PowerShell:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "C:\Users\user\Desktop\APCU_Lab2_Memory\vivado\run_vivado_waveform_gui.ps1"
```

## Что говорить преподавателю

На временной диаграмме видно, что после `START` автомат проходит состояния чтения ROM, загрузки регистра, записи RAM и чтения RAM. Для варианта 4 адрес источника равен `4`, адрес приемника равен `5`. Значение `ROM[4] = 0x5A` появляется на общей шине, фиксируется в регистре, записывается в `RAM[5]`, затем читается обратно. Флаги `DONE = 1` и `OK = 1` подтверждают успешный перенос.
