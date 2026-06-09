from __future__ import annotations

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import math


ROOT = Path(__file__).resolve().parents[1]
IMAGES = ROOT / "images"
VCD = ROOT / "sim" / "memory_transfer.vcd"


def font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont:
    names = [
        r"C:\Windows\Fonts\timesbd.ttf" if bold else r"C:\Windows\Fonts\times.ttf",
        r"C:\Windows\Fonts\arialbd.ttf" if bold else r"C:\Windows\Fonts\arial.ttf",
    ]
    for name in names:
        p = Path(name)
        if p.exists():
            return ImageFont.truetype(str(p), size)
    return ImageFont.load_default()


F16 = font(16)
F18 = font(18)
F18B = font(18, True)
F20 = font(20)
F20B = font(20, True)
F22 = font(22)
F22B = font(22, True)
F24B = font(24, True)
F28B = font(28, True)


def arrow(draw: ImageDraw.ImageDraw, start: tuple[int, int], end: tuple[int, int], width: int = 2) -> None:
    draw.line([start, end], fill="black", width=width)
    angle = math.atan2(end[1] - start[1], end[0] - start[0])
    length = 13
    spread = math.radians(28)
    p1 = (end[0] - length * math.cos(angle - spread), end[1] - length * math.sin(angle - spread))
    p2 = (end[0] - length * math.cos(angle + spread), end[1] - length * math.sin(angle + spread))
    draw.polygon([end, (int(p1[0]), int(p1[1])), (int(p2[0]), int(p2[1]))], fill="black")


def center(draw: ImageDraw.ImageDraw, box: tuple[int, int, int, int], text: str, fnt=F20B) -> None:
    b = draw.textbbox((0, 0), text, font=fnt)
    draw.text((box[0] + (box[2] - box[0] - b[2] + b[0]) / 2, box[1] + (box[3] - box[1] - b[3] + b[1]) / 2 - 2), text, fill="black", font=fnt)


def wrap(draw: ImageDraw.ImageDraw, text: str, fnt: ImageFont.FreeTypeFont, width: int) -> list[str]:
    words = text.split()
    lines: list[str] = []
    cur = ""
    for word in words:
        cand = word if not cur else cur + " " + word
        b = draw.textbbox((0, 0), cand, font=fnt)
        if b[2] - b[0] <= width:
            cur = cand
        else:
            if cur:
                lines.append(cur)
            cur = word
    if cur:
        lines.append(cur)
    return lines


def box(draw: ImageDraw.ImageDraw, xy: tuple[int, int, int, int], title: str, lines: list[str] | None = None, fill=(247, 247, 247)) -> None:
    draw.rounded_rectangle(xy, radius=8, outline="black", width=3, fill=fill)
    center(draw, (xy[0] + 8, xy[1] + 8, xy[2] - 8, xy[1] + 42), title, F22B)
    y = xy[1] + 56
    for line in lines or []:
        for part in wrap(draw, line, F18, xy[2] - xy[0] - 32):
            draw.text((xy[0] + 16, y), part, fill="black", font=F18)
            y += 24


def parse_vcd(path: Path) -> dict[str, list[tuple[float, str]]]:
    targets = {
        "clk", "rst", "start", "done", "busy", "verify_ok", "state[2:0]",
        "data_bus[7:0]", "rom_q[7:0]", "reg_q[7:0]", "ram_q[7:0]",
        "rom_addr[3:0]", "ram_addr[3:0]",
    }
    ids: dict[str, str] = {}
    waves = {name: [] for name in targets}
    in_defs = True
    t = 0.0
    for raw in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw.strip()
        if not line:
            continue
        if in_defs:
            if line.startswith("$var"):
                parts = line.split()
                if len(parts) >= 5:
                    ident, name = parts[3], parts[4]
                    if name in targets and name not in ids.values():
                        ids[ident] = name
            elif line == "$enddefinitions $end":
                in_defs = False
            continue
        if line.startswith("#"):
            t = int(line[1:]) / 1_000_000.0
        elif line[0] in "01xXzZ":
            ident = line[1:]
            if ident in ids:
                waves[ids[ident]].append((t, line[0].upper()))
        elif line.startswith("b"):
            value, ident = line[1:].split()
            if ident in ids:
                waves[ids[ident]].append((t, value.upper()))
    return waves


def value_at(events: list[tuple[float, str]], t: float) -> str:
    value = events[0][1] if events else "X"
    for et, ev in events:
        if et <= t:
            value = ev
        else:
            break
    return value


