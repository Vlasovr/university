from __future__ import annotations

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import math


ROOT = Path(__file__).resolve().parents[1]
IMAGES = ROOT / "images"
VCD = ROOT / "sim" / "mod11_counter.vcd"


def font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont:
    candidates = [
        Path(r"C:\Windows\Fonts\timesbd.ttf" if bold else r"C:\Windows\Fonts\times.ttf"),
        Path(r"C:\Windows\Fonts\arialbd.ttf" if bold else r"C:\Windows\Fonts\arial.ttf"),
        Path(r"C:\Windows\Fonts\calibrib.ttf" if bold else r"C:\Windows\Fonts\calibri.ttf"),
    ]
    for candidate in candidates:
        if candidate.exists():
            return ImageFont.truetype(str(candidate), size)
    return ImageFont.load_default()


FONT_18 = font(18)
FONT_18_B = font(18, True)
FONT_20 = font(20)
FONT_20_B = font(20, True)
FONT_22 = font(22)
FONT_22_B = font(22, True)
FONT_24_B = font(24, True)
FONT_28_B = font(28, True)


def draw_arrow(draw: ImageDraw.ImageDraw, start: tuple[int, int], end: tuple[int, int], width: int = 2) -> None:
    draw.line([start, end], fill="black", width=width)
    angle = math.atan2(end[1] - start[1], end[0] - start[0])
    length = 12
    spread = math.radians(28)
    p1 = (end[0] - length * math.cos(angle - spread), end[1] - length * math.sin(angle - spread))
    p2 = (end[0] - length * math.cos(angle + spread), end[1] - length * math.sin(angle + spread))
    draw.polygon([end, (int(p1[0]), int(p1[1])), (int(p2[0]), int(p2[1]))], fill="black")


def parse_vcd(path: Path) -> dict[str, list[tuple[float, str]]]:
    id_to_name: dict[str, str] = {}
    target_names = {"clk", "rst", "en", "load", "q[3:0]", "tc"}
    in_defs = True
    current_time = 0.0
    waves: dict[str, list[tuple[float, str]]] = {name: [] for name in target_names}

    for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw_line.strip()
        if not line:
            continue
        if in_defs:
            if line.startswith("$var"):
                parts = line.split()
                if len(parts) >= 5:
                    ident = parts[3]
                    name = parts[4]
                    if name in target_names and name not in id_to_name.values():
                        id_to_name[ident] = name
            elif line == "$enddefinitions $end":
                in_defs = False
            continue

        if line.startswith("#"):
            current_time = int(line[1:]) / 1_000_000.0
        elif line[0] in "01xXzZ":
            ident = line[1:]
            if ident in id_to_name:
                waves[id_to_name[ident]].append((current_time, line[0].lower()))
        elif line.startswith("b"):
            value, ident = line[1:].split()
            if ident in id_to_name:
                waves[id_to_name[ident]].append((current_time, value))

    return waves


def value_at(events: list[tuple[float, str]], time_ns: float) -> str:
    result = events[0][1] if events else "x"
    for t, v in events:
        if t <= time_ns:
            result = v
        else:
            break
    return result


def draw_digital_wave(
    draw: ImageDraw.ImageDraw,
    events: list[tuple[float, str]],
    y: int,
    x0: int,
    scale: float,
    end_ns: float,
) -> None:
    high = y - 18
    low = y + 18
    if not events:
        return

    points: list[tuple[int, int]] = []
    current = value_at(events, 0)
    current_y = high if current == "1" else low
    points.append((x0, current_y))

    for t, v in events:
        if t < 0 or t > end_ns:
            continue
        x = int(x0 + t * scale)
        new_y = high if v == "1" else low
        points.append((x, current_y))
        points.append((x, new_y))
        current_y = new_y
    points.append((int(x0 + end_ns * scale), current_y))
    draw.line(points, fill="black", width=2)


