# Маршрутизаторы и канал между зданиями — исследование рынка РБ

Дата сбора данных: 26.10.2026. Цены — в BYN, по данным агрегаторов catalog.onliner.by (API каталога: min/max по всем продавцам) и shop.by (минимальная цена и список магазинов) на эту дату. Технические данные — с сайтов производителей (mikrotik.com, static.tp-link.com / omadanetworks.com, keenetic.com, store.ui.com / techspecs.ui.com). Всё, что не удалось проверить по первоисточнику, помечено «не подтверждено».

Ограничения сбора: сайты a1.by, mts.by, bn.by, belgie.by, mpt.gov.by, pravo.by, 5element.by, deal.by, ultra.by, 21vek.by отказывали в соединении или отдавали 403 при запросе из-за рубежа; по ним использованы только косвенные источники (сниппеты поисковой выдачи, статьи Onliner). Отдельные страницы предложений Onliner (`/prices`) грузятся скриптом и в выгрузку не попали — поэтому для Onliner приведён диапазон min–max из API каталога и число предложений, а поимённый список магазинов взят с shop.by.

---

## 1. Маршрутизаторы для зданий (нужно 2 шт.)

### 1.1. Сводная таблица: цена и доступность в РБ (26.10.2026)

| Модель | Onliner: мин.–макс., BYN (число предложений) | shop.by: мин. цена, BYN (число предложений) | Типовая цена*, BYN | Наличие | MSRP производителя |
|---|---|---|---|---|---|
| MikroTik hEX (RB750Gr3) | 243,13–461,57 (27) [O1] | 223,00 — 100pro.by (23) [S1] | ≈ 225–250 | в наличии у многих (stox.by >10 шт.) | $59,95 [M1] |
| MikroTik hEX refresh (E50UG) | 264,12–445,33 (19) [O2] | 244,00 — newton/lix.by/100pro.by (16) [S2] | ≈ 245–265 | в наличии (stox.by >10 шт., ряд магазинов по 1 шт.) | $59,95 [M2] |
| MikroTik hEX S (RB760iGS) | 280,00–524,56 (32) [O3] | 280,00 — 100pro.by (24) [S3] | ≈ 280–305 | в наличии (stox.by >10 шт.) | $79,00 [M3] |
| MikroTik hEX S (2025, E60iUGS) | 322,88–476,21 (22) [O4] | — (не искал) | ≈ 325–350 | в наличии (22 предложения) | $69,00 [M4] |
| MikroTik RB5009UG+S+IN | 990,00–1452,86 (22) [O5] | 898,40 — vexo.by (22) [S4] | ≈ 900–990 | в наличии | $219,00 [M5] |
| MikroTik RB4011iGS+RM | 807,46–1105,95 (27) [O6] | 740,00 — 100pro.by (21) [S5] | ≈ 740–810 | в наличии (stox.by, newton.by >10 шт.) | не указана на странице |
| TP-Link ER605 (v2) | 309,43–459,68 (19) [O7] | 285,00 — 100pro.by (10) [S6] | ≈ 285–310 | в наличии (у части магазинов по 1 шт.) | — |
| TP-Link ER7206 (TL-ER7206) | 544,44–673,70 (24) [O8] | 443,00 — 100pro.by (21) [S7] | ≈ 445–545 | в наличии | — |
| TP-Link ER7212PC | «Нет в продаже», цен нет [O9] | не найден | — | **нет в продаже в РБ**; на сайте производителя (CH) отмечен как End of Life [T3] | — |
| Keenetic Giga KN-1012 (в ЕС продаётся как Keenetic Hero KN-1012) | 659,00–742,23 (9) [O10] | 599,00 — 1ye.by (7) [S8] | ≈ 600–660 | в наличии (stox.by 5 шт.) | — |
| Keenetic Hero 4G+ KN-2311 | карточка есть, цен нет [O11] | — | — | **не продаётся** (нет предложений) | — |
| Ubiquiti EdgeRouter X (ER-X) | 489,70 (2) [O12] | 342,10 — vexo.by (2) [S9] | ≈ 342–490 | единичные предложения (stox.by 1 шт.) | $59,00 [U1] |
| Ubiquiti EdgeRouter X SFP (ER-X-SFP) | 360,84–457,25 (API) [O12] | — | ≈ 360 | единичные | — |
| Ubiquiti UniFi Cloud Gateway Ultra (UCG-Ultra) | 638,05–791,82 (15) [O13] | 544,00 — 100pro.by (18) [S10] | ≈ 545–640 | в наличии (100pro.by 2 шт.) | $129,00 (в US-магазине «sold out») [U2] |

\* «Типовая» — диапазон, в который попадает большинство предложений (нижняя часть списка shop.by / нижняя граница Onliner). Верхние значения Onliner (400–1400 BYN) — единичные завышенные предложения.

Примечание: В Беларуси продаётся модель KN-1012 под именем «Giga»; на keenetic.com та же KN-1012 описана как «Keenetic Hero» [K1]. Отдельной модели «Hero» с другим железом в РБ нет (Hero 4G+ KN-2311 — LTE-роутер, в продаже отсутствует [O11]).

### 1.2. Сводная таблица: технические характеристики (по данным производителей)