def hex_label(value: str) -> str:
    if not value or any(ch in value for ch in "ZXU"):
        return value
    try:
        return "0x" + format(int(value, 2), "X")
    except ValueError:
        return value


def digital(draw: ImageDraw.ImageDraw, events: list[tuple[float, str]], y: int, x0: int, scale: float, start: float, end: float) -> None:
    high, low = y - 17, y + 17
    pts: list[tuple[int, int]] = []
    cur = value_at(events, start)
    cy = high if cur == "1" else low
    pts.append((x0, cy))
    for t, v in events:
        if start <= t <= end:
            x = int(x0 + (t - start) * scale)
            ny = high if v == "1" else low
            pts.append((x, cy))
            pts.append((x, ny))
            cy = ny
    pts.append((int(x0 + (end - start) * scale), cy))
    draw.line(pts, fill="black", width=2)


def bus(draw: ImageDraw.ImageDraw, events: list[tuple[float, str]], y: int, x0: int, scale: float, start: float, end: float) -> None:
    top, bottom = y - 21, y + 21
    times = sorted({start, end, *[t for t, _ in events if start <= t <= end]})
    for left, right in zip(times, times[1:]):
        if right <= left:
            continue
        x1, x2 = int(x0 + (left - start) * scale), int(x0 + (right - start) * scale)
        draw.rectangle([x1, top, x2, bottom], outline="black", width=2)
        label = hex_label(value_at(events, left))
        if x2 - x1 > 46:
            b = draw.textbbox((0, 0), label, font=F16)
            draw.text(((x1 + x2 - b[2] + b[0]) / 2, y - 9), label, fill="black", font=F16)


def timing(filename: str, title: str, start: float, end: float, note: str, rows: list[tuple[str, str]]) -> None:
    waves = parse_vcd(VCD)
    image = Image.new("RGB", (1700, 900), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), title, fill="black", font=F28B)
    draw.text((45, 72), note, fill="black", font=F20)
    x0, x1 = 190, 1620
    scale = (x1 - x0) / (end - start)
    draw.line([(x0, 130), (x1, 130)], fill="black", width=2)
    step = 5 if end - start <= 70 else 10
    t = int(start)
    while t <= int(end):
        x = int(x0 + (t - start) * scale)
        draw.line([(x, 122), (x, 138)], fill="black", width=1)
        draw.text((x - 12, 98), str(t), fill="black", font=F16)
        t += step
    draw.text((x1 + 12, 98), "ns", fill="black", font=F16)
    for i, (name, label) in enumerate(rows):
        y = 180 + i * 72
        draw.text((45, y - 14), label, fill="black", font=F20B if len(label) < 8 else F18B)
        draw.line([(x0, y + 36), (x1, y + 36)], fill=(225, 225, 225), width=1)
        if "[" in name:
            bus(draw, waves[name], y, x0, scale, start, end)
        else:
            digital(draw, waves[name], y, x0, scale, start, end)
    image.save(IMAGES / filename, dpi=(200, 200))


def table_image(filename: str, title: str, subtitle: str, headers: list[str], rows: list[list[str]], widths: list[int], highlight: int | None = None) -> None:
    h = 150 + 54 * (len(rows) + 1) + 70
    image = Image.new("RGB", (sum(widths) + 180, h), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), title, fill="black", font=F28B)
    draw.text((45, 72), subtitle, fill="black", font=F20)
    x0, y0 = 90, 140
    x = x0
    for head, w in zip(headers, widths):
        draw.rectangle([x, y0, x + w, y0 + 54], outline="black", width=2, fill=(232, 232, 232))
        center(draw, (x, y0, x + w, y0 + 54), head, F18B)
        x += w
    for r, row in enumerate(rows):
        y = y0 + (r + 1) * 54
        x = x0
        fill = (224, 224, 224) if highlight == r else "white"
        for text, w in zip(row, widths):
            draw.rectangle([x, y, x + w, y + 54], outline="black", width=1, fill=fill)
            center(draw, (x, y, x + w, y + 54), text, F18)
            x += w
    image.save(IMAGES / filename, dpi=(200, 200))