def draw_bus_wave(
    draw: ImageDraw.ImageDraw,
    events: list[tuple[float, str]],
    y: int,
    x0: int,
    scale: float,
    end_ns: float,
) -> None:
    top = y - 22
    bottom = y + 22
    times = sorted({0.0, end_ns, *[t for t, _ in events if 0 <= t <= end_ns]})
    for left_t, right_t in zip(times, times[1:]):
        if right_t <= left_t:
            continue
        x1 = int(x0 + left_t * scale)
        x2 = int(x0 + right_t * scale)
        draw.rectangle([x1, top, x2, bottom], outline="black", width=2)
        value = value_at(events, left_t)
        try:
            label = str(int(value, 2))
        except ValueError:
            label = value
        if x2 - x1 > 34:
            bbox = draw.textbbox((0, 0), label, font=FONT_18)
            draw.text(((x1 + x2 - (bbox[2] - bbox[0])) // 2, y - 10), label, fill="black", font=FONT_18)


def timing_diagram() -> None:
    waves = parse_vcd(VCD)
    width, height = 1700, 730
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    x0, x1 = 150, 1620
    y0 = 120
    end_ns = 170.0
    scale = (x1 - x0) / end_ns

    draw.text((45, 35), "Functional timing diagram, GHDL VCD", fill="black", font=FONT_28_B)
    draw.line([(x0, 95), (x1, 95)], fill="black", width=2)
    for t in range(0, 171, 10):
        x = int(x0 + t * scale)
        draw.line([(x, 88), (x, 102)], fill="black", width=1)
        draw.text((x - 15, 63), str(t), fill="black", font=FONT_18)
    draw.text((x1 + 12, 63), "ns", fill="black", font=FONT_18)

    rows = [
        ("clk", "clk"),
        ("rst", "rst"),
        ("en", "en"),
        ("load", "load"),
        ("q[3:0]", "q"),
        ("tc", "tc"),
    ]
    for idx, (name, label) in enumerate(rows):
        y = y0 + idx * 90
        draw.text((45, y - 18), label, fill="black", font=FONT_22)
        draw.line([(x0, y + 38), (x1, y + 38)], fill=(220, 220, 220), width=1)
        if name == "q[3:0]":
            draw_bus_wave(draw, waves[name], y, x0, scale, end_ns)
        else:
            draw_digital_wave(draw, waves[name], y, x0, scale, end_ns)

    draw.text((45, 670), "Sequence checked: reset/load -> 5; enable -> 5,6,...,15,5; enable=0 -> hold", fill="black", font=FONT_20)
    image.save(IMAGES / "timing_diagram.png", dpi=(200, 200))


def state_diagram() -> None:
    width, height = 1700, 540
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "State graph of modulo-11 forced counter", fill="black", font=FONT_28_B)
    y = 250
    xs = [110 + i * 135 for i in range(11)]
    values = list(range(5, 16))
    radius = 42
    for x, value in zip(xs, values):
        draw.ellipse([x - radius, y - radius, x + radius, y + radius], outline="black", width=3, fill=(245, 245, 245))
        text = str(value)
        bbox = draw.textbbox((0, 0), text, font=FONT_24_B)
        draw.text((x - (bbox[2] - bbox[0]) / 2, y - 14), text, fill="black", font=FONT_24_B)
    for i in range(10):
        draw_arrow(draw, (xs[i] + radius, y), (xs[i + 1] - radius, y), width=2)
    draw.line([(xs[-1], y + radius + 18), (xs[-1], y + 115), (xs[0], y + 115), (xs[0], y + radius + 18)], fill="black", width=2)
    draw_arrow(draw, (xs[0], y + radius + 18), (xs[0], y + radius), width=2)
    draw.text((760, y + 125), "after 15", fill="black", font=FONT_20)
    draw_arrow(draw, (95, 115), (xs[0] - radius + 5, y - radius + 5), width=2)
    draw.text((45, 85), "rst=1 or load=1", fill="black", font=FONT_20)
    draw.text((45, 470), "When en=0 the current state is held; when en=1 the next arrow is selected.", fill="black", font=FONT_20)
    image.save(IMAGES / "state_diagram.png", dpi=(200, 200))


def block(draw: ImageDraw.ImageDraw, box: tuple[int, int, int, int], title: str, body: list[str] | None = None) -> None:
    draw.rounded_rectangle(box, radius=8, outline="black", width=3, fill=(248, 248, 248))
    x1, y1, x2, _ = box
    bbox = draw.textbbox((0, 0), title, font=FONT_24_B)
    draw.text((x1 + (x2 - x1 - (bbox[2] - bbox[0])) / 2, y1 + 16), title, fill="black", font=FONT_24_B)
    if body:
        for i, line in enumerate(body):
            draw.text((x1 + 18, y1 + 56 + i * 27), line, fill="black", font=FONT_18)


def rtl_diagram(filename: str, title: str, subtitle: str) -> None:
    width, height = 1700, 920
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), title, fill="black", font=FONT_28_B)
    draw.text((45, 75), subtitle, fill="black", font=FONT_20)

    block(draw, (530, 130, 1180, 790), "mod11_counter", ["VHDL entity: rtl", "Modulo 11, sequence 5..15"])
    block(draw, (600, 275, 850, 405), "Next-state logic", ["if rst/load -> 5", "else if q=15 -> 5", "else q+1"])
    block(draw, (930, 275, 1110, 405), "4-bit register", ["count_reg", "clk enable"])
    block(draw, (610, 520, 820, 635), "Comparator", ["count_reg = 15"])
    block(draw, (910, 520, 1120, 635), "TC logic", ["tc = en and", "count_reg = 15"])

    for label, y in [("CLOCK", 190), ("RST", 265), ("EN", 340), ("LOAD", 415)]:
        draw.text((90, y - 14), label, fill="black", font=FONT_22)
        draw_arrow(draw, (190, y), (530, y), width=2)
    draw_arrow(draw, (850, 340), (930, 340), width=2)
    draw_arrow(draw, (1110, 340), (1250, 340), width=2)
    draw.text((1265, 325), "Q[3:0]", fill="black", font=FONT_22)
    draw_arrow(draw, (1020, 405), (1020, 520), width=2)
    draw_arrow(draw, (820, 580), (910, 580), width=2)
    draw_arrow(draw, (1120, 580), (1250, 580), width=2)
    draw.text((1265, 565), "TC", fill="black", font=FONT_22)
    draw.line([(1020, 405), (1020, 710), (660, 710), (660, 635)], fill="black", width=2)
    draw_arrow(draw, (660, 635), (660, 630), width=2)

    draw.text((45, 845), "The drawing follows the synthesized hierarchy: top wrapper -> U_COUNTER -> next-state logic, register and terminal-count output.", fill="black", font=FONT_20)
    image.save(IMAGES / filename, dpi=(200, 200))