| Параметр | hEX (RB750Gr3) | hEX refresh (E50UG) | hEX S (RB760iGS) | hEX S 2025 (E60iUGS) | RB5009UG+S+IN | RB4011iGS+RM | TP-Link ER605 v2 | TP-Link ER7206 | Keenetic Giga KN-1012 | Ubiquiti ER-X | Ubiquiti UCG-Ultra |
|---|---|---|---|---|---|---|---|---|---|---|---|
| Источник | [M1] | [M2] | [M3] | [M4] | [M5] | [M6] | [T1] | [T2] | [K1] | [U1] | [U3] |
| CPU | MT7621A, 2 ядра/4 потока, 880 МГц (MIPS) | EN7562CT, 2 ядра, 950 МГц (ARM32) | MT7621A, 2/4, 880 МГц | EN7562CT, 2 ядра, 950 МГц | Marvell 88F7040, 4 ядра, до 1,4 ГГц (ARM64) | AL21400, 4 ядра, 533–1900 МГц (ARM32) | не указан в datasheet | 2× Cortex-A53 | MT7981B, 2 ядра, 1,3 ГГц | MIPS1004Kc, 2 ядра, 880 МГц | 4× Cortex-A53, 1,5 ГГц |
| RAM / Flash | 256 MB / 16 MB + microSD | 512 MB / 128 MB NAND | 256 MB / 16 MB + microSD | 512 MB / 128 MB NAND | 1 GB / 1 GB NAND | 1 GB / 512 MB NAND | 256 MB DDR / 128 MB NAND | 512 MB DDR4 / 128 MB NAND | 512 MB DDR4 / 256 MB | 256 MB DDR3 / 256 MB NAND | 3 GB / 16 GB eMMC |
| Порты GbE | 5 | 5 | 5 | 5 | 7×1G + 1×2,5G | 10 | 5 (1 WAN + 2 WAN/LAN + 2 LAN) | 5 (1 WAN + 4 WAN/LAN) | 4×1G + 1×2,5G combo | 5 | 4×1G + 2×2,5G |
| SFP | нет | нет | 1× SFP (1,25G) | 1× SFP (до 2,5G) | 1× SFP+ (10G) | 1× SFP+ | нет | 1× SFP (WAN/LAN) | combo RJ45/SFP 2,5G | нет (есть у ER-X-SFP) | нет |
| IPsec, заявл. (условия) | 231,8 Мбит/с (1 туннель, AES-128-CBC+SHA1, 1400 B, RFC2544, 0,1 % потерь) | 351,7 Мбит/с (те же условия) | 231,8 Мбит/с (те же) | 351,7 Мбит/с (те же) | 1354,1 Мбит/с (те же) | 1031,5 Мбит/с (те же) | 250,94 Мбит/с (ESP-SHA256-AES256); 239,39 (SHA1-AES256) | 844,8 Мбит/с (ESP-SHA256-AES256); 885,3 (SHA1-AES256) | не публикуется | не публикуется (**не подтверждено**) | не публикуется (заявлено только IDS/IPS 1 Гбит/с) |
| IPsec, 256 туннелей AES-256-CBC+SHA256, 1400 B | 117,6 | 189,3 | 117,6 | 188,2 (AES-128+SHA1) | 1409,0 | 1283,5 | — | — | — | — | — |
| WireGuard | да (RouterOS 7, с v7.1) [M7][M8]; скорость не публикуется | да | да | да | да | да | да; 82,62 Мбит/с | да; 341,3 Мбит/с | да; до 460 Мбит/с | нет в EdgeOS (**не подтверждено**) | да (сервер/клиент) |
| Маршрутизация 1518 B с 25 правилами IP-filter | 766,4 Мбит/с | 1430,3 | 766,4 | 1430,3 | 9365,6 | 7209,9 | NAT 941–944 Мбит/с (без фильтров) | NAT 940–945 (без фильтров) | до 1800 Мбит/с IPoE (дуплекс) | — | — |
| IPv6: firewall с состоянием | да: `/ipv6 firewall filter`, matcher `connection-state` (established/related/new/invalid), фильтр по protocol/port [M9] | да | да | да | да | да | «IPv4/IPv6 ACL», «Stateful ACL» [T1] | «IPv4/IPv6 ACL», «Stateful ACL» [T2] | «IPv6 Dual Stack», межсетевой экран (детали по IPv6-ACL не найдены) | да (EdgeOS, Vyatta-подобный firewall) — **не подтверждено по первоисточнику** | «IPv6 ISP support», stateful/zone-based firewall [U3] |
| IPv6: DHCPv6-PD (клиент) / RA | да: `/ipv6 dhcp-client request=prefix` + pool; RA через `/ipv6 nd` (RADVD) [M10][M11] | да | да | да | да | да | режимы WAN: StaticIP/SLAAC/DHCPv6/PPPoE/6to4/PassThrough; «DHCPv6 PD Server (only in Standalone Mode)» | то же | DHCPv6-PD: заявлен Dual Stack, детали не найдены | — | «IPv6 ISP Support» |
| 802.1Q VLAN | да | да | да | да | да | да | да (802.1Q VLAN, Bridge VLAN) | да | да | да | да |
| PoE-out | нет | нет | Ether5, пассивный, до 57 В, 0,5 А | Ether5, пассивный, 0,5 А | нет | Ether10, пассивный | нет | нет | нет | 1 порт passthrough 24 В | нет |
| PoE-in | пассивный 8–30 В | пассивный 12–28 В | 802.3af/at | 802.3af/at 18–57 В | 802.3af/at (порт 1) | пассивный 18–57 В | нет | нет | нет | пассивный 24 В | нет (USB-C 5 В) |
| Рабочая температура | −40…+60 °C | −40…+70 °C | −40…+70 °C | −40…+70 °C | −40…+60 °C | −40…+70 °C | 0…+40 °C | 0…+40 °C | 0…+40 °C | −10…+45 °C | −10…+40 °C |
| Управление | CLI (SSH/Telnet/serial), WebFig, WinBox, API | то же | то же | то же | то же | то же | Web, Omada-контроллер (облако/OC200), приложение; «CLI (only in Standalone Mode)» | то же | Web, мобильное приложение, облако; CLI по Telnet/SSH [K1][K2] | EdgeOS web + CLI (по описанию Ubiquiti в ритейле [U4]) | только UniFi Network (встроенный контроллер), приложение |
| CLI для документирования конфигурации | да: `/export` даёт полный текст конфигурации | да | да | да | да | да | ограниченный CLI только в автономном режиме; экспорт конфигурации — бинарный файл («Export & Import Configuration») | то же | `show running-config` / экспорт startup-config (текст) [K2] | `show configuration` (EdgeOS) — **не подтверждено по первоисточнику** | нет (конфигурация в контроллере) |
| Потребление, макс. | 10 Вт | 10 Вт | 24 Вт (с PoE-out) | — | 25 Вт | 33 Вт | 7,94 Вт | 7,5 Вт | — | 5 Вт | 6,2 Вт |
| Лицензия / ПО | RouterOS L4 | RouterOS v7, L4 | L4 | L4 | RouterOS v7, L5 | v7, L5 | Omada SDN | Omada SDN | KeeneticOS | EdgeOS | UniFi OS |