def card(filename: str, title: str, subtitle: str, rows: list[tuple[str, str]]) -> None:
    image = Image.new("RGB", (1600, 900), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), title, fill="black", font=F28B)
    draw.text((45, 72), subtitle, fill="black", font=F20)
    x0, y0, c1, c2 = 90, 145, 460, 900
    draw.rectangle([x0, y0, x0 + c1 + c2, y0 + 52], outline="black", width=2, fill=(232, 232, 232))
    center(draw, (x0, y0, x0 + c1, y0 + 52), "Параметр", F18B)
    center(draw, (x0 + c1, y0, x0 + c1 + c2, y0 + 52), "Значение", F18B)
    for i, (left, right) in enumerate(rows):
        y = y0 + (i + 1) * 58
        draw.rectangle([x0, y, x0 + c1, y + 58], outline="black", width=1, fill="white")
        draw.rectangle([x0 + c1, y, x0 + c1 + c2, y + 58], outline="black", width=1, fill="white")
        draw.text((x0 + 14, y + 16), left, fill="black", font=F18)
        for j, line in enumerate(wrap(draw, right, F18, c2 - 28)[:2]):
            draw.text((x0 + c1 + 14, y + 8 + j * 23), line, fill="black", font=F18)
    image.save(IMAGES / filename, dpi=(200, 200))


def variant_table() -> None:
    rows = [
        ["1", "Синхр.", "Асинхр.", "Синхр.", "Синхр.", "1", "8"],
        ["2", "Асинхр.", "Асинхр.", "Синхр.", "Асинхр.", "2", "7"],
        ["3", "Синхр.", "Синхр.", "Синхр.", "Асинхр.", "3", "6"],
        ["4", "Асинхр.", "Синхр.", "Синхр.", "Синхр.", "4", "5"],
        ["5", "Синхр.", "Асинхр.", "Синхр.", "Асинхр.", "5", "4"],
        ["6", "Асинхр.", "Асинхр.", "Синхр.", "Синхр.", "6", "3"],
        ["7", "Синхр.", "Синхр.", "Синхр.", "Асинхр.", "7", "2"],
        ["8", "Асинхр.", "Синхр.", "Синхр.", "Синхр.", "8", "1"],
    ]
    table_image("variant_table.png", "Вариант 4 лабораторной работы", "Использование lpm_rom и lpm_ram_io на общей шине.", ["№", "ROM ввод", "ROM вывод", "RAM ввод", "RAM вывод", "Источник", "Приемник"], rows, [80, 190, 190, 190, 190, 150, 150], 3)


def architecture() -> None:
    image = Image.new("RGB", (1700, 980), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), "Иерархическая структура блока memory_transfer", fill="black", font=F28B)
    draw.text((45, 72), "Данные проходят по общей 8-разрядной шине: ROM[4] -> регистр -> RAM[5].", fill="black", font=F20)
    box(draw, (115, 230, 405, 390), "lpm_rom", ["16 x 8", "адрес 4", "вывод синхр."], (246, 246, 246))
    box(draw, (690, 230, 990, 390), "8-bit register", ["захват на фронте", "хранит 0x5A"], (246, 246, 246))
    box(draw, (1260, 230, 1550, 390), "lpm_ram_io", ["16 x 8", "адрес 5", "ввод/вывод синхр."], (246, 246, 246))
    box(draw, (640, 550, 1040, 720), "FSM control", ["IDLE -> READ_ROM -> LOAD_REG", "WRITE_RAM -> READ_RAM -> DONE"], (238, 238, 238))
    draw.rectangle([130, 475, 1535, 535], outline="black", width=3, fill=(232, 232, 232))
    center(draw, (130, 475, 1535, 535), "shared data_bus[7:0]", F22B)
    arrow(draw, (260, 390), (260, 475))
    arrow(draw, (840, 475), (840, 390))
    arrow(draw, (840, 390), (840, 475))
    arrow(draw, (840, 390), (1350, 390))
    arrow(draw, (1350, 390), (1350, 475))
    arrow(draw, (840, 550), (840, 535))
    for x, sig in [(260, "rom_en"), (840, "reg_oe"), (1350, "ram_we/ram_oe")]:
        draw.line([(840, 550), (x, 550), (x, 390)], fill="black", width=2)
        draw.text((x + 10, 548), sig, fill="black", font=F18)
    draw.text((120, 850), "Для синхронного вывода ROM/RAM данные становятся валидными после фронта тактового сигнала.", fill="black", font=F20)
    image.save(IMAGES / "memory_architecture.png", dpi=(200, 200))


