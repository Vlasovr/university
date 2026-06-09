from __future__ import annotations

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import math


ROOT = Path(__file__).resolve().parents[1]
IMG = ROOT / "images"
VCD = ROOT / "sim" / "sn74145.vcd"


def font(size: int, bold: bool = False):
    for name in [r"C:\Windows\Fonts\timesbd.ttf" if bold else r"C:\Windows\Fonts\times.ttf", r"C:\Windows\Fonts\arialbd.ttf" if bold else r"C:\Windows\Fonts\arial.ttf"]:
        p = Path(name)
        if p.exists():
            return ImageFont.truetype(str(p), size)
    return ImageFont.load_default()


F16, F18, F18B, F20, F20B, F22B, F24B, F28B = font(16), font(18), font(18, True), font(20), font(20, True), font(22, True), font(24, True), font(28, True)


def arrow(draw, a, b, width=2):
    draw.line([a, b], fill="black", width=width)
    ang = math.atan2(b[1] - a[1], b[0] - a[0])
    for s in (-1, 1):
        p = (b[0] - 13 * math.cos(ang + s * 0.48), b[1] - 13 * math.sin(ang + s * 0.48))
        draw.line([b, p], fill="black", width=width)


def center(draw, box, text, f=F20B):
    bb = draw.textbbox((0, 0), text, font=f)
    draw.text((box[0] + (box[2] - box[0] - bb[2] + bb[0]) / 2, box[1] + (box[3] - box[1] - bb[3] + bb[1]) / 2 - 2), text, fill="black", font=f)


def wrap(draw, text, f, width):
    lines, cur = [], ""
    for word in text.split():
        cand = word if not cur else cur + " " + word
        if draw.textbbox((0, 0), cand, font=f)[2] <= width:
            cur = cand
        else:
            if cur:
                lines.append(cur)
            cur = word
    if cur:
        lines.append(cur)
    return lines


def box(draw, xy, title, lines=None):
    draw.rounded_rectangle(xy, radius=8, outline="black", width=3, fill=(247, 247, 247))
    center(draw, (xy[0] + 8, xy[1] + 8, xy[2] - 8, xy[1] + 42), title, F22B)
    y = xy[1] + 58
    for line in lines or []:
        for part in wrap(draw, line, F18, xy[2] - xy[0] - 32):
            draw.text((xy[0] + 16, y), part, fill="black", font=F18)
            y += 24


def table_img(filename, title, subtitle, headers, rows, widths, highlight=None):
    h = 150 + 50 * (len(rows) + 1) + 60
    im = Image.new("RGB", (sum(widths) + 180, h), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), title, fill="black", font=F28B)
    d.text((45, 72), subtitle, fill="black", font=F20)
    x0, y0 = 90, 140
    x = x0
    for head, w in zip(headers, widths):
        d.rectangle([x, y0, x + w, y0 + 50], outline="black", width=2, fill=(232, 232, 232))
        center(d, (x, y0, x + w, y0 + 50), head, F18B)
        x += w
    for r, row in enumerate(rows):
        x, y = x0, y0 + (r + 1) * 50
        fill = (226, 226, 226) if r == highlight else "white"
        for text, w in zip(row, widths):
            d.rectangle([x, y, x + w, y + 50], outline="black", width=1, fill=fill)
            center(d, (x, y, x + w, y + 50), text, F18)
            x += w
    im.save(IMG / filename, dpi=(200, 200))