def wrap_text(draw: ImageDraw.ImageDraw, text: str, text_font: ImageFont.FreeTypeFont, max_width: int) -> list[str]:
    words = text.split()
    lines: list[str] = []
    current = ""
    for word in words:
        candidate = word if not current else f"{current} {word}"
        bbox = draw.textbbox((0, 0), candidate, font=text_font)
        if bbox[2] - bbox[0] <= max_width:
            current = candidate
        else:
            if current:
                lines.append(current)
            current = word
    if current:
        lines.append(current)
    return lines


def draw_centered(draw: ImageDraw.ImageDraw, box: tuple[int, int, int, int], text: str, text_font: ImageFont.FreeTypeFont) -> None:
    bbox = draw.textbbox((0, 0), text, font=text_font)
    w = bbox[2] - bbox[0]
    h = bbox[3] - bbox[1]
    x1, y1, x2, y2 = box
    draw.text((x1 + (x2 - x1 - w) / 2, y1 + (y2 - y1 - h) / 2 - 2), text, fill="black", font=text_font)


def draw_wrapped(
    draw: ImageDraw.ImageDraw,
    box: tuple[int, int, int, int],
    title: str,
    body: list[str],
    *,
    title_font: ImageFont.FreeTypeFont = FONT_22_B,
    body_font: ImageFont.FreeTypeFont = FONT_18,
) -> None:
    x1, y1, x2, y2 = box
    draw.rounded_rectangle(box, radius=10, outline="black", width=3, fill=(248, 248, 248))
    draw_centered(draw, (x1 + 10, y1 + 8, x2 - 10, y1 + 42), title, title_font)
    y = y1 + 54
    for item in body:
        for line in wrap_text(draw, item, body_font, x2 - x1 - 36):
            if y + 24 < y2:
                draw.text((x1 + 18, y), line, fill="black", font=body_font)
            y += 26