Пояснения к строке IPsec MikroTik: цифры взяты из раздела «Test results» на страницах продуктов mikrotik.com; методика — Xena2544/RFC 2544, 30+ с на попытку, допуск потерь 0,1 %; производитель оговаривает, что «другие конфигурации, скорее всего, дадут меньшие результаты». На hEX/hEX S (MT7621) аппаратное ускорение IPsec отсутствует; на RB5009/RB4011 — есть. Значение для одного туннеля AES-128-CBC+SHA1 при 1400 B — верхняя оценка.

TP-Link: datasheet ER605 v2.30 (октябрь 2026) [T1] и ER7206 v2.30 [T2] прямо оговаривают: «Rated specifications are based on test results using the specific software. Device performance may vary». Более старые datasheet ER605 (середина 2025) давали IPsec ≈ 260 Мбит/с и WireGuard ≈ 131 Мбит/с [T4] — т. е. цифры меняются от прошивки к прошивке.

### 1.3. Что отсеивается и почему

- **TP-Link ER7212PC** — нет в продаже в РБ (Onliner: «Нет в продаже», shop.by: не найден), на сайте производителя помечен End of Life [T3]. Кроме того, его встроенный PoE-коммутатор на 8 портов и контроллер избыточны для 6 ПК на здание.
- **Keenetic Hero 4G+ (KN-2311)** — LTE-роутер на MT7621A с WireGuard «до 180 Мбит/с» [K3], предложений в РБ нет [O11].
- **Ubiquiti EdgeRouter X** — 2 предложения в РБ, платформа EdgeMAX давно не обновляется; производитель не публикует IPsec-производительность; рабочая температура −10…+45 °C.
- **UCG-Ultra** — требует экосистемы UniFi; нет текстовой CLI-конфигурации для документирования; IPsec-производительность не публикуется; в US-магазине «sold out».
- **RB5009 / RB4011** — IPsec > 1 Гбит/с и 10G SFP+ не востребованы сетью из 12 ПК; цена пары 1,5–2 тыс. BYN.
- **Keenetic Giga KN-1012** — домашний Wi-Fi-роутер за 600–660 BYN; Wi-Fi в проекте обеспечивают отдельные точки доступа, а IPsec-скорость не публикуется (только WireGuard 460 Мбит/с).

---

## 2. Способы связи зданий на 5 км

### 2.1. Вариант (а): site-to-site VPN поверх Internet (IPsec IKEv2 или WireGuard)

Что требуется:
- Статический публичный IPv4 с обеих сторон (по условию задачи провайдер его даёт; у Белтелекома — 28,00 руб. единовременно + 4,80 руб./мес за адрес [B1][B2]). Для IPv6 — публичная подсеть (у Белтелекома /56 — 5,80 руб./мес [B1]).
- Маршрутизатор с IPsec IKEv2 (RouterOS: `/ip ipsec` с профилями IKEv2; TP-Link ER605/ER7206: «IKE v1/v2», «LAN-to-LAN» [T1][T2]) либо WireGuard (RouterOS ≥ 7.1 [M7][M8]).
- Пропускная способность туннеля ограничена: (1) IPsec-производительностью маршрутизатора (hEX refresh — до ~350 Мбит/с в идеальных условиях, реально меньше на малых пакетах: 142,5 Мбит/с при 512 B [M2]); (2) скоростью отдачи у провайдера: «Бизнес-комплект 500» Белтелекома — 500/250 Мбит/с, «Бизнес-комплект 200» — 200/200 Мбит/с [B1]. То есть трафик к файловому серверу из другого здания будет ограничен ~200–250 Мбит/с тарифом, а не маршрутизатором.
- Стоимость: 0 сверх цены маршрутизаторов и абонплаты за Интернет, которая и так нужна по условию.
- Для IPv6-трафика между зданиями: либо отдельный IPv6-транспорт туннеля, либо маршрутизация IPv6 внутри того же IPsec/WireGuard-туннеля (RouterOS поддерживает IPv6 в WireGuard с 7.1 [M8]).

### 2.2. Вариант (б): радиомост 5 ГГц на 5 км

Цены за одно устройство (мост = 2 устройства), 26.10.2026:

| Устройство | Усиление / стандарт | Темп. | MSRP | Onliner, BYN (предложений) | Цена пары (по минимуму), BYN | Источник |
|---|---|---|---|---|---|---|
| MikroTik LHG 5 ac (RBLHGG-5acD) | 24,5 dBi, 802.11ac 2×2, 25 dBm, 5150–5875 МГц | −40…+70 °C | $99 | карточка есть, **цен нет**; на mikrotik.com отмечен «Discontinued» | — | [M12][O14] |
| MikroTik LHG XL 5 ac (RBLHGG-5acD-XL) | 27 dBi, 802.11ac, 25 dBm | −40…+70 °C | $117 | 1897,00–2130,10 (29) — **цена не соответствует MSRP (≈ 4-pack), не подтверждено** | — | [M13][O15] |
| MikroTik SXTsq 5 ac (RBSXTsqG-5acD) | 16 dBi, 802.11ac, 25 dBm | −40…+70 °C | $65 | 289,22–809,50 (10) | ≈ 580 | [M14][O16] |
| MikroTik SXTsq 5 ax | — (802.11ax) | — | — | 261,64–456,58 (API) | ≈ 525 | [O16] |
| MikroTik Wireless Wire Dish (RBLHGG-60adkit) | 60 ГГц, 2 Гбит/с, комплект из 2 шт. | −40…+70 °C | $298/пара | — | — | **до 1500 м** по данным производителя — на 5 км не подходит [M15] |
| Ubiquiti NanoBeam 5AC Gen2 (NBE-5AC-Gen2) | 19 dBi, airMAX ac | −40…+80 °C | $99 | 418,70–1240,72 (35) | ≈ 840 | [U5][O17] |
| Ubiquiti LiteBeam 5AC Gen2 (LBE-5AC-Gen2) | 23 dBi, 25 dBm, «450+ Mbps» | −40…+70 °C | $65 | 456,44–1283,57 (35) | ≈ 913 | [U6][O18] |
| Ubiquiti PowerBeam 5AC Gen2 (PBE-5AC-Gen2) | 25 dBi | −40…+70 °C | $345 (US store) | 668,31–1847,63 (33) | ≈ 1337 | [U7][O19] |

Дальность: Ubiquiti и MikroTik на страницах продуктов не указывают дальность в км; в карточках Onliner (данные продавцов) NanoBeam 5AC Gen2 — «10 000 м», PowerBeam 5AC Gen2 — «25 000 м» на открытой местности [O17][O19]. 5 км для антенн 19–27 dBi — штатная дистанция, но только при прямой видимости.

Требования прямой видимости и высоты мачт (первая зона Френеля):
- Радиус первой зоны Френеля в середине трассы: R = 17,31·√(d1·d2/(f·d)) (м; d, d1, d2 — км; f — ГГц) [F1]. Для d = 5 км, f = 5,5 ГГц: R ≈ 8,3 м (при 5,15 ГГц — 8,5 м). Для нормальной работы принято оставлять свободными не менее 60 % радиуса: ≈ 5,0 м (расчёт по формуле из [F1]; практические пороги затухания — [F2]).
- Выпуклость Земли в середине 5-километровой трассы: h = d1·d2/(2·k·Rз) ≈ 0,37 м при k = 4/3 (0,49 м при k = 1) — на 5 км поправка невелика.
- Итог: луч должен проходить минимум на ~5–5,5 м выше самого высокого препятствия в середине трассы (деревья, крыши). При одноэтажных зданиях (карниз ~4–5 м) и застройке/деревьях 10–15 м по трассе потребуются мачты 10–20 м на каждом здании; точная высота определяется только по профилю трассы (site survey). Без прямой видимости вариант нереализуем.

Регистрация РЭС и разрешения в РБ (кратко):
- В Беларуси РЭС подлежат регистрации владельцем в течение 14 дней после ввоза/получения, а эксплуатация требует разрешения, которое выдаёт РУП «БелГИЭ» при Минсвязи, если иное не установлено законодательством (Положение о регистрации РЭС и ВЧУ, утв. Советом Министров — по сниппету поисковой выдачи, текст на pravo.by недоступен из-за рубежа; **требует проверки по первоисточнику**) [R1].
- Перечень РЭС, не подлежащих регистрации, установлен постановлением Минсвязи от 14.06.2013 № 7 (и связанным постановлением от 01.07.2013 № 9). По разъяснению БелГИЭ (цитируется Onliner, 2015): без регистрации и без разрешения БелГИЭ эксплуатируется только «оконечная (абонентская) аппаратура Wi-Fi, устанавливаемая внутри зданий и сооружений и не использующая антенны, устанавливаемые вне зданий и сооружений» [R2].
- По FAQ БелГИЭ (сниппет поисковой выдачи, сайт belgie.by из-за рубежа недоступен): для беспроводного широкополосного доступа внутри зданий разрешены полосы 2400–2483,5 МГц (ЭИИМ до 100 мВт) и 5150–5350 / 5650–5725 МГц (ЭИИМ не более 200 мВт) [R3]. **Не подтверждено по первоисточнику.**
- Следствие: наружный фиксированный радиомост 5 ГГц с антеннами 19–27 dBi (ЭИИМ порядка 25 дБм + 27 dBi ≈ 52 дБм ≫ 23 дБм/200 мВт) под исключение не попадает — нужны регистрация РЭС и разрешение на использование радиочастотного спектра в БелГИЭ (плюс, по практике, согласование частот). Стоимость и сроки процедуры с официального сайта получить не удалось (**не подтверждено**).

Итого по (б): оборудование 525–1340 BYN за пару + мачты/монтаж/грозозащита + разрешительная процедура БелГИЭ; плюс обязательна прямая видимость, которую по условию задачи никто не гарантирует.

### 2.3. Вариант (в): аренда канала/VPN у оператора

