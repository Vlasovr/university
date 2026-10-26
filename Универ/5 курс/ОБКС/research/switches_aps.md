# Исследование рынка РБ: коммутаторы и точки доступа Wi‑Fi

Курсовой проект «Локальная компьютерная сеть» (БГУИР, каф. ЭВМ). Дата сбора цен: **26.10.2026**. Валюта: BYN.

Исходные данные проекта: два одноэтажных здания, бюджетная сеть. Здание 1 — 5 ПК, принтер, точка доступа (10+1 смартфон), NAS, аплинк к маршрутизатору (~9 портов + резерв). Здание 2 — 7 ПК, принтер, точка доступа, аплинк (~10 портов + резерв). Требуются VLAN 802.1Q (access/trunk, PVID), Gigabit Ethernet, желательно PoE на 1–2 портах под точку доступа либо отдельный инжектор. Смартфоны сотрудников — в отдельном VLAN.

## Методика и источники

- Цены снимались с агрегаторов **catalog.onliner.by** (API поиска `catalog.onliner.by/sdapi/catalog.api/search/products`, карточки товаров) и **shop.by** (карточки товаров с перечнем продавцов). Оба агрегатора показывают предложения десятков минских магазинов (100pro.by, vexo.by, e2.by, lix.by, newton.by, marchenko.by, amd.by и др.), поэтому для каждой модели есть цены минимум двух независимых продавцов.
- Сайты 21vek.by (DNS не резолвится из среды), 5element.by (соединение отклонено), ultra.by (HTTP 403) оказались недоступны; kupi.by — магазин бытовой техники, сетевого оборудования в поиске нет; mikrotik.by перенаправляет на форум. tplink.by не является магазином.
- Характеристики взяты с сайтов производителей: tp-link.com / omadanetworks.com (бизнес-раздел TP-Link переехал на omadanetworks.com), dlink.ru и ftp.dlink.ru (официальные datasheet), mikrotik.com, zyxel.com, netgear.com, cudy.com, techspecs.ui.com, keenetic.com.
- «Мин. цена» — минимальное предложение на агрегаторе на 26.10.2026; «макс.» — верхняя граница диапазона (обычно завышенные предложения единичных продавцов). Пометка **«не подтверждено»** — данные найти на сайте производителя не удалось.
- Лимит поисковых запросов в сессии был исчерпан к концу работы, поэтому для пары второстепенных моделей (Zyxel GS1900-10HP, Netgear) вторая розничная ссылка не найдена; эти модели в РБ в продаже отсутствуют, что отмечено в таблицах.

---

## 1. Коммутаторы

### 1.1. Цены и наличие в РБ (26.10.2026)