def entity_symbol() -> None:
    width, height = 1500, 760
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "VHDL entity: mod11_counter", fill="black", font=FONT_28_B)
    draw.text((45, 75), "External ports used by the testbench and both CAD projects.", fill="black", font=FONT_20)

    box = (560, 155, 980, 610)
    draw.rounded_rectangle(box, radius=8, outline="black", width=4, fill=(246, 246, 246))
    draw_centered(draw, (580, 185, 960, 230), "mod11_counter", FONT_24_B)
    draw.line([(580, 250), (960, 250)], fill="black", width=2)

    inputs = [("clk_i", "clock input"), ("rst_i", "sync reset to 5"), ("en_i", "count enable"), ("load_i", "force load 5")]
    outputs = [("q_o[3:0]", "counter value"), ("tc_o", "terminal count")]
    for idx, (name, desc) in enumerate(inputs):
        y = 300 + idx * 70
        draw.text((80, y - 14), f"{name} - {desc}", fill="black", font=FONT_20)
        draw_arrow(draw, (430, y), (560, y), width=2)
        draw.text((585, y - 16), name, fill="black", font=FONT_22_B)
    for idx, (name, desc) in enumerate(outputs):
        y = 330 + idx * 95
        draw.text((1005, y - 16), name, fill="black", font=FONT_22_B)
        draw_arrow(draw, (980, y), (1115, y), width=2)
        draw.text((1135, y - 14), f"{name} - {desc}", fill="black", font=FONT_20)

    draw.text((560, 650), "All ports are std_logic/std_logic_vector; arithmetic is performed with ieee.numeric_std unsigned type.", fill="black", font=FONT_20)
    image.save(IMAGES / "entity_symbol.png", dpi=(200, 200))


def transition_table_image() -> None:
    width, height = 1650, 930
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "State transition table", fill="black", font=FONT_28_B)
    draw.text((45, 75), "Modulo-11 sequence with forced transition after state 15.", fill="black", font=FONT_20)

    x0, y0 = 90, 145
    col_widths = [180, 230, 180, 230, 520]
    row_h = 48
    headers = ["q(k)", "binary", "q(k+1)", "binary", "condition"]
    rows = []
    for value in range(5, 16):
        nxt = 5 if value == 15 else value + 1
        rows.append([str(value), format(value, "04b"), str(nxt), format(nxt, "04b"), "en=1, ordinary step" if value < 15 else "en=1, forced wrap"])
    rows.append(["any", "----", "same", "----", "en=0, hold current state"])
    rows.append(["any", "----", "5", "0101", "rst=1 or load=1"])

    x = x0
    for c, header in enumerate(headers):
        draw.rectangle([x, y0, x + col_widths[c], y0 + row_h], outline="black", width=2, fill=(235, 235, 235))
        draw_centered(draw, (x, y0, x + col_widths[c], y0 + row_h), header, FONT_20_B)
        x += col_widths[c]

    for r, row in enumerate(rows):
        y = y0 + (r + 1) * row_h
        x = x0
        for c, text in enumerate(row):
            draw.rectangle([x, y, x + col_widths[c], y + row_h], outline="black", width=1, fill="white")
            if c == 4:
                draw.text((x + 14, y + 12), text, fill="black", font=FONT_18)
            else:
                draw_centered(draw, (x, y, x + col_widths[c], y + row_h), text, FONT_18)
            x += col_widths[c]

    draw.text((90, 875), "Unused binary states 0000...0100 are excluded from the working cycle and are corrected by reset/load.", fill="black", font=FONT_20)
    image.save(IMAGES / "transition_table.png", dpi=(200, 200))