- **Белтелеком, «Объединение корпоративных сетей по IP-протоколу» (VPN, только для юрлиц)**, тарифы с 06.04.2026 без НДС [B3]: организация точки подключения — 32,00 руб. единовременно; абонплата за точку — 4,00 руб./мес; плата за полосу — за каждый 1 Мбит/с в месяц: 1–5 Мбит/с — 27,60; 6–10 — 24,50; 11–25 — 21,60; 26–50 — 18,90; 51–75 — 16,10; 76–100 — 13,60; 101 и выше — 9,00 руб. Пример: точка 100 Мбит/с ≈ 100 × 13,60 = 1 360 руб./мес (без НДС, расчёт по тарифной сетке); с 01.01.2026 действует «программа лояльности VPN» — скидка 50 % на трафик для точек в одном населённом пункте, подключённых до 01.01.2026 [B4]. Для двух зданий — порядка 1–3 тыс. руб./мес.
- **Белтелеком, «Предоставление в пользование каналов местного уровня»** (выделенные каналы 64 кбит/с…140 Мбит/с, тарифы с 06.04.2026 без НДС) [B5]: организация канала — 96,00 руб.; абонплата: 2048 кбит/с — 90,60; 10 Мбит/с — 428,40; 20 Мбит/с — 770,70 руб./мес; свыше 20 Мбит/с — доплата за каждый Мбит/с (21–30: 36,20 … 91–100: 29,20 … свыше 155: 19,10 руб.). Канал 100 Мбит/с по этой сетке — порядка 3 тыс. руб./мес (оценка). Страница не называет это «Ethernet L2 точка-точка» — тип интерфейса уточняется у оператора.
- **Белтелеком, «Канал доступа»** (линия от узла Белтелекома до оборудования заказчика, под базовую услугу) [B6]: 135,00 руб. организация; 7,95–16,70 руб./мес при трассе до 3 км; +2,25 руб./мес за каждый км свыше 3 км. Это не самостоятельный канал между зданиями, а «последняя миля» к базовой услуге.
- **A1**: в тарифном документе для юрлиц упоминаются план «Сеть» (объединение офисов клиента в одном городе) и корпоративные планы P6–P150 до 150 Мбит/с; цены из документа извлечь не удалось (a1.by недоступен из-за рубежа) — **не подтверждено** [A1].
- **МТС, «Деловая сеть»**: публичных тарифов на L2-канал «точка-точка» не найдено; сайты недоступны из-за рубежа — **не подтверждено**.
- Вывод: аренда канала у оператора на порядок дороже VPN через Интернет (сотни–тысячи рублей в месяц против 0) и для бюджетной сети не оправдана.

---

## 3. Тарифы провайдеров для юрлиц (Ethernet ~1 Гбит/с, статический IPv4, IPv6)

| Провайдер | Тариф / услуга | Скорость | Цена, руб./мес (без НДС) | Статический IPv4 | IPv6 юрлицам | Источник |
|---|---|---|---|---|---|---|
| Белтелеком | «Бизнес-комплект 500» | 500/250 Мбит/с | 299,00 | 28,00 единовр. + 4,80/мес за адрес | да: выделение подсети 28,40 единовр.; /64 — 0,0256/мес; /56 — 5,80/мес; /48 — 1 190,00/мес (только при наличии спецлицензии Минсвязи) | [B1] (тарифы с 06.04.2026) |
| Белтелеком | «Бизнес-комплект 200» | 200/200 | 146,60 | то же | то же | [B1] |
| Белтелеком | «Бизнес-комплект 100» | 100/50 | 74,90 | то же | то же | [B1] |
| Белтелеком | «Постоянный доступ по выделенным линиям» | до 10 Мбит/с — 124,00; свыше 10 — +9,52 за каждый Мбит/с | 1000 Мбит/с ≈ 124,00 + 990 × 9,52 = 9 549 (расчёт по сетке) + порт 1 Гбит/с 27,00/мес | 28,00 + 4,80/мес | да, те же тарифы на /64, /56, /48 | [B2] |
| Белтелеком | пакеты «Бизнес-кейс» (Интернет + телефония) | 500/250 — 350,00; 100/50 — 82,80 | — | не указано | — | [B7] |
| Белтелеком | изменение тарифов с 01.11.2026 | — | затрагивает физлиц и юрлиц; конкретных цифр для бизнес-интернета в публикации нет | — | — | [B8] |
| A1 | «Интернет для офиса», «Бизнес онлайн 60/80/100/150» | до 150 Мбит/с (по документам A1) | не получено | не получено | не получено | [A1] — **не подтверждено** |
| МТС | «МТС Драйвер» (пакет: интернет для офиса + мобильная связь), июль 2026 | до 200 Мбит/с | скидка до 60 % первый год, цены в публикации нет | не получено | не получено | [MT1] — **не подтверждено** |
| Деловая сеть (bn.by) | — | — | актуальных данных нет (только 2015 г.) | — | — | **не подтверждено** |

Выводы по п. 3:
- Публичный тариф «Ethernet 1 Гбит/с» для юрлиц у Белтелекома не обнаружен: максимум в линейке «Бизнес-комплект» — 500/250 Мбит/с за 299 руб./мес; гигабит доступен только как выделенная линия по помегабитной сетке (≈ 9,5 тыс. руб./мес расчётно) [B1][B2]. Для бюджетной сети на 6 ПК в здании реалистичен «Бизнес-комплект 100/200».
- IPv6 у Белтелекома юрлицам предоставляется официально и дёшево: /56 — 5,80 руб./мес [B1]. Это подтверждает условие задачи «провайдер даёт публичные IPv6-подсети». Обязанность операторов РБ поддерживать IPv6 введена указом Президента от 18.09.2019 (вступил в силу 01.01.2020) [I1]; доля IPv6-пользователей в РБ по Internet Society Pulse — около 11 % [I2].
- A1/МТС/Деловая сеть: тарифы с их сайтов получить не удалось — отмечено как не подтверждённое; при защите ссылаться только на Белтелеком.

---

## 4. Выводы

### 4.1. Выбор маршрутизатора: MikroTik hEX refresh (E50UG) × 2 (альтернатива — hEX S 2025, если нужен SFP)