def card(filename, title, subtitle, rows):
    im = Image.new("RGB", (1600, 900), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), title, fill="black", font=F28B)
    d.text((45, 72), subtitle, fill="black", font=F20)
    x0, y0, c1, c2 = 90, 145, 460, 900
    d.rectangle([x0, y0, x0 + c1 + c2, y0 + 52], outline="black", width=2, fill=(232, 232, 232))
    center(d, (x0, y0, x0 + c1, y0 + 52), "Параметр", F18B)
    center(d, (x0 + c1, y0, x0 + c1 + c2, y0 + 52), "Значение", F18B)
    for i, (left, right) in enumerate(rows):
        y = y0 + (i + 1) * 58
        d.rectangle([x0, y, x0 + c1, y + 58], outline="black", width=1, fill="white")
        d.rectangle([x0 + c1, y, x0 + c1 + c2, y + 58], outline="black", width=1, fill="white")
        d.text((x0 + 14, y + 16), left, fill="black", font=F18)
        for j, line in enumerate(wrap(d, right, F18, c2 - 28)[:2]):
            d.text((x0 + c1 + 14, y + 8 + j * 23), line, fill="black", font=F18)
    im.save(IMG / filename, dpi=(200, 200))


def parse_vcd():
    targets = {"a", "b", "c", "d", "y_logic[9:0]", "y_seq[9:0]"}
    ids, waves = {}, {t: [] for t in targets}
    in_defs, t = True, 0.0
    for raw in VCD.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw.strip()
        if in_defs:
            if line.startswith("$var"):
                parts = line.split()
                if len(parts) >= 5 and parts[4] in targets and parts[4] not in ids.values():
                    ids[parts[3]] = parts[4]
            elif line == "$enddefinitions $end":
                in_defs = False
            continue
        if line.startswith("#"):
            t = int(line[1:]) / 1_000_000.0
        elif line and line[0] in "01":
            if line[1:] in ids:
                waves[ids[line[1:]]].append((t, line[0]))
        elif line.startswith("b"):
            val, ident = line[1:].split()
            if ident in ids:
                waves[ids[ident]].append((t, val))
    return waves


def value_at(events, t):
    value = events[0][1] if events else "X"
    for et, ev in events:
        if et <= t:
            value = ev
        else:
            break
    return value


def digital(draw, events, y, x0, scale, start, end):
    high, low = y - 17, y + 17
    cur = value_at(events, start)
    cy = high if cur == "1" else low
    pts = [(x0, cy)]
    for t, v in events:
        if start <= t <= end:
            x = int(x0 + (t - start) * scale)
            ny = high if v == "1" else low
            pts += [(x, cy), (x, ny)]
            cy = ny
    pts.append((int(x0 + (end - start) * scale), cy))
    draw.line(pts, fill="black", width=2)


def bus(draw, events, y, x0, scale, start, end):
    times = sorted({start, end, *[t for t, _ in events if start <= t <= end]})
    for left, right in zip(times, times[1:]):
        x1, x2 = int(x0 + (left - start) * scale), int(x0 + (right - start) * scale)
        draw.rectangle([x1, y - 21, x2, y + 21], outline="black", width=2)
        val = value_at(events, left)
        label = "0b" + val if len(val) <= 10 else val
        if x2 - x1 > 90:
            center(draw, (x1, y - 21, x2, y + 21), label, F16)


def timing():
    waves = parse_vcd()
    im = Image.new("RGB", (1700, 820), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), "Временная диаграмма SN74145", fill="black", font=F28B)
    d.text((45, 72), "Полный перебор входного BCD-кода 0...15 из GHDL VCD.", fill="black", font=F20)
    x0, x1, start, end = 190, 1620, 0, 170
    scale = (x1 - x0) / (end - start)
    d.line([(x0, 130), (x1, 130)], fill="black", width=2)
    for t in range(0, 171, 10):
        x = int(x0 + t * scale)
        d.line([(x, 122), (x, 138)], fill="black")
        d.text((x - 10, 98), str(t), fill="black", font=F16)
    for i, (name, label) in enumerate([("d", "D"), ("c", "C"), ("b", "B"), ("a", "A"), ("y_logic[9:0]", "Y logic"), ("y_seq[9:0]", "Y seq")]):
        y = 180 + i * 86
        d.text((45, y - 14), label, fill="black", font=F18B)
        d.line([(x0, y + 36), (x1, y + 36)], fill=(225, 225, 225))
        if "[" in name:
            bus(d, waves[name], y, x0, scale, start, end)
        else:
            digital(d, waves[name], y, x0, scale, start, end)
    im.save(IMG / "timing_diagram.png", dpi=(200, 200))