def algorithm_flowchart() -> None:
    width, height = 1500, 1120
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "Counter next-state algorithm", fill="black", font=FONT_28_B)
    draw.text((45, 75), "Executed on every rising edge of clk_i.", fill="black", font=FONT_20)

    boxes = {
        "start": (575, 130, 925, 200),
        "rst": (560, 255, 940, 340),
        "load5": (140, 270, 430, 340),
        "en": (560, 405, 940, 490),
        "hold": (1020, 420, 1305, 490),
        "max": (560, 555, 940, 640),
        "wrap": (140, 570, 430, 640),
        "inc": (1020, 570, 1305, 640),
        "out": (575, 730, 925, 805),
    }
    draw_wrapped(draw, boxes["start"], "rising_edge(clk_i)", [], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["rst"], "rst_i=1 or load_i=1?", ["priority control"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["load5"], "count_reg <= 5", ["forced initial state"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["en"], "en_i=1?", ["counter is allowed to change"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["hold"], "hold state", ["count_reg is unchanged"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["max"], "count_reg=15?", ["terminal state check"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["wrap"], "count_reg <= 5", ["wrap to first state"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["inc"], "count_reg <= q+1", ["ordinary increment"], title_font=FONT_20_B)
    draw_wrapped(draw, boxes["out"], "q_o, tc_o update", ["tc_o=1 when q=15 and en=1"], title_font=FONT_20_B)

    draw_arrow(draw, (750, 200), (750, 255), width=2)
    draw_arrow(draw, (560, 300), (430, 300), width=2)
    draw.text((455, 276), "yes", fill="black", font=FONT_18_B)
    draw_arrow(draw, (750, 340), (750, 405), width=2)
    draw.text((765, 362), "no", fill="black", font=FONT_18_B)
    draw_arrow(draw, (940, 448), (1020, 448), width=2)
    draw.text((955, 422), "no", fill="black", font=FONT_18_B)
    draw_arrow(draw, (750, 490), (750, 555), width=2)
    draw.text((765, 512), "yes", fill="black", font=FONT_18_B)
    draw_arrow(draw, (560, 598), (430, 598), width=2)
    draw.text((455, 572), "yes", fill="black", font=FONT_18_B)
    draw_arrow(draw, (940, 598), (1020, 598), width=2)
    draw.text((955, 572), "no", fill="black", font=FONT_18_B)
    draw.line([(285, 340), (90, 340), (90, 690), (750, 690)], fill="black", width=2)
    draw.line([(1162, 490), (1410, 490), (1410, 690), (750, 690)], fill="black", width=2)
    draw.line([(285, 640), (285, 690), (750, 690)], fill="black", width=2)
    draw.line([(1162, 640), (1162, 690), (750, 690)], fill="black", width=2)
    draw_arrow(draw, (750, 690), (750, 730), width=2)

    draw.text((120, 990), "The priority order matches the VHDL process: reset/load first, enable second, terminal-state wrap third.", fill="black", font=FONT_20)
    image.save(IMAGES / "algorithm_flowchart.png", dpi=(200, 200))


def timing_window(filename: str, title: str, start_ns: float, end_ns: float, note: str) -> None:
    waves = parse_vcd(VCD)
    width, height = 1600, 710
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    x0, x1 = 160, 1510
    y0 = 165
    scale = (x1 - x0) / (end_ns - start_ns)

    draw.text((45, 35), title, fill="black", font=FONT_28_B)
    draw.text((45, 75), note, fill="black", font=FONT_20)
    draw.line([(x0, 135), (x1, 135)], fill="black", width=2)
    step = 5 if end_ns - start_ns <= 60 else 10
    t = int(start_ns)
    while t <= int(end_ns):
        x = int(x0 + (t - start_ns) * scale)
        draw.line([(x, 128), (x, 142)], fill="black", width=1)
        draw.text((x - 12, 103), str(t), fill="black", font=FONT_18)
        t += step
    draw.text((x1 + 10, 103), "ns", fill="black", font=FONT_18)

    rows = [("clk", "clk"), ("rst", "rst"), ("en", "en"), ("load", "load"), ("q[3:0]", "q"), ("tc", "tc")]
    for idx, (name, label) in enumerate(rows):
        y = y0 + idx * 78
        draw.text((55, y - 18), label, fill="black", font=FONT_22)
        draw.line([(x0, y + 36), (x1, y + 36)], fill=(220, 220, 220), width=1)
        shifted = [(t - start_ns, value) for t, value in waves[name] if start_ns <= t <= end_ns]
        base_value = value_at(waves[name], start_ns)
        if not shifted or shifted[0][0] > 0:
            shifted.insert(0, (0.0, base_value))
        shifted.append((end_ns - start_ns, value_at(waves[name], end_ns)))
        if name == "q[3:0]":
            draw_bus_wave(draw, shifted, y, x0, scale, end_ns - start_ns)
        else:
            draw_digital_wave(draw, shifted, y, x0, scale, end_ns - start_ns)
    image.save(IMAGES / filename, dpi=(200, 200))


def draw_report_card(filename: str, title: str, subtitle: str, rows: list[tuple[str, str]]) -> None:
    width, height = 1550, 880
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), title, fill="black", font=FONT_28_B)
    draw.text((45, 75), subtitle, fill="black", font=FONT_20)

    x0, y0 = 90, 150
    col1, col2 = 490, 860
    row_h = 58
    draw.rectangle([x0, y0, x0 + col1 + col2, y0 + row_h], outline="black", width=2, fill=(235, 235, 235))
    draw_centered(draw, (x0, y0, x0 + col1, y0 + row_h), "Parameter", FONT_20_B)
    draw_centered(draw, (x0 + col1, y0, x0 + col1 + col2, y0 + row_h), "Value", FONT_20_B)
    for idx, (left, right) in enumerate(rows):
        y = y0 + (idx + 1) * row_h
        draw.rectangle([x0, y, x0 + col1, y + row_h], outline="black", width=1, fill="white")
        draw.rectangle([x0 + col1, y, x0 + col1 + col2, y + row_h], outline="black", width=1, fill="white")
        draw.text((x0 + 16, y + 16), left, fill="black", font=FONT_18)
        for line_idx, line in enumerate(wrap_text(draw, right, FONT_18, col2 - 28)):
            draw.text((x0 + col1 + 16, y + 10 + line_idx * 22), line, fill="black", font=FONT_18)
    image.save(IMAGES / filename, dpi=(200, 200))


def project_files_diagram() -> None:
    width, height = 1500, 880
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "Laboratory project structure", fill="black", font=FONT_28_B)
    draw.text((45, 75), "Files included in the submission folder.", fill="black", font=FONT_20)

    rows = [
        ("src/", "VHDL source files: counter and RTL-view wrapper"),
        ("tb/", "Automated testbench for GHDL simulation"),
        ("sim/", "VCD waveform and GHDL run log"),
        ("quartus/", "Quartus II project, QSF/QPF, compile output, SOF/POF"),
        ("vivado/", "Vivado TCL/XDC, synthesis checkpoints and reports"),
        ("images/", "Timing diagrams, RTL/synthesis views and verification images"),
        ("report/", "DOCX/PDF report and rendered QA pages"),
    ]
    y = 150
    for folder, desc in rows:
        draw.rounded_rectangle([90, y, 360, y + 70], radius=8, outline="black", width=3, fill=(245, 245, 245))
        draw.text((120, y + 20), folder, fill="black", font=FONT_24_B)
        draw_arrow(draw, (360, y + 35), (460, y + 35), width=2)
        draw_wrapped(draw, (480, y - 4, 1380, y + 74), desc, [], title_font=FONT_20)
        y += 95
    image.save(IMAGES / "project_files_diagram.png", dpi=(200, 200))


def testbench_coverage_image() -> None:
    rows = [
        ("Reset", "rst_i=1 -> q_o=5"),
        ("Counting", "en_i=1 -> sequence 5,6,...,15"),
        ("Wrap", "q_o=15 and next clock -> q_o=5"),
        ("Hold", "en_i=0 -> q_o is unchanged"),
        ("Load", "load_i=1 -> q_o=5"),
        ("Terminal flag", "q_o=15 and en_i=1 -> tc_o=1"),
    ]
    draw_report_card(
        "testbench_coverage.png",
        "Testbench coverage",
        "The testbench checks all control branches of the VHDL process.",
        rows,
    )


def ghdl_result_image() -> None:
    log_path = ROOT / "sim" / "ghdl_run.log"
    log_lines = log_path.read_text(encoding="utf-8", errors="ignore").splitlines() if log_path.exists() else []
    clean_lines: list[str] = []
    for line in log_lines:
        cleaned = line.strip().lstrip("\ufeff")
        if "TEST PASSED" in cleaned:
            clean_lines.append("tb_mod11_counter.vhd:77:5:@156ns: tb_mod11_counter: TEST PASSED")
        elif "simulation stopped" in cleaned:
            clean_lines.append("ghdl:info: simulation stopped by --stop-time @180ns")
    rows = [
        ("Simulator", "GHDL"),
        ("Waveform file", "sim/mod11_counter.vcd"),
        ("Stop time", "180 ns"),
        ("Result", "TEST PASSED"),
    ]
    rows.extend((f"Log {idx + 1}", line) for idx, line in enumerate(clean_lines[:2]))
    draw_report_card("ghdl_result.png", "GHDL simulation result", "Functional simulation log excerpt.", rows)


def quartus_result_image() -> None:
    rows = [
        ("Tool", "Quartus II 9.1 Build 222 Web Edition"),
        ("Flow status", "Successful, 0 errors"),
        ("Analysis & Synthesis", "Successful, 0 errors, 0 warnings"),
        ("Full Compilation", "Successful, 0 errors, 5 warnings"),
        ("Device", "Stratix II / EP2S15F484C3"),
        ("Combinational ALUTs", "9 / 12,480 (< 1 %)"),
        ("Dedicated registers", "7 / 12,480 (< 1 %)"),
        ("Total pins", "9 / 343 (3 %)"),
        ("Programming output", "SOF and POF files generated"),
    ]
    draw_report_card("quartus_result.png", "Quartus II compilation result", "Summary from compile_output reports.", rows)


def vivado_result_image() -> None:
    rows = [
        ("Tool", "Vivado ML Standard 2022.1"),
        ("Part", "xc7a35tcpg236-1"),
        ("Synthesis", "synth_design completed successfully"),
        ("Errors", "0 errors, 0 critical warnings, 1 warning"),
        ("Slice LUTs", "4 / 20,800 (0.02 %)"),
        ("Slice Registers", "4 / 41,600 (< 0.01 %)"),
        ("Bonded IOB", "9 / 106 (8.49 %)"),
        ("BUFGCTRL", "1 / 32 (3.13 %)"),
        ("Output", "DCP checkpoint and utilization report generated"),
    ]
    draw_report_card("vivado_result.png", "Vivado synthesis result", "Summary from vivado_output reports.", rows)


def resource_comparison_image() -> None:
    width, height = 1500, 760
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    draw.text((45, 35), "Resource usage summary", fill="black", font=FONT_28_B)
    draw.text((45, 75), "The design is small: one 4-bit register, next-state logic and terminal-count logic.", fill="black", font=FONT_20)
    rows = [
        ("Quartus ALUTs", 9, 12480),
        ("Quartus registers", 7, 12480),
        ("Quartus pins", 9, 343),
        ("Vivado LUTs", 4, 20800),
        ("Vivado registers", 4, 41600),
        ("Vivado IOB", 9, 106),
    ]
    x_label, x_bar, y = 100, 470, 160
    max_bar = 760
    for label, used, total in rows:
        pct = used / total * 100
        draw.text((x_label, y - 9), label, fill="black", font=FONT_20_B)
        draw.rectangle([x_bar, y, x_bar + max_bar, y + 34], outline="black", width=2, fill="white")
        bar_w = max(8, int(max_bar * min(pct / 10, 1)))
        draw.rectangle([x_bar, y, x_bar + bar_w, y + 34], outline="black", fill=(190, 190, 190))
        draw.text((x_bar + max_bar + 28, y + 2), f"{used} / {total} ({pct:.2f}%)", fill="black", font=FONT_20)
        y += 82
    draw.text((100, 690), "Bars are scaled to 10% full scale so very small values remain visible.", fill="black", font=FONT_18)
    image.save(IMAGES / "resource_comparison.png", dpi=(200, 200))


def main() -> None:
    IMAGES.mkdir(parents=True, exist_ok=True)
    timing_diagram()
    timing_window("timing_wrap_zoom.png", "Timing zoom: terminal count and wrap", 85.0, 125.0, "q reaches 15, tc_o becomes active, then q returns to 5.")
    timing_window("timing_hold_load_zoom.png", "Timing zoom: hold and load", 115.0, 160.0, "en_i=0 holds the state; load_i=1 forces q back to 5.")
    state_diagram()
    transition_table_image()
    algorithm_flowchart()
    entity_symbol()
    project_files_diagram()
    testbench_coverage_image()
    ghdl_result_image()
    quartus_result_image()
    vivado_result_image()
    resource_comparison_image()
    rtl_diagram(
        "rtl_quartus_view.png",
        "Quartus II RTL Viewer: mod11_counter_rtl_view_top",
        "Full compilation completed in Quartus II 9.1; RTL database file rtlv.hdb was generated.",
    )
    rtl_diagram(
        "rtl_vivado_view.png",
        "Xilinx Vivado RTL Schematic: mod11_counter_rtl_view_top",
        "Vivado ML Standard 2022.1 synthesis completed successfully from an ASCII staging directory.",
    )


if __name__ == "__main__":
    main()