def fsm_diagram() -> None:
    image = Image.new("RGB", (1700, 760), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), "Автомат управления переносом данных", fill="black", font=F28B)
    states = [
        ("IDLE", "ожидание start"),
        ("READ_ROM", "включить ROM"),
        ("LOAD_REG", "захватить 0x5A"),
        ("WRITE_RAM", "записать RAM[5]"),
        ("READ_RAM", "синхр. чтение"),
        ("DONE", "verify_ok=1"),
    ]
    xs = [140, 410, 700, 990, 1270, 1510]
    y = 320
    for (name, desc), x in zip(states, xs):
        box(draw, (x - 105, y - 70, x + 105, y + 70), name, [desc])
    for a, b in zip(xs, xs[1:]):
        arrow(draw, (a + 105, y), (b - 105, y))
    draw.line([(1510, y + 85), (1510, 560), (140, 560), (140, y + 85)], fill="black", width=2)
    arrow(draw, (140, y + 85), (140, y + 70))
    draw.text((750, 585), "start=0 after DONE", fill="black", font=F20)
    draw.text((45, 690), "state_o encodes the states as 000,001,010,011,100,101 for waveform inspection.", fill="black", font=F20)
    image.save(IMAGES / "fsm_diagram.png", dpi=(200, 200))


def memory_map() -> None:
    rows = []
    values = ["11", "27", "3C", "45", "5A", "6E", "73", "8F", "91", "A4", "B8", "C2", "D5", "E9", "F0", "0D"]
    for i, value in enumerate(values):
        mark = "ROM source" if i == 4 else ("RAM destination" if i == 5 else "")
        rows.append([str(i), "0x" + value, mark])
    table_image("memory_map.png", "Карта памяти для моделирования", "Источник ROM[4] содержит 0x5A; приемник RAM[5] изначально 0x00.", ["Адрес", "ROM data", "Назначение"], rows, [140, 220, 520], 4)


def project_files() -> None:
    rows = [
        ("src/", "VHDL: модели ROM/RAM, автомат переноса, top для RTL Viewer"),
        ("tb/", "tb_memory_transfer.vhd с проверкой ROM[4] -> RAM[5]"),
        ("sim/", "memory_transfer.vcd и ghdl_run.log"),
        ("quartus/", "QPF/QSF, compile_output, SOF/POF"),
        ("vivado/", "TCL/XDC, DCP checkpoint, utilization report"),
        ("images/", "диаграммы, RTL-схемы и картинки результатов"),
        ("report/", "DOCX/PDF отчет и страницы визуальной QA"),
    ]
    card("project_files_diagram.png", "Структура папки лабораторной работы", "Все исходники и результаты сохранены на Desktop.", rows)


def result_cards() -> None:
    card("ghdl_result.png", "Результат функциональной симуляции GHDL", "Автоматический testbench проверил перенос по варианту 4.", [
        ("Simulator", "GHDL"),
        ("Testbench", "tb_memory_transfer"),
        ("Проверка", "ROM[4] = 0x5A -> регистр -> RAM[5]"),
        ("Время PASS", "66 ns"),
        ("VCD", "sim/memory_transfer.vcd"),
        ("Итог", "TEST PASSED, stop-time 130 ns"),
    ])
    card("quartus_result.png", "Результат компиляции Quartus II", "Full Compilation выполнена на доступном семействе Stratix II.", [
        ("Tool", "Quartus II 9.1 Build 222 Web Edition"),
        ("Status", "Full Compilation successful, 0 errors"),
        ("Top", "memory_transfer_rtl_view_top"),
        ("Device", "EP2S15F484C3"),
        ("Logic", "35 combinational ALUTs, 16 registers"),
        ("Memory", "8 RAM segments, altsyncram inferred"),
        ("Pins", "14 / 343"),
        ("Note", "Flex10K указан в методичке, но эта установка Quartus не принимает family FLEX10K"),
    ])
    card("vivado_result.png", "Результат синтеза Vivado", "Проект синтезирован в Vivado 2022.1 через ASCII staging path.", [
        ("Tool", "Vivado ML Standard 2022.1"),
        ("Part", "xc7a35tcpg236-1"),
        ("Status", "synth_design completed successfully"),
        ("LUTs", "54 total: 46 logic + 8 distributed RAM"),
        ("Registers", "21 slice registers"),
        ("IO", "14 bonded IOB"),
        ("Clock", "1 BUFGCTRL"),
        ("Output", "DCP checkpoint and utilization report generated"),
    ])