Обоснование по совокупности критериев:
1. **Цена.** 244–265 BYN за штуку (16–19 предложений) — одна из двух самых дешёвых позиций списка; дешевле только TP-Link ER605 (285–310) и старый hEX RB750Gr3 (223–250), который по железу вдвое слабее (MIPS 880 МГц, 256 MB) при той же MSRP [M1][M2][S1][S2].
2. **Функции под требования задачи.** Stateful firewall с фильтрацией по протоколу/порту и для IPv4, и для IPv6 (`/ip firewall`, `/ipv6 firewall filter` с `connection-state`) [M9]; DHCPv6-PD-клиент и RA для раздачи провайдерского префикса в VLAN'ы [M10][M11]; 802.1Q на всех портах; IPsec IKEv2 и WireGuard (RouterOS 7) для связи зданий [M7]. У TP-Link ER605 IPv6-функции есть, но DHCPv6-PD-сервер и CLI работают «only in Standalone Mode» [T1], а экспорт конфигурации — бинарный.
3. **Производительность.** IPsec 351,7 Мбит/с (1 туннель, AES-128, 1400 B) и 1430 Мбит/с маршрутизации при 25 правилах фильтра [M2] — с запасом перекрывают и 12 ПК, и отдачу 200–250 Мбит/с бизнес-тарифов Белтелекома [B1]. ER605 — 251 Мбит/с IPsec и 83 Мбит/с WireGuard [T1]. RB5009/RB4011 (1,0–1,35 Гбит/с IPsec) — избыточны при цене 740–990 BYN.
4. **Доступность в РБ.** 16–19 предложений на обоих агрегаторах, крупные магазины держат >10 шт. (stox.by) [S2]; импортёр указан на карточке shop.by.
5. **Надёжность.** Пассивное охлаждение, −40…+70 °C (у TP-Link, Keenetic, Ubiquiti UCG — 0…+40/−10…+40 °C), MTBF ≈ 100 000 ч при 25 °C [M2]; долгосрочная поддержка RouterOS 7 (обновления для всей линейки).
6. **Удобство администрирования и документирования.** Полноценный CLI (SSH), WinBox, WebFig, `/export` даёт текстовую конфигурацию целиком — её можно включить в пояснительную записку. Одинаковая ОС на обоих зданиях упрощает поддержку.

Когда брать **hEX S (2025, E60iUGS)** вместо refresh: если провайдер заводит оптику SFP напрямую или планируется радиомост с питанием PoE-out (у E60iUGS есть PoE-out на Ether5 и SFP 2,5G) — +80 BYN за штуку [M4][O4].

### 4.2. Связь зданий: site-to-site VPN (IPsec IKEv2, резерв — WireGuard) поверх Internet

- По условию у обоих зданий уже есть Gigabit Ethernet от провайдера со статическими IPv4 и IPv6-подсетями, то есть транспорт для туннеля уже оплачен; дополнительная стоимость варианта (а) — 0.
- Радиомост (б) требует прямой видимости на 5 км (мачты так, чтобы над препятствиями в середине трассы оставалось ≥ 5 м), наружного монтажа и грозозащиты, а главное — регистрации РЭС и разрешения БелГИЭ, поскольку наружные антенны и ЭИИМ ≫ 200 мВт под «безразрешительный» перечень не попадают [R2][R3]. Цена пары устройств (525–1340 BYN) сопоставима с ценой пары маршрутизаторов, но не включает мачты и процедуры. Оправдан только там, где нет Интернета в одном из зданий.
- Аренда VPN/канала у оператора (в) — от ~430 руб./мес за 10 Мбит/с и ~1,4 тыс. руб./мес за точку 100 Мбит/с (Белтелеком, без НДС) [B3][B5] — для бюджетной сети неприемлемо.
- Практические параметры туннеля: IKEv2, AES-128/256-GCM или AES-CBC+SHA256, PFS; внутри туннеля — маршрутизация подсетей обоих зданий (IPv4 и IPv6), доступ к файловому серверу по SMB только из VLAN сотрудников через правила `forward` на обоих концах; фильтрация выхода в Интернет «только HTTP/HTTPS» — правилами `/ip firewall filter` и `/ipv6 firewall filter` по `protocol=tcp dst-port=80,443` с `connection-state=established,related` для обратного трафика.

### 4.3. Бюджетная оценка категории

| Статья | Вариант | Сумма, BYN (26.10.2026) |
|---|---|---|
| Маршрутизаторы | 2 × hEX refresh (E50UG) | ≈ 490–530 [S2][O2] |
| Маршрутизаторы (альтернатива) | 2 × hEX S 2025 | ≈ 650 [O4] |
| Канал между зданиями | VPN поверх Internet | 0 |
| Статические IPv4 (Белтелеком) | 2 адреса | 56,00 единовр. + 9,60/мес (без НДС) [B1] |
| IPv6 /56 (Белтелеком) | 2 подсети | 56,80 единовр. + 11,60/мес (без НДС) [B1] |

---

## 5. Источники

MikroTik (mikrotik.com), данные и тесты производителя:
- [M1] hEX RB750Gr3 — https://mikrotik.com/product/RB750Gr3
- [M2] hEX refresh E50UG — https://mikrotik.com/product/hex_2024
- [M3] hEX S RB760iGS — https://mikrotik.com/product/hex_s
- [M4] hEX S (2025) E60iUGS — https://mikrotik.com/product/hex_s_2025
- [M5] RB5009UG+S+IN — https://mikrotik.com/product/rb5009ug_s_in
- [M6] RB4011iGS+RM — https://mikrotik.com/product/rb4011igs_rm
- [M7] WireGuard в RouterOS (документация) — https://help.mikrotik.com/docs/spaces/ROS/pages/69664792/WireGuard
- [M8] Анонс RouterOS v7.1 (WireGuard в списке функций; IPv4 и IPv6) — https://forum.mikrotik.com/t/v7-1-is-released/153754 ; https://administrator.de/knowledge/erster-blick-implementierung-wireguard-routeros-7-1beta2-598512.html
- [M9] IPv6 firewall filter, matcher connection-state — https://manual.mikrotik.com/docs/cli-reference/ipv6/firewall/filter
- [M10] DHCPv6-клиент, request=prefix, pool-name — http://manual.mikrotik.com/docs/cli-reference/ipv6/dhcp-client/
- [M11] IPv6 Neighbor Discovery / RA (RADVD) — https://help.mikrotik.com/docs/spaces/ROS/pages/40992815/IPv6+Neighbor+Discovery
- [M12] LHG 5 ac (discontinued) — https://mikrotik.com/product/lhg_5_ac
- [M13] LHG XL 5 ac — https://mikrotik.com/product/lhg_xl_5_ac
- [M14] SXTsq 5 ac — https://mikrotik.com/product/sxtsq_5_ac
- [M15] Wireless Wire Dish (до 1500 м) — https://mikrotik.com/product/wireless_wire_dish