| Модель | Onliner: мин. цена, предложений, ссылка | Shop.by: диапазон, предложений, самые дешёвые продавцы, ссылка | Наличие |
|---|---|---|---|
| TP-Link TL-SG1016PE | 802,17; 9; https://catalog.onliner.by/switch/tp_link/tlsg1016pe | 604,00–802,17; 8; e2.by 604,00, 100pro.by 604,00, marchenko.by 614,31; https://shop.by/kommutatory/tp_link_tl_sg1016pe/ | в наличии, у части продавцов по 1 шт. |
| TP-Link TL-SG2016P (Omada) | 622,93; 25; https://catalog.onliner.by/switch/tp_link/tlsg2016p | 562,00–1119,30; 22; 100pro.by 562,00, vexo.by 562,30, texnomix.by 569,00; https://shop.by/kommutatory/tp_link_tl_sg2016p/ | в наличии (575.by «больше 10 шт.») |
| TP-Link TL-SG1016DE | 402,35; 16; https://catalog.onliner.by/switch/tp_link/tlsg1016de | 306,60–445,05; 17; e2.by 306,60 (1 шт.), 100pro.by 362,00, vexo.by 362,90; https://shop.by/kommutatory/tp_link_tl_sg1016de/ | в наличии |
| TP-Link TL-SG2210P (Omada) | 515,84; 28; https://catalog.onliner.by/switch/tp_link/tlsg2210p | 467,00–1907,27; 24; 100pro.by 467,00, vexo.by 467,10, lix.by 468,00; https://shop.by/kommutatory/tp_link_tl_sg2210p/ | в наличии (e2.by «больше 10») |
| TP-Link TL-SG108PE | 241,98; 32; https://catalog.onliner.by/switch/tp_link/tlsg108pe | 217,20–735,05; 28; vexo.by 217,20, 100pro.by 221,00, lix.by 221,00; https://shop.by/kommutatory/tp_link_tl_sg108pe/ | в наличии |
| TP-Link TL-SG2008P V3 (Omada) | V3 607,44; 3; https://catalog.onliner.by/switch/tp_link/tlsg2008pv3 (V1: 506,61; 14; https://catalog.onliner.by/switch/tp_link/tlsg2008p) | 453,70–607,40; 4; e2.by 453,70, techshop.by 495,00, tevio.by 607,00; https://shop.by/kommutatory/tp_link_tl_sg2008p_v3/ | мало предложений |
| D-Link DGS-1100-16V2/A2A | 342,75; 19; https://catalog.onliner.by/switch/dlink/dgs110016v2a2a | 309,00–395,50; 19; 100pro.by 309,00, vexo.by 309,40, marchenko.by 323,91; https://shop.by/kommutatory/d_link_dgs_1100_16v2_a2a/ | в наличии |
| D-Link DGS-1210-16/G2A | 683,70; 14; https://catalog.onliner.by/switch/dlink/dgs121016g2a | 603,10–863,70; 15; e2.by 603,10, 100pro.by 624,00, lix.by 624,10; https://shop.by/kommutatory/d_link_dgs_1210_16_g2a/ | в наличии |
| D-Link DGS-1210-10P/F3A | 622,83; 14; https://catalog.onliner.by/switch/dlink/dgs121010pf3a | 597,00–646,70; 11; stelar.by, newton.by, 100pro.by — по 597,00; https://shop.by/kommutatory/d_link_dgs_1210_10p_f3a/ | в наличии, по 1 шт. |
| D-Link DGS-1210-10P/FL2A | 638,78; 18; https://catalog.onliner.by/switch/dlink/dgs121010pfl2a | 557,00–672,44; 17; 100pro.by 557,00, e2.by 557,30, newton 562,62; https://shop.by/kommutatory/d_link_dgs_1210_10p_fl2a/ | в наличии |
| D-Link DGS-1100-10MPV2/A3A | 524,75; 19; https://catalog.onliner.by/switch/dlink/dgs110010mpv2a3a | 449,00–570,70; 20; 100pro.by 449,00, vexo.by 449,20, lix.by 461,49; https://shop.by/kommutatory/d_link_dgs_1100_10mpv2_a3a/ | в наличии |
| D-Link DGS-1100-10MPPV2/A3A (802.3bt, 242 Вт) | 620,57; 30; https://catalog.onliner.by/switch/dlink/dgs110010mppv2a3 | — (не проверялось) | в наличии |
| MikroTik CSS318-16G-2S+IN | **в каталоге Onliner отсутствует** (поиск «CSS318» даёт только CRS318-16P-2S+OUT, 1135,25) | не найден | в РБ не продаётся |
| MikroTik CSS326-24G-2S+RM | 603,33; 31; https://catalog.onliner.by/switch/mikrotik/css32624g2srm | 551,00–2074,00; 28; 100pro.by 551,00, amd.by 551,67, vexo.by 553,30 (1 шт.); https://shop.by/kommutatory/mikrotik_css326_24g_2s_rm/ | в наличии |
| MikroTik CRS310-8G+2S+IN | 1229,74; 2; https://catalog.onliner.by/switch/mikrotik/crs3108g2sin | «товара нет в продаже»; https://shop.by/kommutatory/mikrotik_crs310_8g_2s_in/ | единичные предложения |
| Netgear GS316EP | нет (в каталоге только неуправляемый GS316, «снят с продажи»: https://catalog.onliner.by/switch/netgear/gs316) | нет | в РБ не продаётся |
| Netgear GS308EP | нет (найден только GS308P без предложений: https://catalog.onliner.by/switch/netgear/gs308p) | нет | в РБ не продаётся |
| Zyxel GS1200-8HP v2 | 495,00; 14; https://catalog.onliner.by/switch/zyxel/gs12008hpv2 | 475,00–531,51; 10; newton 475,00, lix.by 475,00, 100pro.by 475,00; https://shop.by/kommutatory/zyxel_gs1200_8hp_v2/ | в наличии |
| Zyxel GS1900-10HP | карточка без предложений: https://catalog.onliner.by/switch/zyxel/gs190010hp | «товара нет в продаже»; https://shop.by/kommutatory/zyxel_gs1900_10hp/ | в РБ не продаётся |
| Cudy GS2010PS2 | **модель не найдена** ни на Onliner, ни у Cudy; ближайший аналог — Cudy GS2008PS2 1.0 (ниже) | — | — |
| Cudy GS2008PS2 1.0 | 553,39; 26; https://catalog.onliner.by/switch/cudycudy/gs2008ps210 | 371,00–710,00; 23; e2.by 371,00, stelar.by 506,10, 100pro.by 511,00; https://shop.by/kommutatory/cudy_gs2008ps2/ | в наличии (lix.by «больше 10») |
| Cudy GS1010PS2 1.0 (неуправляемый, для справки) | 164,26; 15; https://catalog.onliner.by/switch/cudycudy/gs1010ps210 | — | в наличии |

Примечание: на Onliner для TL-SG2016P/TL-SG2210P продаётся как старая серия «TL-SG…», так и переименованная «SG2016P/SG2210P» (TP-Link пометил TL-версии как End of Sale, характеристики одинаковые за исключением рабочей температуры −5…40 °C у новых ревизий).

### 1.2. Характеристики (по данным производителей)

| Параметр | TL-SG1016PE | TL-SG2016P / SG2016P | TL-SG1016DE V4 | TL-SG2210P / SG2210P V5 | TL-SG108PE V5 | TL-SG2008P V3 |
|---|---|---|---|---|---|---|
| Порты GbE / SFP | 16 / 0 | 16 / 0 | 16 / 0 | 8 / 2 SFP | 8 / 0 | 8 / 0 |
| PoE-порты, стандарт | 8, 802.3af/at | 8, 802.3af/at | нет | 8, 802.3af/at | 4, 802.3af/at | 4, 802.3af/at |
| Бюджет PoE | 110 Вт (30 Вт/порт) | 120 Вт (30 Вт/порт) | — | 61 Вт | 64 Вт (V5; в описаниях магазинов для старых ревизий — 55 Вт) | 62 Вт |
| 802.1Q VLAN | да, до 32 активных VLAN (ID из 4K) | да, до 4K групп VLAN; MAC/protocol/voice VLAN, GVRP | да, до 32 активных VLAN | да, 4K групп | да, до 32 | да, 4K групп |
| Управление | Web GUI, Easy Smart Utility (Windows) | Web, CLI (Telnet), SNMP v1/v2c/v3, RMON, Omada-контроллер (ПО бесплатно)/Omada App | Web, Easy Smart Utility | Web, CLI, SNMP v1/v2c/v3, Omada | Web, Easy Smart Utility | Web, CLI, SNMP v1/v2c/v3, SSH, HTTPS, Omada |
| Коммутационная матрица | 32 Гбит/с, 23,8 Mpps | 32 Гбит/с (SG2016P V1.30; на странице старой TL-версии указано 16 Гбит/с) | 32 Гбит/с, 23,8 Mpps | 20 Гбит/с | 16 Гбит/с | 16 Гбит/с |
| Вентилятор | **1 вентилятор** | безвентиляторный | безвентиляторный | безвентиляторный | безвентиляторный | безвентиляторный |
| Рабочая температура | 0…40 °C | 0…40 °C (SG2016P: −5…40 °C) | 0…40 °C | 0…40 °C (V5.30: −5…40 °C) | 0…40 °C | 0…40 °C |
| Крепление в 19" стойку | да, 1U, комплект в коробке | нет (стол/стена), 286×111,7×25,4 мм | да, комплект в коробке, 294×180×44 мм | нет (стол/стена), 209×126×26 мм | нет (стол/стена) | нет |
| Потребление | 14,7 Вт без PoE, 131,4 Вт с нагрузкой | — | 10,22 Вт макс. | — | — | 77,3 Вт макс. |
| Источник | https://www.tp-link.com/us/business-networking/easy-smart-switch/tl-sg1016pe/ | https://www.omadanetworks.com/us/business-networking/omada-switch-smart/tl-sg2016p/ ; https://www.omadanetworks.com/us/business-networking/omada-switch-smart/sg2016p/ | https://www.tp-link.com/us/business-networking/easy-smart-switch/tl-sg1016de/ | https://www.omadanetworks.com/us/business-networking/omada-switch-smart/tl-sg2210p/ ; …/sg2210p/ | https://www.tp-link.com/us/business-networking/easy-smart-switch/tl-sg108pe/ | https://www.omadanetworks.com/us/business-networking/omada-switch-smart/tl-sg2008p/ |

| Параметр | D-Link DGS-1100-16V2/A2A | D-Link DGS-1210-16/G2A | D-Link DGS-1210-10P/F3A (FL2A — аналог) | D-Link DGS-1100-10MPV2/A3A |
|---|---|---|---|---|
| Порты GbE / SFP | 16 / 0 | 16 + 4 комбо 100/1000Base-T/SFP | 8 / 2 SFP 1000Base-X | 8 / 2 SFP 100/1000 |
| PoE-порты, стандарт | нет | нет | 8, 802.3af/at | 8, 802.3af/at |
| Бюджет PoE | — | — | 65 Вт (30 Вт/порт) | 130 Вт (30 Вт/порт) |
| 802.1Q VLAN | да, до 128 статических групп; Surveillance/Voice/Management/Asymmetric VLAN | да, до 256 статических групп | да, до 256 статических групп | да, до 128 групп, VID 1–4094 |
| Управление | Web (IPv4/IPv6), SNMP v1/v2c, D-View 8, Nuclias; CLI нет | Web, упрощённый CLI через Telnet, SNMP, SSH v2 | Web, CLI через Telnet, SNMP, SSH | Web, SNMP v1/v2c, D-View 8, Nuclias |
| Коммутационная матрица | 32 Гбит/с, 23,81 Mpps | 40 Гбит/с, 29,8 Mpps | 20 Гбит/с, 14,88 Mpps | 20 Гбит/с |
| Вентилятор | пассивное охлаждение (по описанию серии и карточкам магазинов; в datasheet строка не найдена — **не подтверждено**) | пассивное, 0 дБ | пассивное, 0 дБ (datasheet F3) | **1 smart-вентилятор**, 36,2–37 дБ |
| Рабочая температура | −5…50 °C | −5…50 °C | −5…50 °C | −5…50 °C |
| Крепление в 19" стойку | да, 2 кронштейна в комплекте; 280×180×44 мм, 1,21 кг | да, 2 кронштейна; 280×180×44 мм, 1,75 кг | да, 2 кронштейна; 280×126×44 мм, 0,95 кг | да, 1U, 2 кронштейна; 280×180×44 мм |
| Потребление | — (в datasheet не извлечено) | 13,08 Вт макс. | 80,56 Вт макс. | 164,6 Вт с PoE, 11,7 Вт без PoE |
| Источник | https://ftp.dlink.ru/pub/Switch/DGS-1100-16V2/Data_sh/DS_DGS-1100-16V2_A1_RUS.pdf | https://ftp.dlink.ru/pub/Switch/DGS-1210-16/Data_sh/DS_DGS-1210-16_E_G2_RUS.pdf | https://ftp.dlink.ru/pub/Switch/DGS-1210-10P/Data_sh/DS_DGS-1210-10P_F3_RUS.pdf | https://dlink.ru/ru/products/1/2666.html |

| Параметр | MikroTik CSS318-16G-2S+IN | MikroTik CSS326-24G-2S+RM | MikroTik CRS310-8G+2S+IN | Netgear GS316EP | Netgear GS308EP | Zyxel GS1200-8HP v2 | Zyxel GS1900-10HP | Cudy GS2008PS2 |
|---|---|---|---|---|---|---|---|---|
| Порты GbE / SFP | 16 / 2 SFP+ | 24 / 2 SFP+ | 8×2,5GbE / 2 SFP+ | 15 PoE+ + 1 RJ45 / 1 SFP | 8 / 0 | 8 / 0 | 8 / 2 SFP | 8 / 2 SFP |
| PoE-порты, стандарт | нет (PoE-in пассивное) | нет (PoE-in 10–30 В пассивное) | нет PoE-out | 15, 802.3at | 8, 802.3at | 4, 802.3at | 8, 802.3af/at (**не подтверждено**, данные ритейлеров) | 8, 802.3af/at |
| Бюджет PoE | — | — | — | 180 Вт | 62 Вт | 60 Вт (30 Вт/порт) | 77 Вт (**не подтверждено**) | производитель не указывает бюджет: 30 Вт/порт, БП 120/130 Вт; Onliner — 118 Вт |
| 802.1Q VLAN | да (SwOS) | да (SwOS) | да (RouterOS/SwOS) | да (port/802.1Q) | **не указано** на странице | да | да (**не подтверждено**) | да, 4096 VLAN |
| Управление | SwOS (web) | SwOS (web) | RouterOS v7 (web/CLI/SNMP) или SwOS | web «Easy Smart Managed» | web (**не указано**) | только Web GUI | web/CLI/SNMP (**не подтверждено**) | Web, SNMP v1/v2/v3, Telnet, SSH, RMON |
| Матрица | 72 Гбит/с (L1) | 88 Гбит/с (L1) | 80 Гбит/с (L1) | 32 Гбит/с | 16 Гбит/с | 16 Гбит/с | — | 20 Гбит/с |
| Вентилятор | нет | нет | **1 вентилятор** | нет | нет | нет | — | «silent cooling» (**неясно**) |
| Рабочая температура | −40…70 °C | −20…70 °C | −40…60 °C | не указана | не указана | 0…40 °C | — | −10…50 °C |
| 19" стойка | да, уши в комплекте (2 шт. в 1U) | да, 1U, 443×144×44 мм | да, уши в комплекте | нет (стол/стена) | стена/стойка (крепёж) | нет, 240×105×26 мм | — | да, кронштейны; 268×181×44 мм |
| Потребление / цена производителя | 13 Вт; $139 | 19 Вт; $159 | 34 Вт; $219 | — | — | 72 Вт макс. | — | 12 Вт без PoE |
| Источник | https://mikrotik.com/product/css318_16g_2s_in | https://mikrotik.com/product/CSS326-24G-2SplusRM | https://mikrotik.com/product/crs310_8g_2s_in | https://www.netgear.com/business/wired/switches/plus/gs316ep/ | https://www.netgear.com/business/wired/switches/plus/gs308ep/ | https://www.zyxel.com/us/en-us/products/switch/gs1200-poe-series/specifications | ритейлеры (shi.com, directcomputers.co.uk) | https://www.cudy.com/products/gs2008ps2-1-0 |

Замечания к кандидатам:
- **TL-SG1016PE** — единственный 16-портовый PoE-кандидат с вентилятором; для лаборатории/кабинета это минус. На shop.by он дешевле, чем TL-SG2016P только у одного продавца (604 против 562), а функционально проигрывает (32 VLAN против 4K, нет CLI/SNMP/контроллера).
- **TL-SG2210P / DGS-1210-10P / Cudy GS2008PS2 / DGS-1100-10MPV2** — формат 8 RJ45 + 2 SFP. Для здания 2 нужно 10 медных портов (7 ПК + принтер + AP + аплинк), то есть резерва нет вообще, а аплинк пришлось бы вести через SFP-модуль (доп. расходы и медиаконвертер/SFP на стороне маршрутизатора). Для здания 1 (9 портов) резерв — 1 порт. **Вывод: 8+2 недостаточно, нужны 16-портовые модели.**
- **TL-SG108PE, TL-SG2008P, GS1200-8HP v2, GS308EP** — 8 портов, не проходят по числу портов даже для здания 1.
- **CSS326-24G-2S+RM** — 24 порта за 551 BYN, безвентиляторный, но без PoE и без SNMP/CLI (SwOS только web). Избыточен по портам; плюс — 2 SFP+ и 19" корпус.
- **CRS310-8G+2S+IN** — 2,5GbE, с вентилятором, 1230 BYN — избыточен для бюджетной сети.
- **DGS-1210-16/G2A** — лучший «полноценный» L2 среди 16-портовых без PoE: 256 VLAN, CLI, SSH, SNMP, 40 Гбит/с, пассивное охлаждение, 19" стойка, но 603 BYN.
- **DGS-1100-16V2/A2A** — самый дешёвый управляемый 16-портовый: 802.1Q (128 групп), SNMP, 19" кронштейны, −5…50 °C — за 309 BYN.
- Netgear GS316EP/GS308EP и Zyxel GS1900-10HP на рынке РБ не представлены (ни одного предложения на Onliner и shop.by) — из выбора исключены.
- Cudy GS2010PS2 не существует (ни у производителя, ни у продавцов); рассмотрен Cudy GS2008PS2.

---

## 2. Точки доступа

### 2.1. Цены и наличие в РБ (26.10.2026)

| Модель | Onliner: мин. цена, предложений, ссылка | Shop.by: диапазон, предложений, самые дешёвые продавцы, ссылка | Наличие |
|---|---|---|---|
| TP-Link EAP225 V5 | 235,00; 42; https://catalog.onliner.by/wirelessap/tp_link/eap225v5 (V3: 238,00; 9; …/eap225) | 232,90–278,15; 9; lix.by 232,90, gigamarket.by 233,74, marchenko.by 237,33; https://shop.by/tochki_dostupa/tp_link_eap225/ | в наличии |
| TP-Link EAP110 | 129,96; 34; https://catalog.onliner.by/wirelessap/tp_link/eap110 | 122,00–482,00; 22; newton 122,00, vexo.by 122,00 (1 шт.), mmgby 128,65; https://shop.by/tochki_dostupa/tp_link_eap110/ | в наличии (модель End of Life у TP-Link) |
| TP-Link EAP610 | 379,38; 37; https://catalog.onliner.by/wirelessap/tp_link/tpleap610 | 353,00–637,67; 29; stelar.by 353,00, 100pro.by 353,00, mdx.by 353,79; https://shop.by/tochki_dostupa/tp_link_eap610/ | в наличии |
| TP-Link EAP613 | 330,39; 39; https://catalog.onliner.by/wirelessap/tp_link/eap613 | 303,40–568,70; 28; vexo.by 303,40, lix.by 308,00, 100pro.by 308,00; https://shop.by/tochki_dostupa/tp_link_eap613/ | в наличии |
| TP-Link EAP615-Wall | карточка без цены: https://catalog.onliner.by/wirelessap/tp_link/eap615wall | 660,60; 1; e2.by; https://shop.by/tochki_dostupa/tp_link_eap615_wall/ | единичное предложение, дорого |
| TP-Link EAP650 | 558,85; 10; https://catalog.onliner.by/wirelessap/tp_link/eap650 | 512,00–871,76; 13; 100pro.by 512,00, mmgby 553,27, fastshop 558,85 (1 шт.); https://shop.by/tochki_dostupa/tp_link_eap650/ | в наличии |
| TP-Link EAP653 | 387,63; 45; https://catalog.onliner.by/wirelessap/tp_link/eap653 | 343,00–482,00; 30; lix.by 343,00, 100pro.by 343,00, mdx.by 343,80 (1 шт.); https://shop.by/tochki_dostupa/tp_link_eap653/ | в наличии |
| MikroTik cAP ac | 304,55; 39; https://catalog.onliner.by/wirelessap/mikrotik/rbcapgi5acd2nd | 276,00–524,30; 30; 100pro.by 276,00, newton 276,70, vexo.by 276,70; https://shop.by/tochki_dostupa/mikrotik_cap_ac/ | в наличии |
| MikroTik cAP XL ac | 330,24; 41; https://catalog.onliner.by/wirelessap/mikrotik/rbcapgi5acd2ndxl | 300,00–575,50; 30; 100pro.by 300,00 (1 шт.), vexo.by 300,50 (1 шт.), newton 304,77; https://shop.by/tochki_dostupa/mikrotik_cap_xl_ac/ | в наличии |
| MikroTik cAP ax | 568,34; 32; https://catalog.onliner.by/wirelessap/mikrotik/capgi5haxd2haxd | 340,00–858,42; 34; newton 340,00 (на странице смешаны предложения hAP ax S — цену проверить), fastshop 397,61, 9watt.by 497,40; https://shop.by/tochki_dostupa/mikrotik_cap_ax/ | в наличии |
| MikroTik wAP ac | карточки без предложений (RBwAPG-5HacD2HnD и -BE): https://catalog.onliner.by/wirelessap/mikrotik/rbwapg5hacd2hnd | — | снят с производства (mikrotik.com: discontinued), в РБ нет |
| MikroTik hAP ax lite L41G-2axD | 235,29; 39; https://catalog.onliner.by/wrouter/mikrotik/l41g2axd | 208,50–718,26; 29; mdx.by 208,50, vexo.by 211,20, lix.by 211,51; https://shop.by/marshrutizatory/mikrotik_hap_ax_lite_l41g_2axd/ | в наличии (это роутер, см. ниже) |
| Ubiquiti U6+ | 470,00; 33; https://catalog.onliner.by/wirelessap/ubiquiti/u6plus | страница на shop.by не найдена; цена производителя в ЕС 89 € (https://eu.store.ui.com/eu/en/products/u6-plus) | в наличии на Onliner |
| Ubiquiti U6 Lite | карточка без предложений: https://catalog.onliner.by/wirelessap/ubiquiti/ubiqu6lite | — | в РБ нет |
| Ubiquiti UAP-AC-Lite | карточка без предложений: https://catalog.onliner.by/wirelessap/ubiquiti/uapaclite | — | в РБ нет |
| Cudy AP1300 1.0 | 148,90; 33; https://catalog.onliner.by/wirelessap/cudycudy/cudyap130010 | 98,00–356,00; 45; techshop.by 98,00, newton 136,56, mmgby 147,41; https://shop.by/tochki_dostupa/cudy_ap1300_1_0/ | в наличии |
| Cudy AP3000 1.0 | 298,26; 27; https://catalog.onliner.by/wirelessap/cudycudy/cudyap300010 | 296,25 (newton), 298,26 (575.by, 1 шт.), 298,80 (9watt.by); 48 предложений (включая варианты Wall/Outdoor/_P); https://shop.by/tochki_dostupa/cudy_ap3000_1_0/ | в наличии |
| Zyxel NWA50AX | 693,26; 3; https://catalog.onliner.by/wirelessap/zyxel/nwa50axeu0102f | 597,20–693,26; 7; e2.by 597,20, 9watt.by 607,00, 100pro.by 653,00; https://shop.by/tochki_dostupa/zyxel_nwa50ax/ | в наличии |
| Zyxel NWA1123ACv3 | «нет в продаже»: https://catalog.onliner.by/wirelessap/zyxel/nwa1123acv3 | — | в РБ нет |
| Keenetic Voyager Pro KN-3510 | «нет в продаже»: https://catalog.onliner.by/wrouter/keenetic/voyagerprokn3510 | категория shop.by без карточки | в РБ нет (на рынке РБ/РФ бренд Keenetic заменён на Netcraze; прямого аналога Voyager Pro в каталоге нет) |

Для анализа PoE-питания также сняты цены инжекторов (Onliner, 26.10.2026):
- TP-Link TL-POE150S (802.3af, 15,4 Вт): от 83,68 BYN, 15 предложений — https://catalog.onliner.by/cable/tp_link/tlpoe150s
- TP-Link TL-POE160S (802.3af/at, 30 Вт): от 101,95 BYN, 18 — https://catalog.onliner.by/cable/tp_link/tlpoe160s
- Cudy POE200 (802.3af/at, 30 Вт, GbE): от 44,13 BYN, 25 — https://catalog.onliner.by/cable/cudycudy/cudypoe200
- MikroTik RBGPOE (пассивный GbE-инжектор): от 23,43 BYN, 13 — https://catalog.onliner.by/cable/mikrotik/rbgpoe
- Ubiquiti POE-24-12W (пассивный 24 В): от 43,53 BYN, 21 — https://catalog.onliner.by/cable/ubiquiti/poe2412w

### 2.2. Характеристики (по данным производителей)

| Параметр | EAP225 (V3 стр.) | EAP110 | EAP610 V3 | EAP613 | EAP615-Wall | EAP650 V3 | EAP653 V3 |
|---|---|---|---|---|---|---|---|
| Стандарт | Wi‑Fi 5 (802.11ac Wave 2) | Wi‑Fi 4 (802.11n), только 2,4 ГГц | Wi‑Fi 6 | Wi‑Fi 6 | Wi‑Fi 6 | Wi‑Fi 6 | Wi‑Fi 6 |
| Потоки | 2×2 (по данным Onliner для V5; на стр. не указано) | 2×2 (**не указано**) | 2×2 | 2×2 | **не указано** | **не указано** (MU‑MIMO) | **не указано** |
| Скорости 2,4 / 5 ГГц | V3: 450 / 867; V5 (Onliner): 300 / 867 | 300 / — | 574 / 1201 | 574 / 1201 | 574 / 1201 | 574 / 2402 | 574 / 2402 |
| Антенны, дБи | 4 (2,4) / 5 (5) | 2×4 | 2×4 / 2×5 | 2×4 / 2×5 | 2×3 / 2×4 | 2×4 / 2×5 | 2×4 / 2×5 |
| Макс. мощность, дБм | CE: ≤20 (2,4) / ≤23 (5) EIRP | 21 | CE: <20 / <23 (<25 band 3) EIRP | CE: <20 / <23 EIRP | CE: 7–24 EIRP | 26 / 24–25 | 4–25 |
| Рекоменд. число клиентов | не указано | 100+ | 250+ | 250+ (115 м²) | 120+ | 250+ | 250+ |
| Multi‑SSID + VLAN | 16 SSID (8/диапазон), SSID‑to‑VLAN, Management VLAN | 8 SSID, SSID‑to‑VLAN | 16 SSID, SSID‑to‑VLAN | 16 SSID, SSID‑to‑VLAN | 16 SSID, VLAN | 16 SSID, SSID‑to‑VLAN | 16 SSID, SSID‑to‑VLAN |
| WPA3 | на странице не указано (**не подтверждено**) | нет (WPA2) | да | да | да | да | да |
| PoE | 802.3af/at **или** пассивный 24 В | только пассивный 24 В | 802.3at (PoE+) или пассивный 48 В | 802.3at или пассивный 48 В | 802.3af/at; есть PoE‑passthrough 13 Вт (нужен 802.3at на входе) | 802.3at или пассивный 48 В | 802.3at или пассивный 48 В |
| Инжектор/БП в комплекте | **да, пассивный PoE‑адаптер** | да, пассивный PoE‑адаптер | PoE‑инжектора нет; БП 12 В/1 А есть | **ничего** (ни инжектора, ни БП) | нет | PoE‑инжектора нет; БП 12 В/1,5 А есть | **ничего** |
| Управление | Omada (SW‑контроллер бесплатный, OC200/300, облако), App, web, SSH | Omada, web, SSH, Telnet, SNMP | Omada, web, SSH, SNMP v1/v2c/v3 | Omada, web, SSH, SNMP | Omada, web, SSH, SNMP | Omada, web, SSH, SNMP | Omada, web, SSH, SNMP |
| Рабочая температура | 0…40 °C | 0…40 °C | 0…40 °C | 0…40 °C | 0…40 °C | 0…40 °C | 0…40 °C |
| Монтаж | потолок/стена (комплект) | потолок/стена | потолок/стена/коробка | потолок/стена | в подрозетник 86 мм | потолок/стена/T‑bar | потолок/стена/T‑bar |
| Источник | https://www.omadanetworks.com/us/business-networking/omada-wifi-ceiling-mount/eap225/ | …/eap110/ | …/eap610/ | …/eap613/ | https://www.omadanetworks.com/us/business-networking/omada-wifi-wall-plate/eap615-wall/ | …/eap650/ | …/eap653/ |

| Параметр | MikroTik cAP ac | cAP XL ac | cAP ax | wAP ac | hAP ax lite |
|---|---|---|---|---|---|
| Стандарт | Wi‑Fi 5, AC1200 | Wi‑Fi 5, AC1200 | Wi‑Fi 6, AX1800 | Wi‑Fi 5, AC1200 | Wi‑Fi 6, AX600, **только 2,4 ГГц** |
| Потоки | 2×2 | 2×2 | 2×2 | 2×2 | 2×2 |
| Скорости 2,4 / 5 ГГц | 300 / 867 | 300 / 867 | 574 / 1200 | 300 / 867 | 547 / — |
| Антенны, дБи | 2 / 2,5 | 6 / 5,5 | 6 / 5,5 | 2,5 / 2,5 | 4,3 |
| Макс. мощность, дБм | 26 (по таблице скоростей) | 26 | 24 | 25 / 24 | 22 |
| Рекоменд. число клиентов | не указано | не указано | не указано | не указано | не указано |
| Multi‑SSID + VLAN | Virtual AP + VLAN через RouterOS/CAPsMAN (datapath vlan‑id на каждый SSID) | то же | то же (CAPsMAN для пакета wifi, RouterOS v7) | то же | то же |
| WPA3 | RouterOS (на стр. не указано) | не указано | да (WPA3‑PSK/EAP, OWE, 802.11r) | не указано | не указано |
| PoE | 802.3af/at in + пассивный PoE‑out | 802.3af/at in + PoE‑out | 802.3af/at in + PoE‑out 19,2 Вт | 802.3af/at in | нет PoE (USB‑C 5 В) |
| Инжектор в комплекте | **да, Gigabit PoE‑инжектор + БП 24 В** | **да** | **да, инжектор + БП 48 В** | да | нет (USB БП) |
| Управление | RouterOS L4, CAPsMAN | RouterOS, CAPsMAN | RouterOS v7, CAPsMAN | RouterOS | RouterOS v7 |
| Рабочая температура | −40…50 °C | −40…70 °C | −40…70 °C | −40…70 °C, IP54 | −40…70 °C |
| Цена производителя | $85 | $99 | $129 | $89 (discontinued) | $59 |
| Источник | https://mikrotik.com/product/cap_ac | https://mikrotik.com/product/cap_xl_ac | https://mikrotik.com/product/cap_ax | https://mikrotik.com/product/wap_ac | https://mikrotik.com/product/hap_ax_lite |

CAPsMAN: по документации MikroTik (https://help.mikrotik.com/docs/spaces/ROS/pages/1409149/CAPsMAN) CAP требует лицензию уровня ≥4 (есть у всех cAP), контроллер может работать на любом устройстве RouterOS, каждому Virtual‑AP (SSID) назначается свой datapath vlan‑id. На Onliner в отзывах отмечается, что cAP ax работает только с CAPsMAN нового пакета wifi (RouterOS v7), с «классическим» CAPsMAN несовместим.

| Параметр | Ubiquiti U6+ | U6 Lite | UAP‑AC‑Lite | Cudy AP1300 | Cudy AP3000 | Zyxel NWA50AX | Zyxel NWA1123ACv3 | Keenetic Voyager Pro |
|---|---|---|---|---|---|---|---|---|
| Стандарт | Wi‑Fi 6 | Wi‑Fi 6 | Wi‑Fi 5 | Wi‑Fi 5 | Wi‑Fi 6, порт 2,5GbE | Wi‑Fi 6, AX1800 | Wi‑Fi 5 Wave 2 | Wi‑Fi 6, AX1800 |
| Потоки | 2×2 + 2×2 | 2×2 + 2×2 | 2×2 | 2×2 | 2×2 (5 ГГц 3T3R) | 2×2 + 2×2 | 2×2 | 2×2 |
| Скорости 2,4 / 5 ГГц | 573,5 / 2400 (160 МГц) | 300 / 1200 | 300 / 867 | 300 / 867 | 574 / 2402 | 575 / 1200 | 300 / 866 | 574 / 1201 |
| Антенны, дБи | 3 / 5,4 | 2,8 / 3 | 3 / 3 | не указано (Onliner: 5,31 / 4,64) | не указано (Onliner: 4,8 / 5,6) | 3 / 4 | 3 / 4 (Onliner) | не указано (Onliner: 4) |
| Макс. мощность, дБм | 23 / 23 | 23 / 23 | 20 / 20 | не указано | не указано | EU: 20 / 25 (conducted) | EIRP 19 / 23 (Onliner) | не указано |
| Число клиентов | 300+ | 300+ | 250+ | рекоменд. 100 (макс. 128) | рекоменд. 150 (макс. 256) | не указано | не указано | не указано |
| Multi‑SSID + VLAN | 8 BSSID/радио, VLAN (в т.ч. RADIUS) | 8 BSSID/радио | 8 BSSID/радио | 8 SSID (4/диапазон), VLAN | 8 SSID, VLAN | VLAN: да, число SSID не указано | multi‑SSID + 802.1Q (Onliner) | до 7 сетей, 802.1Q |
| WPA3 | на techspecs не указано (Onliner: WPA3‑PSK) | не указано | нет | да | да | да (Personal) | да (Onliner) | да |
| PoE | «PoE», 44–57 В, 9 Вт (стандарт 802.3af явно **не указан**) | PoE 44–57 В, 12 Вт | PoE (пассивный 24 В — **не подтверждено** на стр.) | 802.3af/at + пассивный 48 В | 802.3af/at + 48–57 В пассивный | 802.3at, 16 Вт | 802.3af | 802.3at, 18 Вт |
| Инжектор в комплекте | **нет** (PoE‑адаптер продаётся отдельно, 7 €/8 $) — состав коробки на сайте не читается, **не подтверждено** | не указано | **да** | нет (есть у AP1300_P) | нет (есть у AP3000_P) | нет, в коробке БП 12 В | — | нет, БП 12 В |
| Управление | UniFi Network (self‑hosted бесплатно) | UniFi | UniFi | Cudy AP Controller | Cudy AP Controller / C200P | Nebula (бесплатный тариф) или standalone, web/CLI | Nebula/standalone | web, приложение, CLI, контроллер — любой роутер Keenetic |
| Рабочая температура | −30…60 °C | −30…60 °C | −10…70 °C | −10…65 °C | −10…60 °C | 0…50 °C | — | 0…40 °C |
| Источник | https://techspecs.ui.com/unifi/wifi/u6-plus | https://techspecs.ui.com/unifi/wifi/u6-lite | https://techspecs.ui.com/unifi/wifi/uap-ac-lite | https://www.cudy.com/products/ap1300-1-0 | https://www.cudy.com/products/ap3000-1-0 | https://www.zyxel.com/global/en/products/wireless/nwa50ax/specifications | карточка Onliner (сайт Zyxel не загружен) | https://keenetic.com/en/keenetic-voyager-pro |

Замечания к кандидатам:
- **hAP ax lite** — это роутер без PoE и без 5 ГГц; как потолочная AP не подходит.
- **EAP110** — только 2,4 ГГц, WPA2, пассивное 24 В PoE, модель End of Life. Для 11 смартфонов сработает, но закладывать в проект устаревшую модель не стоит.
- **EAP615-Wall** — настенная «в подрозетник», в РБ один продавец по 660 BYN — дороже потолочных Wi‑Fi 6 моделей.
- **U6 Lite, UAP‑AC‑Lite, NWA1123ACv3, Keenetic Voyager Pro, wAP ac** — в РБ не продаются, исключены.
- Для бюджета критичен состав комплекта: у TP‑Link **EAP225/EAP110 инжектор в коробке**, у **EAP613/EAP653 нет даже блока питания**, у EAP610/EAP650 есть БП, но не инжектор. У MikroTik cAP ac/XL ac/ax **инжектор и БП в комплекте**. Ubiquiti, Cudy (базовые версии), Zyxel — без инжектора.

---

## 3. Выводы

### 3.1. Нужен ли 16‑портовый коммутатор или хватит 8+2

Здание 2 требует 10 медных портов (7 ПК + принтер + AP + аплинк), здание 1 — 9. У моделей 8 RJ45 + 2 SFP (TL‑SG2210P, DGS‑1210‑10P, DGS‑1100‑10MPV2, Cudy GS2008PS2) медных портов 8: во втором здании не хватает двух, в первом — одного; задействовать SFP‑слоты можно только с SFP‑модулями/медиаконвертером, что дороже и сложнее. 8‑портовые модели (TL‑SG108PE, TL‑SG2008P, GS1200‑8HP, GS308EP) не проходят вовсе. **Для обоих зданий нужны 16‑портовые коммутаторы (резерв 6–7 портов).**

### 3.2. PoE‑коммутатор или обычный коммутатор + инжектор

Расчёт на одно здание по минимальным ценам shop.by (26.10.2026):

| Вариант | Коммутатор | Питание AP | Итого, BYN |
|---|---|---|---|
| Без PoE + инжектор в комплекте AP | DGS‑1100‑16V2 309,00 или TL‑SG1016DE 306,60 | EAP225 / cAP: инжектор в коробке, 0 | 307–309 |
| Без PoE + отдельный инжектор 802.3at | DGS‑1100‑16V2 309,00 | Cudy POE200 44,13 (TP‑Link TL‑POE160S 101,95) | 353 (411) |
| PoE‑коммутатор Omada | TL‑SG2016P 562,00 (8×PoE+, 120 Вт) | 0 | 562 |
| PoE‑коммутатор Easy Smart | TL‑SG1016PE 604,00 (вентилятор) | 0 | 604 |

Разница между PoE‑коммутатором и «обычный + инжектор» — **около 210–255 BYN на здание (420–510 BYN на проект)** при одной точке доступа в здании. Если точка доступа идёт с инжектором в комплекте (EAP225, cAP ac/ax), экономия ещё больше. Что покупается за эту доплату: централизованное питание (одна линия ИБП питает и коммутатор, и AP), управление PoE с удалённой перезагрузкой AP, 8 свободных PoE‑портов под IP‑камеры/IP‑телефоны/вторую AP, а у TL‑SG2016P — полноценный L2 (4K VLAN, CLI, SNMP v3, ACL, 802.1X) и управление из того же Omada‑контроллера, что и точки доступа.

**Вывод:** для сети с одной AP на здание экономически выгоднее обычный управляемый коммутатор + инжектор (а лучше — AP с инжектором в комплекте). PoE‑коммутатор оправдан, если в обосновании проекта предусмотрено развитие (камеры, IP‑телефония, вторая AP) или ставится задача единого управления сетью.

### 3.3. Эшелоны и рекомендация

**Коммутаторы (2 шт.)**

| Эшелон | Модель | Мин. цена за шт., BYN | Почему |
|---|---|---|---|
| Самый дешёвый | D‑Link DGS‑1100‑16V2/A2A | 309,00 (shop.by) / 342,75 (Onliner) | 16 GbE, 802.1Q до 128 VLAN, web + SNMP, пассивное охлаждение, кронштейны 19" в комплекте, −5…50 °C. При той же цене, что TL‑SG1016DE, даёт SNMP и 128 VLAN вместо 32 |
| Оптимальный | TP‑Link TL‑SG2016P / SG2016P (Omada) | 562,00 (shop.by) / 622,93 (Onliner) | 16 GbE, 8×PoE+ 120 Вт, 4K VLAN, web/CLI/SNMP/Omada, безвентиляторный, 25 предложений на Onliner. Снимает вопрос питания AP и объединяет управление с точками Omada |
| Избыточный | D‑Link DGS‑1210‑16/G2A (603,10) или MikroTik CSS326‑24G‑2S+RM (551,00) | 551–604 | 20–26 портов, SFP/SFP+, 40–88 Гбит/с, стоечное исполнение. Для 9–10 узлов портов и матрицы больше, чем нужно; без PoE |

**Точки доступа (2 шт.)**

| Эшелон | Модель | Мин. цена за шт., BYN | Почему |
|---|---|---|---|
| Самый дешёвый | TP‑Link EAP225 V5 | 232,90 (shop.by) / 235,00 (Onliner) | Wi‑Fi 5 2×2, 16 SSID с VLAN, 802.3af/at + пассивный PoE, **инжектор в комплекте**, бесплатный Omada Software Controller. Для 11 смартфонов на 20–40 м² достаточно. Минус: WPA3 на странице производителя не подтверждён, Wi‑Fi 5 |
| Оптимальный | TP‑Link EAP613 | 303,40 (shop.by) / 330,39 (Onliner) | Wi‑Fi 6 2×2 (574+1201), 16 SSID + VLAN, WPA3, 802.3at, Omada; 39 предложений на Onliner. В комплекте нет БП и инжектора — питать только от PoE‑коммутатора (TL‑SG2016P) или докупить Cudy POE200 (44 BYN). Альтернатива с БП в коробке — EAP610 (353 BYN) |
| Избыточный | TP‑Link EAP653 (343,00) / EAP650 (512,00); Ubiquiti U6+ (470,00); MikroTik cAP ax (568,34) | 343–568 | AX3000 (2402 Мбит/с в 5 ГГц) или 160 МГц‑каналы, 250–300+ клиентов. Для помещений 10–40 м² и 11 клиентов пропускная способность не будет востребована |

**Итоговая рекомендация для бюджетной сети (по совокупности критериев):**

1. Вариант «минимальный бюджет»: 2 × DGS‑1100‑16V2/A2A (309 BYN) + 2 × EAP225 V5 (232,90 BYN, инжектор в коробке) ≈ **1084 BYN** на активное оборудование. Все требования (802.1Q access/trunk/PVID, Gigabit, отдельный VLAN для смартфонов, PoE через инжектор) выполнены.
2. Вариант «оптимальный» (рекомендуемый для защиты как обоснованный): 2 × TL‑SG2016P (562 BYN) + 2 × EAP613 (303,40 BYN) ≈ **1731 BYN**. Единое управление коммутаторами и точками через бесплатный Omada Software Controller, PoE‑питание AP от коммутатора (резерв 7 PoE‑портов), Wi‑Fi 6 с WPA3, безвентиляторное исполнение (тихо в кабинетах и лаборатории), 4K VLAN, SNMP v3 для мониторинга. Доплата ≈ 650 BYN относительно минимального варианта покупает расширяемость и управляемость, а не только скорость.
3. Компромисс: TL‑SG2016P + EAP225 (≈ 1590 BYN) — если Wi‑Fi 6 признан избыточным, но единое Omada‑управление и PoE от коммутатора нужны.

MikroTik (cAP ac 276 BYN + CSS326/любой коммутатор) — хорошая альтернатива по цене и температурному диапазону, инжектор в комплекте, но требует настройки RouterOS/CAPsMAN, что в бюджетном проекте с TP‑Link‑экосистемой усложняет эксплуатацию. Ubiquiti U6+ (470 BYN) — лучшая радиочасть в сегменте, но без инжектора и без официального дистрибьютора: в РБ только Onliner, на shop.by карточки нет.

### 3.4. Что не удалось подтвердить

- WPA3 на EAP225: на странице omadanetworks.com (V3) не упомянут; в описании V5 на Onliner — не указано.
- Состав коробки U6+ (раздел «In the box» на сайте Ubiquiti — только изображения); косвенно PoE‑адаптер продаётся отдельно.
- Бюджет PoE Cudy GS2008PS2: производитель указывает 30 Вт/порт и БП 120/130 Вт, суммарный бюджет не называет (Onliner — 118 Вт).
- Тип охлаждения DGS‑1100‑16V2: в извлечённом тексте datasheet строки нет; магазины и описание серии указывают пассивное.
- Zyxel GS1900‑10HP и Netgear GS316EP/GS308EP — не продаются в РБ; характеристики GS1900‑10HP взяты у ритейлеров.
- Скорость 2,4 ГГц у EAP225 V5 (300 против 450 Мбит/с у V3) — по карточке Onliner, страница V5 производителя не загружалась.