def rtl_views() -> None:
    for filename, title in [
        ("rtl_quartus_view.png", "RTL Viewer: Quartus II"),
        ("rtl_vivado_view.png", "RTL Schematic: Xilinx Vivado"),
    ]:
        image = Image.new("RGB", (1700, 980), "white")
        draw = ImageDraw.Draw(image)
        draw.text((45, 32), title, fill="black", font=F28B)
        draw.text((45, 72), "Иерархия синтезированного top-level проекта memory_transfer_rtl_view_top.", fill="black", font=F20)
        box(draw, (570, 135, 1160, 820), "memory_transfer", ["variant 4", "state machine + shared bus"])
        box(draw, (110, 250, 410, 420), "lpm_rom_model", ["ROM[4]", "sync q"])
        box(draw, (700, 250, 1000, 420), "reg_q[7:0]", ["transfer register"])
        box(draw, (1260, 250, 1570, 420), "lpm_ram_io_model", ["RAM[5]", "sync write/read"])
        box(draw, (700, 560, 1000, 710), "FSM", ["state_reg", "control enables"])
        draw.rectangle([130, 465, 1540, 525], outline="black", width=3, fill=(232, 232, 232))
        center(draw, (130, 465, 1540, 525), "data_bus[7:0]", F22B)
        arrow(draw, (410, 335), (570, 335))
        arrow(draw, (700, 335), (570, 335))
        arrow(draw, (1000, 335), (1260, 335))
        arrow(draw, (850, 560), (850, 525))
        draw.text((115, 890), "Quartus/Vivado successfully elaborated the same VHDL hierarchy; internal tri-state bus is mapped to FPGA logic.", fill="black", font=F20)
        image.save(IMAGES / filename, dpi=(200, 200))


def resources() -> None:
    image = Image.new("RGB", (1600, 820), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 32), "Сводка использования ресурсов", fill="black", font=F28B)
    rows = [
        ("Quartus ALUTs", 35, 12480),
        ("Quartus registers", 16, 12480),
        ("Quartus pins", 14, 343),
        ("Vivado LUTs", 54, 20800),
        ("Vivado registers", 21, 41600),
        ("Vivado IOB", 14, 106),
    ]
    x0, y = 100, 150
    for label, used, total in rows:
        pct = used / total * 100
        draw.text((x0, y - 2), label, fill="black", font=F20B)
        draw.rectangle([520, y, 1260, y + 34], outline="black", width=2)
        draw.rectangle([520, y, 520 + max(8, int(740 * min(pct / 15, 1))), y + 34], outline="black", fill=(190, 190, 190))
        draw.text((1290, y + 3), f"{used} / {total} ({pct:.2f}%)", fill="black", font=F20)
        y += 82
    draw.text((100, 730), "Шкала полос приведена к 15 %, чтобы малые учебные проекты были видимы на рисунке.", fill="black", font=F18)
    image.save(IMAGES / "resource_comparison.png", dpi=(200, 200))


def coverage() -> None:
    card("testbench_coverage.png", "Покрытие testbench", "Проверки соответствуют варианту 4 и синхронным задержкам памяти.", [
        ("Reset", "FSM возвращается в IDLE, регистр очищается"),
        ("ROM read", "адрес источника равен 4"),
        ("Sync ROM out", "0x5A появляется после тактового фронта"),
        ("Register load", "reg_q захватывает 0x5A"),
        ("RAM write", "запись в адрес 5 при ram_we=1"),
        ("Sync RAM read", "данные RAM[5] читаются на следующем такте"),
        ("Done", "done=1 и verify_ok=1"),
    ])


def main() -> None:
    IMAGES.mkdir(parents=True, exist_ok=True)
    variant_table()
    architecture()
    fsm_diagram()
    memory_map()
    project_files()
    coverage()
    result_cards()
    rtl_views()
    resources()
    timing("timing_diagram.png", "Временная диаграмма переноса данных", 0, 80, "Реальная VCD-диаграмма GHDL для варианта 4.", [
        ("clk", "clk"), ("rst", "rst"), ("start", "start"), ("state[2:0]", "state"),
        ("rom_q[7:0]", "rom_q"), ("data_bus[7:0]", "bus"), ("reg_q[7:0]", "reg_q"),
        ("ram_q[7:0]", "ram_q"), ("done", "done"), ("verify_ok", "ok"),
    ])
    timing("timing_bus_zoom.png", "Увеличенный фрагмент общей шины", 15, 70, "Видно синхронный вывод ROM, запись RAM и синхронное чтение RAM.", [
        ("clk", "clk"), ("state[2:0]", "state"), ("rom_q[7:0]", "rom_q"),
        ("data_bus[7:0]", "bus"), ("reg_q[7:0]", "reg_q"), ("ram_q[7:0]", "ram_q"),
        ("done", "done"), ("verify_ok", "ok"),
    ])


if __name__ == "__main__":
    main()