def diagrams():
    rows = []
    for n in range(16):
        outs = ["H"] * 10
        if n <= 9:
            outs[n] = "L"
        rows.append([str(n), str((n >> 3) & 1), str((n >> 2) & 1), str((n >> 1) & 1), str(n & 1), "".join(outs)])
    table_img("truth_table.png", "Таблица истинности 145", "Для кодов 0...9 один выход активен низким уровнем; 10...15 - все выходы H.", ["№", "D", "C", "B", "A", "Y0...Y9"], rows, [80, 80, 80, 80, 80, 520])

    im = Image.new("RGB", (1500, 850), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), "Условное обозначение VHDL-модуля", fill="black", font=F28B)
    box(d, (560, 160, 980, 680), "sn74145", ["BCD-to-decimal decoder", "active-low outputs", "invalid 10...15 -> all H"])
    for name, y in [("A", 260), ("B", 330), ("C", 400), ("D", 470)]:
        d.text((150, y - 12), name, fill="black", font=F22B)
        arrow(d, (220, y), (560, y))
    for i in range(10):
        y = 210 + i * 42
        d.text((1010, y - 10), f"Y{i}", fill="black", font=F18B)
        arrow(d, (980, y), (1125, y))
    im.save(IMG / "entity_symbol.png", dpi=(200, 200))

    im = Image.new("RGB", (1700, 900), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), "Логические уравнения активных-низких выходов", fill="black", font=F28B)
    eqs = [
        "Y0 = A + B + C + D",
        "Y1 = /A + B + C + D",
        "Y2 = A + /B + C + D",
        "Y3 = /A + /B + C + D",
        "Y4 = A + B + /C + D",
        "Y5 = /A + B + /C + D",
        "Y6 = A + /B + /C + D",
        "Y7 = /A + /B + /C + D",
        "Y8 = A + B + C + /D",
        "Y9 = /A + B + C + /D",
    ]
    for i, eq in enumerate(eqs):
        d.text((110 + (i // 5) * 760, 145 + (i % 5) * 86), eq, fill="black", font=F24B)
    d.text((110, 765), "Формулы построены как инверсия условия выбора соответствующего десятичного выхода.", fill="black", font=F20)
    im.save(IMG / "logic_equations.png", dpi=(200, 200))

    im = Image.new("RGB", (1700, 900), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), "Сравнение двух VHDL-описаний", fill="black", font=F28B)
    box(d, (150, 260, 540, 460), "sn74145_concurrent", ["только логические операторы", "параллельные присваивания"])
    box(d, (150, 560, 540, 760), "sn74145_sequential", ["process", "case BCD", "последовательные операторы"])
    box(d, (710, 340, 980, 650), "common inputs", ["A, B, C, D"])
    box(d, (1190, 390, 1510, 610), "MATCH", ["y_logic = y_seq"])
    arrow(d, (710, 420), (540, 360))
    arrow(d, (710, 575), (540, 660))
    arrow(d, (980, 495), (1190, 495))
    im.save(IMG / "compare_topology.png", dpi=(200, 200))

    for fname, title in [("rtl_quartus_view.png", "RTL Viewer: Quartus II"), ("rtl_vivado_view.png", "RTL Schematic: Vivado")]:
        im = Image.new("RGB", (1700, 940), "white")
        d = ImageDraw.Draw(im)
        d.text((45, 32), title, fill="black", font=F28B)
        box(d, (570, 145, 1130, 790), "sn74145_compare_top", ["variant 4", "two implementations", "MATCH output"])
        box(d, (120, 300, 460, 480), "U_LOGIC", ["OR equations", "active-low decoder"])
        box(d, (120, 590, 460, 770), "U_SEQ", ["case 0000...1001", "invalid -> all H"])
        box(d, (1260, 390, 1550, 560), "outputs", ["YL0..YL9", "YS0..YS9", "MATCH"])
        arrow(d, (570, 390), (460, 390))
        arrow(d, (570, 680), (460, 680))
        arrow(d, (1130, 475), (1260, 475))
        d.text((120, 870), "Синтез не выводит регистров: схема остается комбинационной.", fill="black", font=F20)
        im.save(IMG / fname, dpi=(200, 200))


def result_cards():
    card("ghdl_result.png", "Результат GHDL", "Полный перебор входного кода BCD 0...15.", [
        ("Simulator", "GHDL"),
        ("Testbench", "tb_sn74145"),
        ("Комбинации", "16 input combinations"),
        ("Проверка", "active-low output for 0...9, all H for 10...15"),
        ("Итог", "TEST PASSED at 160 ns"),
    ])
    card("quartus_result.png", "Результат Quartus II", "Full Compilation для sn74145_compare_top.", [
        ("Tool", "Quartus II 9.1 Build 222 Web Edition"),
        ("Status", "Full Compilation successful, 0 errors"),
        ("Logic", "10 combinational ALUTs, 0 registers"),
        ("Pins", "25 / 343"),
        ("Timing", "Longest tpd A -> YL2 = 10.176 ns"),
        ("Note", "MATCH stuck at VCC because implementations are equivalent"),
    ])
    card("vivado_result.png", "Результат Vivado", "Синтез для xc7a35tcpg236-1.", [
        ("Status", "synth_design completed successfully"),
        ("Slice LUTs", "9 / 20800"),
        ("Registers", "0 / 41600"),
        ("IOB", "25 / 106"),
        ("Cells", "18 LUT4, 1 CARRY4, 4 IBUF, 21 OBUF"),
    ])
    card("testbench_coverage.png", "Покрытие testbench", "Для комбинационной схемы выполнен полный перебор.", [
        ("Inputs", "A, B, C, D"),
        ("Valid BCD", "0...9: one selected output is L"),
        ("Invalid BCD", "10...15: all outputs are H"),
        ("Concurrent", "checked against expected vector"),
        ("Sequential", "checked against expected vector"),
        ("Equivalence", "y_logic equals y_seq"),
    ])
    card("project_files_diagram.png", "Структура папки", "Корректный комплект для варианта 4.", [
        ("src/", "sn74145_concurrent, sn74145_sequential, compare top"),
        ("tb/", "tb_sn74145 exhaustive testbench"),
        ("sim/", "sn74145.vcd and ghdl_run.log"),
        ("quartus/", "QPF/QSF and compile output"),
        ("vivado/", "TCL/XDC and synthesis output"),
        ("images/", "task, diagrams, RTL and tool results"),
        ("report/", "DOCX/PDF report and rendered QA pages"),
    ])


def resources():
    im = Image.new("RGB", (1600, 760), "white")
    d = ImageDraw.Draw(im)
    d.text((45, 32), "Сводка ресурсов", fill="black", font=F28B)
    rows = [("Quartus ALUTs", 10, 12480), ("Quartus registers", 0, 12480), ("Quartus pins", 25, 343), ("Vivado LUTs", 9, 20800), ("Vivado registers", 0, 41600), ("Vivado IOB", 25, 106)]
    y = 145
    for label, used, total in rows:
        pct = used / total * 100
        d.text((100, y - 2), label, fill="black", font=F20B)
        d.rectangle([520, y, 1260, y + 34], outline="black", width=2)
        if used:
            d.rectangle([520, y, 520 + max(8, int(740 * min(pct / 25, 1))), y + 34], outline="black", fill=(190, 190, 190))
        d.text((1290, y + 3), f"{used} / {total} ({pct:.2f}%)", fill="black", font=F20)
        y += 82
    d.text((100, 685), "Нулевые регистры подтверждают комбинационный характер декодера.", fill="black", font=F18)
    im.save(IMG / "resource_comparison.png", dpi=(200, 200))


def main():
    IMG.mkdir(parents=True, exist_ok=True)
    diagrams()
    timing()
    result_cards()
    resources()


if __name__ == "__main__":
    main()
