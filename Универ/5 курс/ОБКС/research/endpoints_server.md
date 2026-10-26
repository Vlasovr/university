# Оконечное оборудование: исследование рынка РБ (дата проверки цен — 26.10.2026)

Проект: ЛВС научно-исследовательской медицинской организации, два здания, 12 АРМ, 2 цветных сетевых принтера A4 (по одному на лабораторию), файловый сервер на 12 пользователей (обмен файлами + личные папки), доступ из второго здания через VPN. Сеть бюджетная.

## 0. Метод и ограничения

- Основной источник цен — агрегатор catalog.onliner.by (минимальная цена среди продавцов, число предложений, карточки отдельных магазинов на `*.shop.onliner.by`) и shop.by. Цены ПК, мониторов, наборов клавиатура+мышь и ОС сняты 26.10.2026 через открытый API каталога Onliner (`catalog.onliner.by/sdapi/catalog.api/…`, `shop.api.onliner.by/products/<key>/positions`); у каждого предложения поле `date_update` = 2026-10-26.
- Из среды исследования не открывались 21vek.by, 5element.by, electronic.by, itelite.by, kompo.by, synology.by, nas.by, servers.by (DNS/гео-блокировка). Страницы категорий shop.by отдают список товаров только скриптом, поэтому с shop.by взяты карточки конкретных товаров, а не выборки.
- Все цены в BYN с НДС, «от X» — нижняя граница у продавцов агрегатора. Там, где параметр в карточке не указан и не подтверждён производителем, стоит пометка **не подтверждено**.
- Символ валюты на сайтах («ƃ», «Ђ») = белорусский рубль.

---

## 1. Рабочие места (ПК, монитор, клавиатура+мышь)

### 1.1. Системные блоки, бюджет 800–1400 BYN

Все кандидаты — готовые сборки белорусских производителей (TGPC — ООО «ТехноГрупп», Iven — ООО «Компьютеры Айвен», Минск), продаются через 17–22 магазина на Onliner. Брендовые мини-ПК (Lenovo, HP, Dell) в бюджет не входят — см. 1.2.