TP-Link / Omada:
- [T1] ER605(UN) 2.30 Datasheet (08.10.2026) — https://static.tp-link.com/upload/product-overview/2026/202610/20261008/ER605(UN)2.30_Datasheet.pdf
- [T2] ER7206(UN) 2.30 Datasheet (08.10.2026) — https://static.tp-link.com/upload/product-overview/2026/202610/20261008/ER7206(UN)2.30_Datasheet.pdf
- [T3] ER7212PC (V2), страница производителя (End of Life) — https://www.omadanetworks.com/ch/business-networking/omada-router-integrated-router/er7212pc/
- [T4] ER605(UN) v2.20 Datasheet (06.2025, более ранние цифры IPsec/WireGuard) — https://static.tp-link.com/upload/product-overview/2025/202506/20250605/ER605(UN)%20v2.20%20DataSheet.pdf

Keenetic:
- [K1] Keenetic Hero (KN-1012) — характеристики — https://keenetic.com/keenetic-hero
- [K2] Keenetic CLI, сохранение конфигурации — https://help.keenetic.com/hc/articles/213965889 ; https://help.keenetic.com/hc/en-us/articles/360000575200
- [K3] Keenetic Hero 4G+ (KN-2311) — https://keenetic.com/keenetic-hero-4g-plus

Ubiquiti:
- [U1] EdgeRouter X (ER-X), характеристики и цена — https://store.ui.com/us/en/products/er-x
- [U2] UCG-Ultra, цена — https://store.ui.com/us/en/products/ucg-ultra
- [U3] UCG-Ultra, техданные — https://techspecs.ui.com/unifi/cloud-gateways/ucg-ultra
- [U4] Описание EdgeOS (web UI + CLI) в карточке ритейлера — https://galaxus.de/en/s1/product/ubiquiti-edgerouter-x-routers-4678785 (вторичный источник)
- [U5] NanoBeam 5AC Gen2 — https://store.ui.com/collections/operator-airmax-devices/products/nanobeam-5ac-gen2
- [U6] LiteBeam 5AC Gen2 — https://store.ui.com/collections/operator-airmax-devices/products/litebeam-5ac-gen2
- [U7] PowerBeam 5AC Gen2 — https://store.ui.com/collections/operator-airmax-devices/products/powerbeam-5ac-gen2

Цены в РБ — catalog.onliner.by (26.10.2026):
- [O1] hEX RB750Gr3 — https://catalog.onliner.by/dslmodem/mikrotik/rb750gr3
- [O2] hEX refresh — https://catalog.onliner.by/dslmodem/mikrotik/hexrefresh
- [O3] hEX S RB760iGS — https://catalog.onliner.by/dslmodem/mikrotik/rb760igs
- [O4] hEX S (2025) — https://catalog.onliner.by/dslmodem/mikrotik/mrshrtztr2025
- [O5] RB5009UG+S+IN — https://catalog.onliner.by/dslmodem/mikrotik/rb5009ugsin
- [O6] RB4011iGS+RM — https://catalog.onliner.by/dslmodem/mikrotik/rb4011igsrm
- [O7] TP-Link ER605 — https://catalog.onliner.by/dslmodem/tp_link/er605
- [O8] TP-Link TL-ER7206 — https://catalog.onliner.by/dslmodem/tp_link/tler7206
- [O9] TP-Link ER7212PC («Нет в продаже») — https://catalog.onliner.by/dslmodem/tp_link/er7212pc
- [O10] Keenetic Giga KN-1012 — https://catalog.onliner.by/wrouter/keenetic/kn101201ru
- [O11] Keenetic Hero 4G+ KN-2311 (без предложений) — https://catalog.onliner.by/wrouter/keenetic/kn2311
- [O12] Ubiquiti EdgeRouter X — https://catalog.onliner.by/dslmodem/ubiquiti/erx ; ER-X-SFP — https://catalog.onliner.by/dslmodem/ubiquiti/erxsfp
- [O13] Ubiquiti UCG-Ultra — https://catalog.onliner.by/dslmodem/ubiquiti/ucgultra
- [O14] MikroTik LHG 5 ac (без цен) — https://catalog.onliner.by/wirelessap/mikrotik/rblhgg5acd
- [O15] MikroTik LHG XL 5 ac — https://catalog.onliner.by/wirelessap/mikrotik/rblhgg5acdxl
- [O16] MikroTik SXTsq 5 ac — https://catalog.onliner.by/wirelessap/mikrotik/rbsxtsqg5acd ; SXTsq 5 ax — https://catalog.onliner.by/wirelessap/mikrotik/sxtsq5axd
- [O17] Ubiquiti NanoBeam 5AC Gen2 — https://catalog.onliner.by/wirelessap/ubiquiti/nbe5acgen2
- [O18] Ubiquiti LiteBeam 5AC Gen2 — https://catalog.onliner.by/wirelessap/ubiquiti/lbe5acgen2
- [O19] Ubiquiti PowerBeam 5AC Gen2 — https://catalog.onliner.by/wirelessap/ubiquiti/pbe5acgen2
- Диапазоны min–max получены через API каталога: https://catalog.onliner.by/sdapi/catalog.api/search/products?query=<модель>