| № | Модель | CPU | ОЗУ | SSD | Чипсет / БП | LAN | Wi-Fi | ОС | Гарантия | Цена, BYN (26.10.2026) | Магазины | Наличие |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | **TGPC Office 93037 I-X** | Intel Core i3-12100 (4C/8T, 3,3–4,3 ГГц, TDP 60 Вт) | 16 ГБ DDR4-3200 (2 слота, до 64 ГБ) | 240 ГБ | Intel H610 / 500 Вт, корпус M4X | **1 × 1 Gbit** (карточка Onliner) | нет | без ОС | 24 мес (у TUF — 36) | от **1 317,99** до 1 586,22; 22 предл. | [Onliner](https://catalog.onliner.by/desktoppc/tgpc/tgpc93037), [TUF 1 317,99](https://19523.shop.onliner.by), [ВИКО 1 318,00](https://5363.shop.onliner.by), [Sli 1 335,82](https://1748.shop.onliner.by) | Sli — «на складе», ВИКО/LevelUP — 1 шт., отгрузка 1–2 дня |
| 2 | **Iven Office 181168** | Intel Core i3-12100 | 16 ГБ DDR4 | 240 ГБ 2,5" | чипсет в карточке не указан / 500 Вт | **не подтверждено** (в карточке Onliner нет раздела «Интерфейсы»; платформа LGA1700/H610 имеет 1GbE на всех платах — уточнять у продавца) | нет данных | в карточке не указана (у продавцов — без ОС) | 12–36 мес (Sli, ВИКО — 24; TUF — 36) | от **1 295,99** до 1 686,10; 22 предл. | [Onliner](https://catalog.onliner.by/desktoppc/iven/iven181168), [TUF 1 295,99](https://19523.shop.onliner.by), [ВИКО 1 296,00](https://5363.shop.onliner.by), [Sli 1 341,73](https://1748.shop.onliner.by) | Sli — «на складе», сегодня; остальные 1 шт./под заказ |
| 2а | Iven Office 181177 (тот же, но i3-13100) | Intel Core i3-13100 | 16 ГБ DDR4 | 240 ГБ | — / 500 Вт | не подтверждено | — | — | 12–36 | от 1 297,80; 21 предл. | [Onliner](https://catalog.onliner.by/desktoppc/iven/iven181177) | — |
| 3 | **TGPC Business 87231 A-X** | AMD Ryzen 3 3200G (4C/4T, 3,6–4,0 ГГц, 65 Вт, Vega 8) | 16 ГБ DDR4 | 480 ГБ | AMD A520M / 500 Вт | поле «LAN» есть, скорость не заполнена → **не подтверждено** (A520 — платформа с 1GbE) | нет | без ОС | 24 (TUF — 36) | от **1 146,30** до 1 412,17; 19 предл. | [Onliner](https://catalog.onliner.by/desktoppc/tgpc/tgpc87231), [LevelUP 1 146,30](https://8765.shop.onliner.by), [VIPCOMP 1 156,00](https://3886.shop.onliner.by), [Sli 1 156,68](https://1748.shop.onliner.by) | VIPCOMP и Sli — «на складе» |
| 4 | **Iven Office 179552** | AMD Ryzen 3 3200G | 16 ГБ DDR4 | 240 ГБ 2,5" | — / 500 Вт | не подтверждено | — | — | 12–36 | от **1 066,30** до 1 367,60; 21 предл. | [Onliner](https://catalog.onliner.by/desktoppc/iven/iven179552), [LevelUP 1 066,30](https://8765.shop.onliner.by), [ВИКО 1 077,00](https://5363.shop.onliner.by), [VIPCOMP 1 098,45](https://3886.shop.onliner.by) | VIPCOMP — «на складе» |
| 5 | TGPC Office 93034 I-X | Intel Pentium Gold G7400 (2C/4T, 3,7 ГГц) | 16 ГБ DDR4-3200 | 240 ГБ | Intel H610 / 500 Вт | **1 × 1 Gbit** | нет | без ОС | 24 | от 1 269,43; 17 предл. | [Onliner](https://catalog.onliner.by/desktoppc/tgpc/tgpc93034) | — |
| 6 | TGPC Office 93039 I-X (вариант №1 с SSD 480 ГБ) | i3-12100 | 16 ГБ | 480 ГБ | H610 / 500 Вт | 1 × 1 Gbit (по линейке) | нет | без ОС | 24 | от 1 455,00; 21 предл. — **выше бюджета** | [Onliner](https://catalog.onliner.by/desktoppc/tgpc/tgpc93039) | — |

Примечания.
- Характеристики №1, №3, №5 — из раздела «Характеристики» карточек Onliner (чипсет, LAN 1 × 1 Gbit, «без ОС», корпус M4X 500W, 165×270×345 мм, 8,45 кг). У Iven карточки короче, и сетевой адаптер в них не описан: это надо подтверждать у продавца или по паспорту изделия.
- Ни у одной сборки нет Wi-Fi — для стационарных АРМ на витой паре это и не требуется.
- Конфигурации с 8 ГБ ОЗУ (BVK, SkySystems, Jet на Athlon/Celeron, 715–900 BYN) из рассмотрения исключены: для офисной работы с браузером и лабораторными данными в 2026 г. 16 ГБ — минимум, чтобы не менять ПК через 2–3 года.
- 21vek.by (Z-Tech I3-121-8-480-610-N-0001n, Jet Office 3i12100D16SD96VGALW5) из среды исследования не открылся; цены **не подтверждены**.

### 1.2. Брендовые мини-ПК (для справки — за пределами бюджета)

| Модель | Конфигурация | Цена, BYN | Источник |
|---|---|---|---|
| Lenovo ThinkCentre neo 50q Gen 5 13B90027GP | i3-1315U, 8 ГБ DDR5, SSD 256 ГБ, Wi-Fi 6E, без ОС | от 2 294,46 (10 предл.); shop.by — от 2 198,80 | [Onliner](https://catalog.onliner.by/minipc/lenovo/13b90027gp), [shop.by](https://shop.by/nettopy/lenovo_thinkcentre_neo_50q_gen_5_13b9008dsa/) (блок «Другие модели Lenovo») |
| Lenovo ThinkCentre M70q Gen 5 12TDS1KY00 | i5-13400T, 16 ГБ DDR5, 512 ГБ, без ОС | от 3 271,00 (11 предл.) | [Onliner](https://catalog.onliner.by/minipc/lenovo/12tds1ky00) |
| HP Pro Mini 400 G9 935X8EA | i3-13100T, 8 ГБ, 256 ГБ, без ОС | от 2 321,40 (16 предл.) | [Onliner](https://catalog.onliner.by/minipc/hp/935x8ea) |
| Dell OptiPlex Micro / ASUS ExpertCenter PN | карточки в каталоге есть, **предложений с ценой нет** | — | поиск Onliner «Dell OptiPlex Micro», «ASUS ExpertCenter PN» |
| Beelink EQi12 (i3-1220P, 16/500) | без ОС | от 2 605,46 (2 предл.) | [Onliner](https://catalog.onliner.by/minipc/beelink/beqi121220p165g) |
| Chuwi LarkBox X i3-1220P/8GB/256GB/**Win11 Pro** | Wi-Fi 6, BT 5.2; Ethernet в карточке не указан → не подтверждено | от **1 125,00** (21 предл.) | [Onliner](https://catalog.onliner.by/minipc/chuwi/larkboxxi38256w1) |

Вывод по 1.2: брендовые мини-ПК начинаются от ~2 200 BYN за 8 ГБ ОЗУ — в 1,7–2 раза дороже местной сборки с 16 ГБ. Единственный мини-ПК в бюджете (Chuwi, 8 ГБ, лицензия Windows в комплекте) проигрывает по ОЗУ и ремонтопригодности.

### 1.3. Операционная система

| Позиция | Цена, BYN | Источник |
|---|---|---|
| Microsoft Windows 11 Pro 64-bit OEI FQC-10547 (рус., бессрочная, корпоративная) | 17 предложений: от 100,46 до 687,87; **медиана 150,00** (AMD 100,46; ГИГАМАРКЕТ 100,46; Lix 112,83; СтоксБай 116,40) | [Onliner](https://catalog.onliner.by/antivirus/microsoft/fqc10547) |

Разброс цен большой; в смету закладывать медиану (150 BYN) и брать у поставщика с документами. Альтернатива для бюджетной сети — Linux (бесплатно), если прикладное ПО лабораторий это допускает.

### 1.4. Монитор 23,8" IPS (офисный, 1920×1080)

| Модель | Параметры | Цена, BYN (26.10.2026) | Магазины | Наличие / гарантия |
|---|---|---|---|---|
| **LG 24U411A-B** | 23,8" IPS, 1920×1080, 120 Гц, 250 нит, HDMI + VGA | от **277,70** до 408,70; 75 предл. | [Onliner](https://catalog.onliner.by/display/lg/24u411ab), [Golden 277,70](https://21049.shop.onliner.by), [Mobil 277,74](https://21481.shop.onliner.by), [Newton 278,26](https://6498.shop.onliner.by) | Mobil, TERON, Newton — «на складе»; 24 мес |
| **AOC 24B15H3** | 23,8" IPS, 1920×1080, 120 Гц, 250 нит, HDMI + VGA | от **249,00** до 474,50; 62 предл. | [Onliner](https://catalog.onliner.by/display/aoc/24b15h3), [Newton 249,00](https://6498.shop.onliner.by), [ITMarket 249,00](https://416.shop.onliner.by), [Электросила 249,00](https://12735.shop.onliner.by) | Newton, ITMarket, Mobil — «на складе»; 24–36 мес |
| HAFF T238 (бел. бренд) | 23,8" IPS, 120 Гц, динамики, HDMI + DP + VGA | от 232,80; 36 предл. | [Onliner](https://catalog.onliner.by/display/haff/t238t238) | рейтинг 2,6/5 по 7 отзывам |
| Philips 24E1N1100A/01 | 23,8" IPS, 120 Гц, 8 бит, динамики, HDMI + VGA | от 291,50; 57 предл. | [Onliner](https://catalog.onliner.by/display/philips/24e1n1100a01) | — |

### 1.5. Клавиатура + мышь

| Набор | Тип | Цена, BYN | Магазины | Наличие / гарантия |
|---|---|---|---|---|
| **Logitech MK120 920-002561** | проводной, USB, рус. раскладка | от **68,49** до 321,43; 52 предл. | [Onliner](https://catalog.onliner.by/peripheralkits/logitech/desktopmk120), [Быстрый выбор 68,49](https://21384.shop.onliner.by), [SilverShop 68,59](https://21283.shop.onliner.by) | «на складе»; 12–36 мес |
| Logitech Wireless Combo MK220 920-003169 | беспроводной 2,4 ГГц, рус. раскладка | от 65,87; 48 предл. | [Onliner](https://catalog.onliner.by/peripheralkits/logitech/combo_mk220), [575 67,58](https://19096.shop.onliner.by) | «на складе» |

Для лабораторий предпочтительнее проводной MK120: нет батареек и радиоканала, цена та же.

### 1.6. Стоимость одного АРМ и всех 12

| Вариант | Системный блок | Монитор | Клавиатура+мышь | ОС | Итого за АРМ | × 12 |
|---|---|---|---|---|---|---|
| **А (рекомендуемый)**: TGPC Office 93037 I-X | 1 317,99 | LG 24U411A-B 277,70 | MK120 68,49 | без ОС (Linux) | **1 664,18** | **19 970,16** |
| А + Windows 11 Pro OEI (медиана) | 1 317,99 | 277,70 | 68,49 | 150,00 | 1 814,18 | 21 770,16 |
| Б (минимум): TGPC Business 87231 A-X | 1 146,30 | AOC 24B15H3 249,00 | MK120 68,49 | без ОС | 1 463,79 | 17 565,48 |
| В: Iven Office 181168 | 1 295,99 | LG 24U411A-B 277,70 | MK120 68,49 | без ОС | 1 642,18 | 19 706,16 |

---

## 2. Цветные принтеры A4 с Ethernet

### 2.1. Отсев (не подходят или не продаются в РБ на 26.10.2026)

| Модель | Причина |
|---|---|
| Brother HL-L3220CW / CWE | нет Ethernet и автодуплекса по документации Brother ([support.brother.com](https://support.brother.com/g/s/id/htmldoc/printer/cv_hll3220cw/uke/html/GUID-2E7A7E59-10C1-4182-864D-7D1C021BC548_1.html)); в РБ не продаётся |
| Brother HL-L3240CDW | Ethernet есть, но предложений в РБ нет ([Onliner](https://catalog.onliner.by/printers/brother), [shop.by](https://shop.by/printery/brother/)); тонеры TN-248 в РБ не найдены |
| Canon i-SENSYS LBP631Cw / LBP633Cdw | предложений нет ([Onliner 631Cw](https://catalog.onliner.by/printers/canon/5159c004/prices), [shop.by 633Cdw](https://shop.by/printery/canon_lbp633cdw_5159c001/)); 631Cw у Canon снят с производства |
| Kyocera ECOSYS P5026cdn | «нет в продаже» ([Onliner](https://catalog.onliner.by/printers/kyoceramita/p5026cdn)) |
| Epson L8050 | нет Ethernet, ручной дуплекс ([epson.eu](https://www.epson.eu/en_EU/products/printers/inkjet/consumer/ecotank-l8050-a4-wi-fi-ink-tank-photo-printer/p/34296)) |
| Canon PIXMA G3470 | нет Ethernet ([Onliner](https://catalog.onliner.by/printers/canon/pixmag3470black)); G5040 — 1 продавец без статуса; GM4040 — монохромный |
| Epson L6190, HP OfficeJet Pro 9010/9110/9120, Brother DCP-T820DW / MFC-T920DW | 0 предложений в РБ |

### 2.2. Сравнение моделей, реально продающихся в РБ

Стоимость отпечатка: чб = цена чёрного картриджа / ресурс; цв = Σ (цена картриджа_i / ресурс_i) по K, C, M, Y. Цены картриджей — минимальные на Onliner/shop.by, 26.10.2026.

| Модель | Технология | Скорость цв/чб, стр/мин | Ethernet | Wi-Fi | Автодуплекс | Ресурс сменных картриджей (ISO) | Цена картриджей, BYN | **Отпечаток чб / цв, BYN** | Нагрузка, стр/мес (рек. / макс.) | Цена принтера, BYN | Магазины | Наличие |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **Pantum CP1100DN** | лазер | 18 / 18 | да | нет | да | CTL-1100XK 3000; XC/XM/XY 2300 (старт: K 1000, CMY 700) | XK 252,89; XC 253,60; XM 234,00; XY 252,89 | **0,084 / 0,406** | до 1 500 / 30 000 | от **1 370,00** (shop.by, 17 предл.); 1 627,82 amd.by | [shop.by](https://shop.by/printery/pantum_cp1100dn/), [amd.by](https://www.amd.by/printery-i-mfu/pantum-cp1100dn/) | x-core.by 1 шт.; amd.by под заказ |
| **Pantum CP1100DW** | лазер | 18 / 18 | да | да | да | те же | те же | 0,084 / 0,406 | до 1 500 / 30 000 | от 1 444,00 (shop.by); от 1 562,82 (Onliner, 17 предл.) | [Onliner](https://catalog.onliner.by/printers/pantum/cp1100dw), [shop.by](https://shop.by/printery/pantum_cp1100dw/) | в наличии (dreli.by, 575.by по 1 шт.) |
| Pantum CP2100DW | лазер | 20 / 20 | да | да | да | CTL-R2100HK 3000; HC/HM/HY 2500 | HK 264,52; HC 265,45; HM 263,00; HY 263,00 | 0,088 / 0,405 | до 1 500 / 30 000 | от 1 366,20 (lix.by); 1 450,83 Onliner (1 предл.) | [shop.by](https://shop.by/printery/pantum_cp2100dw/), [Onliner](https://catalog.onliner.by/printers/pantum/cp2100dw) | 3 продавца, статус не указан |
| HP Color Laser 150nw | лазер | **4** / 18 | да | да | **нет** (ручной) | 117A: W2070A 1000; W2071A/72A/73A 700 | 230,12; 256,61; 261,81; 255,12 | 0,230 / **1,335** | 100–500 / 20 000 | от 1 033,00 (shop.by, 31 предл.); от 1 123,69 (Onliner, 44) | [Onliner](https://catalog.onliner.by/printers/hp/4zb95a), [shop.by](https://shop.by/printery/hp_color_laser_150nw_4zb95a/) | в наличии (stox.by >10 шт.) |
| HP Color LaserJet Pro 4203dn | лазер | 33 / 33 | да (Gigabit) | нет | да | 230A: W2300A 2000; W2301A/02A/03A 1800 (230X: K 7500, цв. 5500) | 418,00; 483,00 ×3; W2300X 955,52 | 0,209 (230A) или 0,127 (230X) / 1,014 (230A) | 750–4 000 / 50 000 | от 1 311,00 (shop.by, 36); от 1 470,49 (Onliner, 52) | [Onliner](https://catalog.onliner.by/printers/hp/4ra89a), [shop.by](https://shop.by/printery/hp_color_laserjet_pro_4203dn_4ra89a/) | в наличии (1teh.by, fastshop по 1 шт.) |
| HP Color LaserJet Pro 4203dw | лазер | 33 / 33 | да (Gigabit) | да | да | как 4203dn | как 4203dn | как 4203dn | 750–4 000 / 50 000 | от 1 506,00 (shop.by, 26); от 1 621,20 (Onliner, 36) | [Onliner](https://catalog.onliner.by/printers/hp/5hh48a), [shop.by](https://shop.by/printery/hp_color_laserjet_pro_4203dw_5hh48a/) | в наличии |
| HP Color LaserJet Pro M255dw | лазер | 21 / 21 | да | да | да | 207A: K 1350, CMY 1250; 207X: K 3150, CMY 2450 | W2210A 350,75; W2211X 587,89; W2213X 482,78; W2212X — не найден | 0,260 / не подтверждено | 150–2 500 / 40 000 | 2 959,58 (1 продавец) | [Onliner](https://catalog.onliner.by/printers/hp/7kw64a) | статус не указан |
| Xerox C230 | лазер | 22–24 / 22–24 | да | да | да | 006R04395 K 3000; 96/97/98 CMY 2500 | K 233,59; C 571,92; M 599,30; Y 545,85 | 0,078 / 0,765 | до 1 500 / 30 000 | от 1 400,00 (teono, tiviline); 1 524,54 Onliner (2 предл.) | [Onliner](https://catalog.onliner.by/printers/xerox/c230vdni), [shop.by](https://shop.by/printery/xerox_c230/) | teono — доставка 29.10 |
| Kyocera ECOSYS PA2100cwx | лазер | 21 / 21 | да (Gigabit) | да | да | TK-5430 K/C/M/Y по 1250 (TK-5440 в РБ не найдены) | K 215,00; C/M/Y по 201,00 | 0,172 / 0,654 | не подтверждено / 30 000 | от 1 681,00 (shop.by); от 1 777,40 (Onliner, 22); 1 712,00 amd.by | [Onliner](https://catalog.onliner.by/printers/kyoceramita/pa2100cwx), [amd.by](https://www.amd.by/printery-i-mfu/kyocera-mita-pa2100cwx-110c093nl0/) | в наличии (amd.by, corven.by, 100pro.by) |
| Epson EcoTank L6290 (МФУ) | струйная СНПЧ | 8,5 / 15,5 (ISO) | да | да | да | Epson 101: BK 7500; C/M/Y по 6000 | BK 56,00; C 39,50; M 38,00; Y 38,00 | **0,0075 / 0,027** | не подтверждено | 2 141,63 (shop.by, 3); 2 257,93 Onliner (1) | [Onliner](https://catalog.onliner.by/printers/epson/epsc11cj60406), [shop.by](https://shop.by/mnogofunktsionalnye_ustroystva/epson_ecotank_l6290/) | по 1 шт.; у Epson снят с производства |
| Epson EcoTank L6490 (МФУ, замена) | струйная СНПЧ | 9,5 / 17 (ISO) | да | да | да | Epson 112; ресурс флаконов **не подтверждён** | BK от 14,00 (возможно аналог); C/M/Y от 84,00 | не подтверждено | не подтверждено | от **1 291,00** (shop.by, 32); от 1 387,05 (Onliner, 37) | [Onliner](https://catalog.onliner.by/printers/epson/c11cj88405), [shop.by](https://shop.by/mnogofunktsionalnye_ustroystva/epson_ecotank_l6490/) | в наличии (Mobil, Newton, MyMarket) |
| HP OfficeJet Pro 9023 (МФУ, замена 9010/9120) | струйная картриджная | 20 / 24 | да | да | да | 963XL: BK 3000, CMY 1600 | BK 249,87; C 187,83; M 183,71; Y 175,51 | 0,083 / 0,425 | 2 000 / 30 000 | от 1 141,00 (shop.by, 25); от 1 239,38 (Onliner, 31) | [Onliner](https://catalog.onliner.by/printers/hp/1mr70b), [shop.by](https://shop.by/mnogofunktsionalnye_ustroystva/hp_officejet_pro_9023/) | в наличии |

Расчёт для Pantum CP1100DN/DW: 252,89/3000 + 253,60/2300 + 234,00/2300 + 252,89/2300 = 0,084 + 0,110 + 0,102 + 0,110 = **0,406 BYN/стр (цв)**; чб = 0,084. Для HP 150nw: 230,12/1000 + 256,61/700 + 261,81/700 + 255,12/700 = 1,335. Для Epson L6290: 56/7500 + 39,5/6000 + 38/6000 + 38/6000 = 0,027.

Противоречия в источниках: amd.by и shop.by в описании CP1100DN пишут «только USB», тогда как производитель указывает Ethernet и автодуплекс ([pantum.ru CP1100DN](https://www.pantum.ru/products/laser-devices/cvetnoj-printer-cp1100dn/)) — принято по производителю (индекс «N» = network). Onliner приписывает HP 150nw автодуплекс, HP — только ручной.

Расчёт на год при ~300 стр/мес на принтер (200 чб + 100 цв): Pantum CP1100DN — 12 × (200×0,084 + 100×0,406) = 689 BYN; HP 150nw — 12 × (200×0,230 + 100×1,335) = 2 154 BYN; Epson L6490 (по данным L6290) — 12 × (200×0,0075 + 100×0,027) = 50 BYN.

---

## 3. Файловый сервер

### 3.1. Класс (а): 2-дисковые NAS

| Модель | CPU | RAM | Отсеки | LAN | ФС / RAID / снапшоты | Пользователи, квоты, домен | VPN-сервер | Цена без дисков, BYN (Onliner / shop.by) | Предл. | Наличие |
|---|---|---|---|---|---|---|---|---|---|---|
| **Synology DS223j** | Realtek RTD1619B, 4×1,7 ГГц | 1 ГБ DDR4 | 2 × 3,5"/2,5" | 1 × 1GbE | ext4/**Btrfs**; RAID 0/1/SHR; Snapshot Replication (Btrfs) | DSM: пользователи/группы, квоты, клиент AD/LDAP¹ | Synology VPN Server (OpenVPN/L2TP/PPTP)¹ | **778,00 / 757,00** | 38 / 25 | Onliner-склад 862,33; stelar.by 760,70; newton 765,01; stox.by 778 |
| Synology DS223 | RTD1619B | 2 ГБ | 2 | 1 × 1GbE | ext4/Btrfs, SHR | ¹ | ¹ | 1 229,99 / 1 049,99 | 30 / 25 | hypersale 1 049,99 (1 шт.) |
| Synology DS124 | RTD1619B | 1 ГБ | **1** (RAID1 невозможен) | 1 × 1GbE | ext4/Btrfs | ¹ | ¹ | 662,27 / 630,00 | 30 / 28 | — |
| Synology DS224+ | Intel Celeron J4125 | 2 ГБ (до 6) | 2 | **2 × 1GbE** | ext4/Btrfs | ¹ | ¹ | 1 899,99 / 1 869,99 | 1 / 2 | уходит с рынка; замена DS225+ от 1 419,99 (shop.by) |
| **QNAP TS-233** | ARM Cortex-A55, 4×2,0 ГГц | 2 ГБ | 2 | 1 × 1GbE | **ext4** (Btrfs нет); снапшоты томов/LUN | SMB/NFS; домен, LDAP-сервер; квоты; до 4 096 польз. | QVPN: OpenVPN, L2TP/IPsec, **WireGuard**, QBelt | 1 049,99 / 909,99 | 32 / 26 | hypersale 909,99; mega trade 949,49 |
| QNAP TS-216G | Cortex-A55 | 4 ГБ | 2 | 1 × 2,5GbE + 1 × 1GbE | ext4, снапшоты | QTS | QVPN | 2 299,99 / 2 249,99 | 1 / 2 | дорого для класса |
| QNAP TS-264-8G | Intel N5095 | 8 ГБ | 2 + 2 × M.2 | 2 × 2,5GbE | ext4 (QTS) или ZFS (QuTS hero) | QTS/QuTS | QVPN | 2 801,01 / 2 475,70 | 34 / 24 | много предложений |
| TerraMaster F2-212 / F2-223 | — | — | — | — | — | — | — | **нет в продаже** | — | — |
| TerraMaster F2-425 (замена) | Intel N5095 | 4 ГБ | 2 | 1 × 2,5GbE | ext4/Btrfs, TRAID | TOS — не подтверждено | не подтверждено | 959,07 / **830,00** | 2 / 8 | texnomix.by 830, spx.by 830 |
| Asustor Drivestor 2 AS1102T / AS1102TL | — | — | — | — | — | — | — | **нет в продаже** | — | — |
| Asustor Drivestor 2 Pro AS3302T | Realtek RTD1296 | 2 ГБ | 2 | 1 × 2,5GbE | ext4; снапшоты только iSCSI | ADM — не подтверждено | не подтверждено | 1 357,91 / 1 152,40 | 20 / 16 | stelar.by 1 152,40 |
| UGREEN NASync DXP2800 | Intel N100 | 8 ГБ | 2 + M.2 | 1 × 2,5GbE | — | — | — | карточка есть, **0 предложений** | 0 | — |

¹ Функции DSM — по документации Synology ([DS223j specs](https://www.synology.com/en-global/products/DS223j#specs), [VPN Server](https://www.synology.com/en-global/dsm/packages/VPNCenter)); сайт отдавал 403 автоматическому запросу, CPU/RAM/LAN/Btrfs подтверждены карточками Onliner/shop.by. Источники цен: [Onliner NAS](https://catalog.onliner.by/nas), [shop.by сетевые накопители](https://shop.by/setevye_nakopiteli/); характеристики QNAP — [TS-233](https://www.qnap.com/en/product/ts-233/specs/hardware), [TS-264](https://www.qnap.com/en/product/ts-264/specs/hardware); Asustor — [AS3302T](https://www.asustor.com/en/product?p_id=72); TerraMaster — [F2-425](https://www.terra-master.com/products/f2-425).

### 3.2. Класс (б): готовые серверы начального уровня

| Модель | Поиск в РБ | Результат | Ориентир вне РБ (не цена РБ) |
|---|---|---|---|
| HPE ProLiant ML30 Gen11 | Onliner (поиск), shop.by/servery/ (фильтр HP — пусто), 21vek/itelite/servers.by/dsol/evro-it/ivtcom/newton/elfa — недоступны | **не подтверждено** | РФ: P65093-001 (E-2414, 16 ГБ) 86 858 ₽ (hpeserver.ru) |
| HPE MicroServer Gen10 Plus v2 | те же | **не подтверждено**; только б/у Gen10 на forpro.by — 3 082 BYN (сниппет, страница не открылась) | РФ: от 105 400 ₽; EU: от 1 983 € |
| Dell PowerEdge T150 / T160 / T360 | Onliner, shop.by (фильтр Dell — пусто) | **не подтверждено** | РФ: T160 от 251 600 ₽; EU: T150 921 € |
| Lenovo ThinkSystem ST50 V2 / ST250 V2 | Onliner, shop.by | **не подтверждено**; у РФ-продавцов «снято с производства» | EU: ST50 V2 1 810 € (Bechtle) |
| Аналог, реально в продаже в РБ: iRU Rock W9102E (Xeon E-2314, 8 ГБ, 500 ГБ) | [shop.by/servery/](https://shop.by/servery/) → gigamarket.by | **4 741,65 BYN** (1 предл.); 16 ГБ — 5 190,06 | — |

Брендовые башенные серверы в белорусской рознице есть только под заказ у интеграторов (цена по запросу). Минимальная подтверждённая серверная платформа на Xeon E — ~4,7 тыс. BYN без ОС, дисков и ИБП, т.е. в 5–6 раз дороже NAS.

### 3.3. Класс (в): файловый сервер на базе ПК (TrueNAS SCALE / Ubuntu Server + Samba — бесплатно)

| Системный блок | CPU / ОЗУ / SSD | БП / корпус | Цена, BYN | Предл. | Свободные SATA и отсеки 3,5" |
|---|---|---|---|---|---|
| TGPC Business 87264 A-X | Ryzen 3 3200G / 8 ГБ / 480 ГБ | 500 Вт / midi-tower, AMD A520M | 955,00 | 18 | в карточке **не указано**; A520 даёт до 4 SATA; уточнять у продавца — [Onliner](https://catalog.onliner.by/desktoppc/tgpc/tgpc87264) |
| Iven Office 179550 | Ryzen 3 3200G / 8 ГБ / 240 ГБ | 500 Вт / стандартный | 842,00 | 18 | не указано — [Onliner](https://catalog.onliner.by/desktoppc/iven/iven179550) |
| TGPC Office 93037 I-X (тот же, что для АРМ) | i3-12100 / 16 ГБ / 240 ГБ | 500 Вт / M4X | 1 317,99 | 22 | H610: 4 × SATA 6 Gb/s по спецификации чипсета; отсеки корпуса не указаны |
| Мини-ПК на N100 (IRBY M2-N100-8-256) | N100 / 8 / 256 | мини | 1 294,79 | 2 | **нет отсеков 3,5"** — не подходит |

Итог класса (в): ПК 842–1 318 + 2 × HDD 1 430–1 700 ≈ **2 300–3 000 BYN** без ИБП. Минусы: нужен администратор для ZFS/Samba, нет гарантии на решение целиком, потребление 40–60 Вт против 10–20 Вт у NAS, число отсеков под 3,5" в карточках не подтверждено.

### 3.4. Диски 3,5" для RAID1

| Диск | Класс | Onliner, BYN (предл.) | shop.by, BYN (магазины) | Примечание |
|---|---|---|---|---|
| **WD Red Plus 4 TB WD40EFPX** | NAS, CMR, 24/7, 3 года | 849,99–1 449,32 (43) — [Onliner](https://catalog.onliner.by/hdd/westerndigital/wd40efpx) | от 849,00 (32): 9watt.by 849, newton 955,28 — [shop.by](https://shop.by/zhestkie_diski/western_digital_red_plus_4tb_wd40efpx/) | — |
| WD Red Plus 2 TB WD20EFPX | NAS, CMR | 671,78–991,90 (46) — [Onliner](https://catalog.onliner.by/hdd/westerndigital/wd20efpx) | newton 656,79; 9watt.by 668,90 — [shop.by](https://shop.by/zhestkie_diski/western_digital_red_plus_2tb_wd20efpx/) | 2 ТБ лишь на 20% дешевле 4 ТБ |
| **Seagate IronWolf 4 TB ST4000VN006** | NAS, CMR, 24/7, 3 года | 730,18–1 137,10 (54) — [Onliner](https://catalog.onliner.by/hdd/seagate/st4000vn006) | от 714,81 (38): stelar.by 725,20; 9watt.by 728,50 — [shop.by](https://shop.by/zhestkie_diski/seagate_ironwolf_4tb_st4000vn006/) | дешевле Red Plus на ~120 BYN |
| Seagate IronWolf 2 TB ST2000VN003 | NAS, CMR | 643,81–921,26 (41) — [Onliner](https://catalog.onliner.by/hdd/seagate/st2000vn003) | 9watt.by 597,10 — [shop.by](https://shop.by/zhestkie_diski/seagate_ironwolf_2tb_st2000vn003/) | — |
| Toshiba N300 4 TB HDWG440 | NAS | «снят с продажи» на Onliner; на shop.by нет | только MN10ADA400ES — 1 549–1 606 | невыгодно |
| WD Purple 4 TB WD44PURZ | видеонаблюдение, CMR, 24/7, 3 года | от 595,29 (60) — [Onliner](https://catalog.onliner.by/hdd/westerndigital/wd44purz) | от 571,00 (37) — [shop.by](https://shop.by/zhestkie_diski/western_digital_purple_surveillance_4tb_wd44purz/) | допустимая бюджетная замена |
| WD Blue 4 TB WD40EZAX | десктоп, 2 года | от 649,30 (46) — [Onliner](https://catalog.onliner.by/hdd/westerndigital/wd40ezax) | от 624,80 — [shop.by](https://shop.by/zhestkie_diski/western_digital_blue_4tb_wd40ezax/) | не для 24/7; WD20EZBX — SMR, для RAID не подходит |

### 3.5. ИБП 650–1000 ВА (line-interactive, USB для корректного выключения NAS)

| Модель | ВА / Вт | Розетки | USB | Onliner, BYN (предл.) | shop.by, BYN (магазины) |
|---|---|---|---|---|---|
| **CyberPower UT850EG** | 850 / 425 | 3 × Schuko | да | 232,85–320,76 (32) — [Onliner](https://catalog.onliner.by/ups/cyberpower/ut850eg) | newton 218,90; stelar.by 228,80 — [shop.by](https://shop.by/ibp/cyberpower_ut850eg/) |
| CyberPower BU850E | 850 / 425 | 4 × Schuko | да | 238,13–320,85 (37) — [Onliner](https://catalog.onliner.by/ups/cyberpower/bu850e) | от 214,00 (27) — [shop.by](https://shop.by/ibp/cyberpower_bu850e/) |
| **Ippon Back Power Pro II 800** | 800 / 480 | 4 × IEC C13 | да (+RS-232), AVR 162–290 В | 340,33–532,76 (33) — [Onliner](https://catalog.onliner.by/ups/ippon/bppii800) | от 313,00 (28): stelar.by, newton — [shop.by](https://shop.by/ibp/ippon_back_power_pro_ii_800/) |
| Ippon Back Basic 850 Euro | 850 / 480 | 3 × Schuko | да | 215,10–428,00 (37) — [Onliner](https://catalog.onliner.by/ups/ippon/basic850euro) | 850 S Euro от 200,78 — [shop.by](https://shop.by/ibp/ippon_back_basic_850_s_euro/) |
| **APC Back-UPS BX750MI** | 750 / 410 | 4 × IEC C13 | да | 503,76–714,66 (31) — [Onliner](https://catalog.onliner.by/ups/apc/bx750mi) | lix.by 479,19; newton 511,03 — [shop.by](https://shop.by/ibp/apc_back_ups_750va_bx750mi/) |
| APC BX750MI-GR / BX950MI-GR | 750/410; 950/520 | Schuko | да | 501,91 (31) — [Onliner](https://catalog.onliner.by/ups/apc/bx750migr); 662,21 (27) — [Onliner](https://catalog.onliner.by/ups/apc/bx950migr) | — |
| APC BX650MI | — | — | — | **в РБ не найден** (только устаревший BX650CI-RS 802,46) | — |
| Powercom Raptor RPT-800A / 800AP | 800 / 480 | IEC или Schuko (Euro) | у «A» USB противоречиво (не подтверждено); у «AP» — да | RPT-800AP 192,18 (48) — [Onliner](https://catalog.onliner.by/ups/powercom/rpt800ap) | RPT-800A от 190,00 — [shop.by](https://shop.by/ibp/powercom_raptor_rpt_800a/) |
| Powercom Spider SPD-850U | 850 / 510 | 8 × Schuko | да (USB+RS-232) | 422,00 (52) — [Onliner](https://catalog.onliner.by/ups/powercom/spiderspd850u) | 426,50 — [shop.by](https://shop.by/ibp/powercom_spd_850u/) |
| SVC V-650-F / V-800-F | — | — | — | **нет в продаже** | — |

---

## 4. Выводы

### 4.1. ПК и АРМ

Критерии: процессор не старше 12-го поколения Intel или Zen 2 с запасом на 5 лет; 16 ГБ ОЗУ; SSD; подтверждённый Gigabit Ethernet; гарантия производителя 24 мес; цена ≤ 1 400 BYN; доступность у нескольких продавцов.

Оптимальный выбор — **TGPC Office 93037 I-X** (i3-12100 / H610 / 16 ГБ DDR4-3200 / SSD 240 ГБ / 500 Вт / LAN 1 × 1 Gbit / без ОС), 1 317,99–1 335,82 BYN, 22 продавца, есть «на складе». Обоснование по совокупности: единственный из кандидатов до 1 400 BYN, у кого в карточке подтверждены и чипсет, и гигабитный LAN, и «без ОС»; i3-12100 (4C/8T, 2022 г.) заметно производительнее Ryzen 3 3200G (4C/4T, 2019 г.) при той же цене платформы; 2 слота DDR4 до 64 ГБ и свободные SATA дают запас на модернизацию; гарантия 24 мес (у TUF — 36). Iven Office 181168 на 22 BYN дешевле и по конфигурации идентичен, но сетевой адаптер в карточке не описан — его можно брать как равноценную замену после подтверждения у продавца. TGPC Business 87231 A-X (Ryzen 3 3200G, SSD 480) экономит ~170 BYN на АРМ (≈2 060 BYN на 12 шт.), но это платформа 2019 г. без подтверждённой скорости LAN; её разумно оставить как «минимальный» вариант сметы.

АРМ целиком (вариант А): 1 317,99 + LG 24U411A-B 277,70 + Logitech MK120 68,49 = **1 664,18 BYN**; 12 АРМ — **19 970 BYN** без ОС; с Windows 11 Pro OEI (медиана 150 BYN) — 1 814,18 / 21 770 BYN. Монитор LG 24U411A-B выбран как IPS 23,8" с наибольшим числом продавцов (75) и гарантией 24 мес; AOC 24B15H3 на 29 BYN дешевле и равноценен.

### 4.2. Принтер

Критерии: Ethernet (обязательно), автодуплекс, стоимость цветного и чёрно-белого отпечатка, цена, нагрузка, доступность расходников в РБ.

Оптимальный выбор — **Pantum CP1100DN** (или CP1100DW, если нужен Wi-Fi), от 1 370 BYN: самый низкий среди лазерных принтеров РБ отпечаток (0,084 чб / 0,406 цв BYN), Ethernet и автодуплекс, ресурс сменных картриджей 3 000/2 300 стр., нагрузка до 1 500 стр./мес при максимуме 30 000, 17–20 продавцов. Самый дешёвый лазерный конкурент HP Color Laser 150nw (от 1 033 BYN) проигрывает по цвету в 3,3 раза (1,335 BYN/стр.), печатает цвет 4 стр./мин и не имеет автодуплекса — экономия 340 BYN на покупке съедается за первые ~5 000 цветных страниц. HP 4203dn быстрее (33 стр./мин) и прочнее (50 000 стр./мес), но при 300 стр./мес это избыточно, а тонеры 230A дают 1,014 BYN/стр. Xerox C230 близок по цене и чб-отпечатку, но цветные тонеры в РБ 545–600 BYN, продавцов два. Для двух лабораторий: 2 × 1 370 = **2 740 BYN**.

Альтернатива при больших объёмах цветной печати (графики, микрофотографии): Epson EcoTank L6490 (МФУ, от 1 291 BYN, Ethernet, дуплекс, 37 продавцов) с отпечатком на порядок дешевле (по подтверждённому L6290: 0,0075 / 0,027 BYN), но ресурс чернил 112 для L6490 не подтверждён, цветная ISO-скорость 9,5 стр./мин, и струйная головка при редкой печати в лаборатории может пересыхать.

### 4.3. Файловый сервер

Критерии: RAID1, управление пользователями/квотами, снапшоты (защита от случайного удаления и шифровальщиков), поддержка домена/LDAP, VPN для второго здания, энергопотребление и администрирование, цена, доступность в РБ.

Оптимальный класс — **(а) готовый 2-дисковый NAS**, конкретно **Synology DS223j**. Обоснование: для 12 пользователей и задачи «обмен файлами + личные папки» нагрузка низкая, 1GbE и 1 ГБ ОЗУ достаточны; DSM даёт RAID1/SHR, пользователей, группы, квоты, снапшоты Btrfs, клиент AD/LDAP и встроенный VPN Server без администрирования ОС; потребление ~10–20 Вт, гарантия на устройство целиком, 38 продавцов и наличие на складе Onliner. QNAP TS-233 (от 909,99) имеет 2 ГБ ОЗУ и WireGuard в QVPN, но файловая система ext4 без снапшотов ФС — для хранения личных файлов сотрудников это минус, который перевешивает выигрыш в памяти. Класс (б) в рознице РБ фактически отсутствует, а ближайшая серверная платформа на Xeon E стоит 4 742 BYN без дисков, ОС и ИБП — для бюджетной сети на 12 человек это избыточно. Класс (в) по цене сопоставим с NAS, но требует сборки, настройки TrueNAS/ZFS и постоянного администрирования; число отсеков 3,5" в карточках ПК не подтверждено.

Для доступа из второго здания VPN логичнее терминировать на маршрутизаторах зданий (site-to-site), оставив NAS обычным SMB-сервером в ЛВС первого здания; VPN Server на NAS — резервный канал удалённого доступа.

Рекомендуемая конфигурация (минимальные цены 26.10.2026):

| Позиция | Цена, BYN | Источник |
|---|---|---|
| Synology DiskStation DS223j | 778,00 (Onliner) / 757,00 (shop.by) | [Onliner](https://catalog.onliner.by/nas/synology/ds223j) |
| 2 × Seagate IronWolf 4 TB ST4000VN006 (RAID1 → 4 ТБ полезных, ~330 ГБ на пользователя) | 2 × 730,18 = 1 460,36 | [Onliner](https://catalog.onliner.by/hdd/seagate/st4000vn006) |
| ИБП CyberPower UT850EG (850 ВА / 425 Вт, USB-HID, совместим с DSM) | 232,85 (Onliner) / 218,90 (shop.by) | [Onliner](https://catalog.onliner.by/ups/cyberpower/ut850eg) |
| **Итого** | **≈ 2 471 BYN** (Onliner) / ≈ 2 436 BYN (shop.by) | |

Варианты: WD Red Plus 4 TB вместо IronWolf — +240 BYN (итого ≈ 2 711); ИБП APC BX750MI-GR — +270 BYN; Synology DS223 (2 ГБ ОЗУ) — +270–450 BYN.

### 4.4. Сводная смета оконечного оборудования

| Статья | Кол-во | Цена, BYN | Сумма, BYN |
|---|---|---|---|
| АРМ (TGPC Office 93037 I-X + LG 24U411A-B + Logitech MK120) | 12 | 1 664,18 | 19 970,16 |
| Windows 11 Pro OEI (опционально) | 12 | 150,00 | 1 800,00 |
| Принтер Pantum CP1100DN | 2 | 1 370,00 | 2 740,00 |
| Файловый сервер (DS223j + 2 × IronWolf 4 TB + UT850EG) | 1 | 2 471,21 | 2 471,21 |
| **Итого без ОС** | | | **25 181,37** |
| **Итого с Windows** | | | **26 981,37** |

---

## 5. Что осталось неподтверждённым

- Сетевой адаптер и ОС у сборок Iven (181168, 179552) и скорость LAN у TGPC 87231 — в карточках Onliner не указаны.
- Цены 21vek.by (Z-Tech, Jet, MultiOffice) — сайт недоступен из среды исследования.
- Цены серверов HPE/Dell/Lenovo в РБ — в рознице не найдены, только РФ/EU ориентиры.
- Число SATA-портов и отсеков 3,5" у конкретных системных блоков для класса (в).
- Ресурс чернил Epson 112 для L6490; цена W2212X для HP M255dw; USB у Powercom RPT-800A (источники противоречат).
- Программные функции Synology/TerraMaster/Asustor (VPN, AD/LDAP, снапшоты) — по документации производителей, без прямой выборки с их сайтов (403).

## 6. Источники (сводно)

ПК, мониторы, наборы, ОС: карточки и API Onliner — [TGPC 93037](https://catalog.onliner.by/desktoppc/tgpc/tgpc93037), [TGPC 93034](https://catalog.onliner.by/desktoppc/tgpc/tgpc93034), [TGPC 93039](https://catalog.onliner.by/desktoppc/tgpc/tgpc93039), [TGPC 93090](https://catalog.onliner.by/desktoppc/tgpc/tgpc93090), [TGPC 87231](https://catalog.onliner.by/desktoppc/tgpc/tgpc87231), [Iven 181168](https://catalog.onliner.by/desktoppc/iven/iven181168), [Iven 181177](https://catalog.onliner.by/desktoppc/iven/iven181177), [Iven 179552](https://catalog.onliner.by/desktoppc/iven/iven179552), [Chuwi LarkBox X](https://catalog.onliner.by/minipc/chuwi/larkboxxi38256w1), [Lenovo neo 50q Gen 5](https://catalog.onliner.by/minipc/lenovo/13b90027gp), [HP Pro Mini 400 G9](https://catalog.onliner.by/minipc/hp/935x8ea), [LG 24U411A-B](https://catalog.onliner.by/display/lg/24u411ab), [AOC 24B15H3](https://catalog.onliner.by/display/aoc/24b15h3), [HAFF T238](https://catalog.onliner.by/display/haff/t238t238), [Logitech MK120](https://catalog.onliner.by/peripheralkits/logitech/desktopmk120), [Logitech MK220](https://catalog.onliner.by/peripheralkits/logitech/combo_mk220), [Windows 11 Pro OEI](https://catalog.onliner.by/antivirus/microsoft/fqc10547); список ПК 800–1 400 BYN: `https://catalog.onliner.by/sdapi/catalog.api/search/desktoppc?price[from]=800&price[to]=1400`; shop.by — [Lenovo neo 50q](https://shop.by/nettopy/lenovo_thinkcentre_neo_50q_gen_5_13b9008dsa/), [Intel Core i3-12100](https://shop.by/kompyutery/intel_core_i3_12100/).

Принтеры: характеристики — [Pantum CP1100DN](https://www.pantum.ru/products/laser-devices/cvetnoj-printer-cp1100dn/), [CP1100DW](https://www.pantum.ru/products/laser-devices/cvetnoj-printer-cp1100dw/), [CP2100DW](https://www.pantum.ru/products/laser-devices/cvetnoj-printer-cp2100dw/), [HP 150nw](https://www.hp.com/ie-en/products/printers/product-details/product-specifications/24494363), [HP 4203dn](https://www.hp.com/emea_africa-en/products/printers/product-details/product-specifications/38348063), [HP M255dw](https://www.hp.com/emea_africa-en/products/printers/product-details/product-specifications/29448871), [Xerox C230](https://www.xerox.com/en-us/office/printers/xerox-c230-printer/specifications), [Kyocera PA2100cwx](https://www.kyoceradocumentsolutions.com/asia/en/products/printer/ecosys-pa2100cwx/spec-sheet.html), [Epson L6290](https://www.epson.eu/en_EU/products/printers/inkjet/consumer/ecotank-l6290-multifunction-wi-fi-ink-tank-a4-printer,-with-up-to-3-years-of-ink-included/p/30260), [Canon LBP631Cw](https://sfcc-service.canon-europe.com/5159C004/5159C004.html). Цены принтеров и картриджей — карточки Onliner/shop.by/amd.by, перечисленные в таблице 2.2; картриджи: [CTL-1100XK](https://catalog.onliner.by/cartridges/pantum/ctl1100xk), [XC](https://catalog.onliner.by/cartridges/pantum/ctl1100xc), [XM](https://catalog.onliner.by/cartridges/pantum/ctl1100xm), [XY](https://catalog.onliner.by/cartridges/pantum/ctl1100xy), [CTL-R2100HK](https://catalog.onliner.by/cartridges/pantum/ctlr2100hk), [HP W2070A](https://shop.by/kartridzhi_dlya_printerov_i_mfu/hp_117a_w2070a/), [W2071A](https://shop.by/kartridzhi_dlya_printerov_i_mfu/hp_117a_w2071a/), [W2072A](https://shop.by/kartridzhi_dlya_printerov_i_mfu/hp_117a_w2072a/), [W2073A](https://shop.by/kartridzhi_dlya_printerov_i_mfu/hp_117a_w2073a/), [W2300A](https://catalog.onliner.by/cartridges/hp/w2300a), [W2301A](https://catalog.onliner.by/cartridges/hp/w2301a), [W2300X](https://catalog.onliner.by/cartridges/hp/w2300x), [Xerox 006R04395](https://catalog.onliner.by/cartridges/xerox/1006r04395), [006R04396](https://catalog.onliner.by/cartridges/xerox/1006r04396), [Kyocera TK-5430K](https://catalog.onliner.by/cartridges/kyocera/tk5430k), [Epson 101 BK](https://catalog.onliner.by/cartridges/epson/c13t03v14a), [Epson 101 C](https://catalog.onliner.by/cartridges/epson/c13t03v24a), [HP 963XL BK](https://catalog.onliner.by/cartridges/hp/3ja30ae).

NAS, серверы, диски, ИБП: [Onliner NAS](https://catalog.onliner.by/nas), [DS223j](https://catalog.onliner.by/nas/synology/ds223j), [shop.by сетевые накопители](https://shop.by/setevye_nakopiteli/), [shop.by серверы](https://shop.by/servery/), [QNAP TS-233](https://www.qnap.com/en/product/ts-233/specs/hardware), [Asustor AS3302T](https://www.asustor.com/en/product?p_id=72), [TerraMaster F2-425](https://www.terra-master.com/products/f2-425), [UGREEN DXP2800](https://ai.ugreen.com/products/ugreen-nasync-dxp2800-nas-storage); ориентиры серверов вне РБ — hpeserver.ru, servermall.ru, hpe-servers.ru, dell-servers.ru, serverict.com, bechtle.com; диски и ИБП — карточки Onliner/shop.by из таблиц 3.4–3.5.