Цены в РБ — shop.by (26.10.2026):
- [S1] hEX RB750Gr3 — https://shop.by/marshrutizatory/mikrotik_hex_rb750gr3/
- [S2] hEX refresh E50UG — https://shop.by/marshrutizatory/mikrotik_hex_refresh_e50ug/
- [S3] hEX S RB760iGS — https://shop.by/marshrutizatory/mikrotik_hex_s_rb760igs/
- [S4] RB5009UG+S+IN — https://shop.by/marshrutizatory/mikrotik_rb5009ug_s_in/
- [S5] RB4011iGS+RM — https://shop.by/marshrutizatory/mikrotik_rb4011igs_rm/
- [S6] TP-Link ER605 — https://shop.by/marshrutizatory/tp_link_er605/
- [S7] TP-Link TL-ER7206 — https://shop.by/marshrutizatory/tp_link_tl_er7206/
- [S8] Keenetic Giga KN-1012 — https://shop.by/marshrutizatory/keenetic_giga_kn_1012/
- [S9] Ubiquiti EdgeRouter X — https://shop.by/marshrutizatory/ubiquiti_edgerouter_x_er_x/
- [S10] Ubiquiti UCG-Ultra — https://shop.by/marshrutizatory/ubiquiti_unifi_cloud_gateway_ultra/

Провайдеры:
- [B1] Белтелеком, «Постоянный доступ в сеть Интернет» для юрлиц (тарифы с 06.04.2026; статический IPv4, подсети IPv6) — https://www.beltelecom.by/business/internet/high-speed
- [B2] Белтелеком, «Постоянный доступ по выделенным линиям» (помегабитная сетка, порт 1 Гбит/с, IPv4/IPv6) — https://www.beltelecom.by/business/internet/leased-line
- [B3] Белтелеком, «Объединение корпоративных сетей по IP-протоколу» (VPN), тарифы — https://www.beltelecom.by/business/additional-services/business-solutions-networking-vpn
- [B4] Белтелеком, «Программа лояльности VPN» с 01.01.2026 — https://beltelecom.by/en/node/983400/printable/print
- [B5] Белтелеком, «Предоставление в пользование каналов местного уровня» — https://www.beltelecom.by/business/additional-services/business-solutions-networking/lines-channels/provision-local-channels
- [B6] Белтелеком, «Канал доступа» — https://www.beltelecom.by/business/additional-services/business-solutions-networking/lines-channels/wireless-channel-access
- [B7] Белтелеком, пакеты «Бизнес-кейс» — https://www.beltelecom.by/business/pakety-uslug
- [B8] Onliner, «Белтелеком повышает некоторые тарифы с 1 ноября» (19.10.2026) — https://tech.onliner.by/2026/10/19/beltelekom-povyshaet-tarify-s-1-noyabrya ; страница тарифов — https://www.beltelecom.by/tarify-s-1-noyabrya-2026-goda
- [A1] A1, тарифы для юрлиц (PDF; сайт из-за рубежа недоступен) — https://www.a1.by/medias/sys_master/images/hb9/ha1/9292781125662.pdf ; https://internet.a1.by/files/poryadok_deytvuyushchih_tp_yul_0.pdf
- [MT1] Onliner, «МТС запустил тарифы „МТС Драйвер“» (21.07.2026) — https://tech.onliner.by/2026/07/21/mts-zapustil-tarify-mts-drajver
- [I1] Указ об обязательной поддержке IPv6 операторами с 01.01.2020 — https://eufordigital.eu/belarus-becomes-first-country-to-make-ipv6-mandatory-for-isps/ ; https://e-belarus.org/news/201909251.html
- [I2] Internet Society Pulse, Беларусь (доля IPv6) — https://pulse.internetsociety.org/en/reports/by/

Регулирование радиочастот в РБ:
- [R1] Положение о регистрации РЭС и ВЧУ (регистрация в 14 дней; разрешение БелГИЭ) — текст на pravo.by недоступен из-за рубежа; сводка — https://www.mpt.gov.by/ru/o-regulirovanii-i-upravlenii-ispolzovaniem-radiochastotnogo-spektra-v-belarusi
- [R2] Onliner, 26.10.2015 — разъяснение БелГИЭ о постановлениях Минсвязи № 7 (14.06.2013) и № 9 (01.07.2013): без регистрации только абонентская Wi-Fi-аппаратура внутри зданий без наружных антенн — https://tech.onliner.by/2015/10/26/wifi-5
- [R3] БелГИЭ, FAQ (полосы и ЭИИМ для ШБД внутри зданий: 2400–2483,5 МГц до 100 мВт; 5150–5350 и 5650–5725 МГц до 200 мВт) — https://belgie.by/ru/faqs/ (недоступен из-за рубежа, данные по сниппету выдачи)
- Перечень НПА по радиочастотному спектру (Минсвязи) — https://mpt.gov.by/ru/perechen-npa-reguliruyushchih-deyatelnost/radiochastotnyy-spektr ; изменения перечня 2016 г. — https://pravo.by/novosti/novosti-pravo-by/2016/september/21472/

Расчёт трассы:
- [F1] Формула радиуса первой зоны Френеля R = 17,31·√(d1·d2/(f·d)) — методичка СПбГУТ: https://www.sut.ru/doci/umu/disc/5622/03_vlss20mu_bspd_prakt_poteri.pdf
- [F2] Практические пороги перекрытия зоны Френеля (20 % — «полузакрытая», 50 % и более — работа без потерь практически невозможна) — https://habr.com/ru/articles/256299/
