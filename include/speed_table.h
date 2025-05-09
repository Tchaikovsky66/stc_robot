const unsigned int speed_profile_int[100] = {
    5,
    6,
    10,
    13,
    17,
    19,
    23,
    25,
    28,
    30,
    33,
    35,
    37,
    39,
    41,
    43,
    44,
    46,
    48,
    49,
    51,
    52,
    54,
    55,
    57,
    58,
    59,
    61,
    62,
    63,
    64,
    65,
    67,
    68,
    69,
    70,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    80,
    81,
    82,
    83,
    84,
    84,
    85,
    86,
    87,
    87,
    88,
    88,
    89,
    90,
    90,
    91,
    91,
    92,
    92,
    93,
    93,
    93,
    94,
    94,
    95,
    95,
    95,
    96,
    96,
    96,
    97,
    97,
    97,
    98,
    98,
    98,
    98,
    98,
    99,
    99,
    99,
    99,
    99,
    99,
    99,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
};

// 自动生成的S曲线速度表
// 最大速度: 200mm/s
// 最小速度: 20mm/s
// 加速度: 400mm/s²
// 加加速度: 1200mm/s³
// 总步数: 1747

#define SPEED_TABLE_SIZE 1747

const unsigned int delay_table[SPEED_TABLE_SIZE] = {
    11520,
    11520,
    11517,
    11511,
    11500,
    11486,
    11467,
    11444,
    11417,
    11386,
    11351,
    11312,
    11270,
    11225,
    11176,
    11125,
    11070,
    11012,
    10951,
    10889,
    10824,
    10756,
    10687,
    10617,
    10544,
    10470,
    10395,
    10318,
    10241,
    10163,
    10084,
    10004,
    9923,
    9843,
    9762,
    9681,
    9600,
    9518,
    9437,
    9356,
    9276,
    9195,
    9115,
    9036,
    8957,
    8878,
    8800,
    8723,
    8646,
    8570,
    8494,
    8420,
    8346,
    8273,
    8200,
    8129,
    8058,
    7988,
    7919,
    7851,
    7784,
    7717,
    7651,
    7586,
    7522,
    7459,
    7397,
    7335,
    7275,
    7215,
    7156,
    7098,
    7040,
    6983,
    6928,
    6873,
    6818,
    6765,
    6712,
    6660,
    6608,
    6558,
    6508,
    6459,
    6410,
    6362,
    6315,
    6269,
    6223,
    6178,
    6133,
    6089,
    6046,
    6003,
    5961,
    5919,
    5878,
    5837,
    5797,
    5758,
    5719,
    5681,
    5643,
    5606,
    5569,
    5533,
    5497,
    5461,
    5426,
    5392,
    5358,
    5324,
    5291,
    5258,
    5226,
    5194,
    5163,
    5132,
    5101,
    5071,
    5041,
    5011,
    4982,
    4953,
    4925,
    4896,
    4869,
    4841,
    4814,
    4787,
    4761,
    4734,
    4709,
    4683,
    4658,
    4633,
    4608,
    4584,
    4559,
    4536,
    4512,
    4489,
    4466,
    4443,
    4420,
    4398,
    4376,
    4354,
    4332,
    4311,
    4290,
    4269,
    4248,
    4228,
    4208,
    4188,
    4168,
    4148,
    4129,
    4110,
    4091,
    4072,
    4053,
    4035,
    4017,
    3998,
    3981,
    3963,
    3945,
    3928,
    3911,
    3894,
    3877,
    3860,
    3844,
    3827,
    3811,
    3795,
    3779,
    3763,
    3748,
    3732,
    3717,
    3702,
    3687,
    3672,
    3657,
    3642,
    3628,
    3613,
    3599,
    3585,
    3571,
    3557,
    3544,
    3530,
    3516,
    3503,
    3490,
    3477,
    3464,
    3451,
    3438,
    3425,
    3412,
    3400,
    3387,
    3375,
    3363,
    3351,
    3339,
    3327,
    3315,
    3303,
    3292,
    3280,
    3269,
    3258,
    3246,
    3235,
    3224,
    3213,
    3202,
    3191,
    3181,
    3170,
    3159,
    3149,
    3139,
    3128,
    3118,
    3108,
    3098,
    3088,
    3078,
    3068,
    3058,
    3048,
    3039,
    3029,
    3020,
    3010,
    3001,
    2992,
    2982,
    2973,
    2964,
    2955,
    2946,
    2937,
    2928,
    2919,
    2911,
    2902,
    2893,
    2885,
    2876,
    2868,
    2860,
    2851,
    2843,
    2835,
    2827,
    2819,
    2811,
    2803,
    2795,
    2787,
    2779,
    2771,
    2763,
    2756,
    2748,
    2740,
    2733,
    2725,
    2718,
    2711,
    2703,
    2696,
    2689,
    2681,
    2674,
    2667,
    2660,
    2658,
    2658,
    2658,
    2658,
    2657,
    2650,
    2644,
    2637,
    2630,
    2623,
    2617,
    2610,
    2604,
    2597,
    2591,
    2585,
    2578,
    2572,
    2566,
    2560,
    2553,
    2547,
    2541,
    2535,
    2529,
    2523,
    2518,
    2512,
    2506,
    2500,
    2494,
    2489,
    2483,
    2477,
    2472,
    2466,
    2461,
    2455,
    2450,
    2445,
    2439,
    2434,
    2429,
    2423,
    2418,
    2413,
    2408,
    2403,
    2398,
    2393,
    2388,
    2383,
    2378,
    2373,
    2368,
    2363,
    2358,
    2353,
    2349,
    2344,
    2339,
    2334,
    2330,
    2325,
    2320,
    2316,
    2311,
    2307,
    2302,
    2298,
    2293,
    2289,
    2285,
    2280,
    2276,
    2272,
    2267,
    2263,
    2259,
    2255,
    2250,
    2246,
    2242,
    2238,
    2234,
    2230,
    2226,
    2222,
    2218,
    2214,
    2210,
    2206,
    2202,
    2198,
    2194,
    2190,
    2186,
    2182,
    2179,
    2175,
    2171,
    2167,
    2163,
    2160,
    2156,
    2152,
    2149,
    2145,
    2141,
    2138,
    2134,
    2131,
    2127,
    2124,
    2120,
    2117,
    2113,
    2110,
    2106,
    2103,
    2099,
    2096,
    2093,
    2089,
    2086,
    2083,
    2079,
    2076,
    2073,
    2069,
    2066,
    2063,
    2060,
    2057,
    2053,
    2050,
    2047,
    2044,
    2041,
    2038,
    2034,
    2031,
    2028,
    2025,
    2022,
    2019,
    2016,
    2013,
    2010,
    2007,
    2004,
    2001,
    1998,
    1995,
    1992,
    1990,
    1987,
    1984,
    1981,
    1978,
    1975,
    1972,
    1970,
    1967,
    1964,
    1961,
    1958,
    1956,
    1953,
    1950,
    1947,
    1945,
    1942,
    1939,
    1937,
    1934,
    1931,
    1929,
    1926,
    1923,
    1921,
    1918,
    1916,
    1913,
    1911,
    1908,
    1905,
    1903,
    1900,
    1898,
    1895,
    1893,
    1890,
    1888,
    1885,
    1883,
    1880,
    1878,
    1876,
    1873,
    1871,
    1868,
    1866,
    1864,
    1861,
    1859,
    1856,
    1854,
    1852,
    1849,
    1847,
    1845,
    1842,
    1840,
    1838,
    1836,
    1833,
    1831,
    1829,
    1827,
    1824,
    1822,
    1820,
    1818,
    1815,
    1813,
    1811,
    1809,
    1807,
    1805,
    1802,
    1800,
    1798,
    1796,
    1794,
    1792,
    1790,
    1787,
    1785,
    1783,
    1781,
    1779,
    1777,
    1775,
    1773,
    1771,
    1769,
    1767,
    1765,
    1763,
    1761,
    1759,
    1757,
    1755,
    1753,
    1751,
    1749,
    1747,
    1745,
    1743,
    1741,
    1739,
    1737,
    1735,
    1733,
    1731,
    1729,
    1728,
    1727,
    1727,
    1727,
    1727,
    1727,
    1727,
    1727,
    1726,
    1724,
    1722,
    1720,
    1718,
    1716,
    1714,
    1712,
    1711,
    1709,
    1707,
    1705,
    1703,
    1701,
    1700,
    1698,
    1696,
    1694,
    1692,
    1691,
    1689,
    1687,
    1685,
    1684,
    1682,
    1680,
    1678,
    1677,
    1675,
    1673,
    1672,
    1670,
    1668,
    1666,
    1665,
    1663,
    1661,
    1660,
    1658,
    1657,
    1655,
    1653,
    1652,
    1650,
    1648,
    1647,
    1645,
    1644,
    1642,
    1641,
    1639,
    1637,
    1636,
    1634,
    1633,
    1631,
    1630,
    1628,
    1627,
    1625,
    1624,
    1622,
    1621,
    1619,
    1618,
    1616,
    1615,
    1613,
    1612,
    1610,
    1609,
    1607,
    1606,
    1605,
    1603,
    1602,
    1600,
    1599,
    1598,
    1596,
    1595,
    1593,
    1592,
    1591,
    1589,
    1588,
    1586,
    1585,
    1584,
    1582,
    1581,
    1580,
    1578,
    1577,
    1576,
    1574,
    1573,
    1572,
    1571,
    1569,
    1568,
    1567,
    1565,
    1564,
    1563,
    1561,
    1560,
    1559,
    1558,
    1556,
    1555,
    1554,
    1553,
    1551,
    1550,
    1549,
    1548,
    1547,
    1545,
    1544,
    1543,
    1542,
    1541,
    1539,
    1538,
    1537,
    1536,
    1535,
    1533,
    1532,
    1531,
    1530,
    1529,
    1528,
    1526,
    1525,
    1524,
    1523,
    1522,
    1521,
    1520,
    1519,
    1517,
    1516,
    1515,
    1514,
    1513,
    1512,
    1511,
    1510,
    1509,
    1508,
    1506,
    1505,
    1504,
    1503,
    1502,
    1501,
    1500,
    1499,
    1498,
    1497,
    1496,
    1495,
    1494,
    1493,
    1492,
    1491,
    1490,
    1489,
    1488,
    1487,
    1486,
    1485,
    1483,
    1482,
    1481,
    1480,
    1480,
    1479,
    1478,
    1477,
    1476,
    1475,
    1474,
    1473,
    1472,
    1471,
    1470,
    1469,
    1468,
    1467,
    1466,
    1465,
    1464,
    1463,
    1462,
    1461,
    1460,
    1459,
    1458,
    1457,
    1457,
    1456,
    1455,
    1454,
    1453,
    1452,
    1451,
    1450,
    1449,
    1448,
    1447,
    1447,
    1446,
    1445,
    1444,
    1443,
    1442,
    1441,
    1440,
    1440,
    1439,
    1438,
    1437,
    1436,
    1435,
    1434,
    1434,
    1433,
    1432,
    1431,
    1430,
    1429,
    1428,
    1428,
    1427,
    1426,
    1425,
    1424,
    1423,
    1423,
    1422,
    1421,
    1420,
    1419,
    1419,
    1418,
    1417,
    1416,
    1415,
    1415,
    1414,
    1413,
    1412,
    1411,
    1411,
    1410,
    1409,
    1408,
    1408,
    1407,
    1406,
    1405,
    1404,
    1404,
    1403,
    1402,
    1401,
    1401,
    1400,
    1399,
    1398,
    1398,
    1397,
    1396,
    1395,
    1395,
    1394,
    1393,
    1392,
    1392,
    1391,
    1390,
    1390,
    1389,
    1388,
    1387,
    1387,
    1386,
    1385,
    1385,
    1384,
    1383,
    1382,
    1382,
    1381,
    1380,
    1380,
    1379,
    1378,
    1378,
    1377,
    1376,
    1375,
    1375,
    1374,
    1373,
    1373,
    1372,
    1371,
    1371,
    1370,
    1369,
    1369,
    1368,
    1367,
    1367,
    1366,
    1365,
    1365,
    1364,
    1363,
    1363,
    1362,
    1362,
    1361,
    1360,
    1360,
    1359,
    1358,
    1358,
    1357,
    1356,
    1356,
    1355,
    1355,
    1354,
    1353,
    1353,
    1352,
    1351,
    1351,
    1350,
    1350,
    1349,
    1348,
    1348,
    1347,
    1346,
    1346,
    1345,
    1345,
    1344,
    1343,
    1343,
    1342,
    1342,
    1341,
    1341,
    1340,
    1339,
    1339,
    1338,
    1338,
    1337,
    1336,
    1336,
    1335,
    1335,
    1334,
    1334,
    1333,
    1332,
    1332,
    1331,
    1331,
    1330,
    1330,
    1329,
    1328,
    1328,
    1327,
    1327,
    1326,
    1326,
    1325,
    1325,
    1324,
    1324,
    1323,
    1322,
    1322,
    1321,
    1321,
    1320,
    1320,
    1319,
    1319,
    1318,
    1318,
    1317,
    1317,
    1316,
    1315,
    1315,
    1314,
    1314,
    1313,
    1313,
    1312,
    1312,
    1311,
    1311,
    1310,
    1310,
    1309,
    1309,
    1308,
    1308,
    1307,
    1307,
    1306,
    1306,
    1305,
    1305,
    1304,
    1304,
    1303,
    1303,
    1302,
    1302,
    1301,
    1301,
    1300,
    1300,
    1299,
    1299,
    1298,
    1298,
    1297,
    1297,
    1297,
    1296,
    1296,
    1295,
    1295,
    1294,
    1294,
    1293,
    1293,
    1292,
    1292,
    1291,
    1291,
    1290,
    1290,
    1290,
    1289,
    1289,
    1288,
    1288,
    1287,
    1287,
    1286,
    1286,
    1285,
    1285,
    1285,
    1284,
    1284,
    1283,
    1283,
    1282,
    1282,
    1281,
    1281,
    1281,
    1280,
    1280,
    1279,
    1279,
    1278,
    1278,
    1278,
    1277,
    1277,
    1276,
    1276,
    1275,
    1275,
    1275,
    1274,
    1274,
    1273,
    1273,
    1273,
    1272,
    1272,
    1271,
    1271,
    1270,
    1270,
    1270,
    1269,
    1269,
    1268,
    1268,
    1268,
    1267,
    1267,
    1266,
    1266,
    1266,
    1265,
    1265,
    1264,
    1264,
    1264,
    1263,
    1263,
    1263,
    1262,
    1262,
    1261,
    1261,
    1261,
    1260,
    1260,
    1259,
    1259,
    1259,
    1258,
    1258,
    1258,
    1257,
    1257,
    1256,
    1256,
    1256,
    1255,
    1255,
    1255,
    1254,
    1254,
    1253,
    1253,
    1253,
    1252,
    1252,
    1252,
    1251,
    1251,
    1251,
    1250,
    1250,
    1249,
    1249,
    1249,
    1248,
    1248,
    1248,
    1247,
    1247,
    1247,
    1246,
    1246,
    1246,
    1245,
    1245,
    1245,
    1244,
    1244,
    1244,
    1243,
    1243,
    1243,
    1242,
    1242,
    1242,
    1241,
    1241,
    1241,
    1240,
    1240,
    1240,
    1239,
    1239,
    1239,
    1238,
    1238,
    1238,
    1237,
    1237,
    1237,
    1236,
    1236,
    1236,
    1235,
    1235,
    1235,
    1234,
    1234,
    1234,
    1233,
    1233,
    1233,
    1232,
    1232,
    1232,
    1231,
    1231,
    1231,
    1231,
    1230,
    1230,
    1230,
    1229,
    1229,
    1229,
    1228,
    1228,
    1228,
    1228,
    1227,
    1227,
    1227,
    1226,
    1226,
    1226,
    1225,
    1225,
    1225,
    1225,
    1224,
    1224,
    1224,
    1223,
    1223,
    1223,
    1223,
    1222,
    1222,
    1222,
    1221,
    1221,
    1221,
    1221,
    1220,
    1220,
    1220,
    1219,
    1219,
    1219,
    1219,
    1218,
    1218,
    1218,
    1217,
    1217,
    1217,
    1217,
    1216,
    1216,
    1216,
    1216,
    1215,
    1215,
    1215,
    1214,
    1214,
    1214,
    1214,
    1213,
    1213,
    1213,
    1213,
    1212,
    1212,
    1212,
    1212,
    1211,
    1211,
    1211,
    1211,
    1210,
    1210,
    1210,
    1210,
    1209,
    1209,
    1209,
    1209,
    1208,
    1208,
    1208,
    1208,
    1207,
    1207,
    1207,
    1207,
    1206,
    1206,
    1206,
    1206,
    1205,
    1205,
    1205,
    1205,
    1204,
    1204,
    1204,
    1204,
    1203,
    1203,
    1203,
    1203,
    1203,
    1202,
    1202,
    1202,
    1202,
    1201,
    1201,
    1201,
    1201,
    1200,
    1200,
    1200,
    1200,
    1200,
    1199,
    1199,
    1199,
    1199,
    1198,
    1198,
    1198,
    1198,
    1198,
    1197,
    1197,
    1197,
    1197,
    1196,
    1196,
    1196,
    1196,
    1196,
    1195,
    1195,
    1195,
    1195,
    1195,
    1194,
    1194,
    1194,
    1194,
    1193,
    1193,
    1193,
    1193,
    1193,
    1192,
    1192,
    1192,
    1192,
    1192,
    1191,
    1191,
    1191,
    1191,
    1191,
    1190,
    1190,
    1190,
    1190,
    1190,
    1189,
    1189,
    1189,
    1189,
    1189,
    1188,
    1188,
    1188,
    1188,
    1188,
    1187,
    1187,
    1187,
    1187,
    1187,
    1187,
    1186,
    1186,
    1186,
    1186,
    1186,
    1185,
    1185,
    1185,
    1185,
    1185,
    1185,
    1184,
    1184,
    1184,
    1184,
    1184,
    1183,
    1183,
    1183,
    1183,
    1183,
    1183,
    1182,
    1182,
    1182,
    1182,
    1182,
    1182,
    1181,
    1181,
    1181,
    1181,
    1181,
    1181,
    1180,
    1180,
    1180,
    1180,
    1180,
    1180,
    1179,
    1179,
    1179,
    1179,
    1179,
    1179,
    1178,
    1178,
    1178,
    1178,
    1178,
    1178,
    1177,
    1177,
    1177,
    1177,
    1177,
    1177,
    1176,
    1176,
    1176,
    1176,
    1176,
    1176,
    1176,
    1175,
    1175,
    1175,
    1175,
    1175,
    1175,
    1174,
    1174,
    1174,
    1174,
    1174,
    1174,
    1174,
    1173,
    1173,
    1173,
    1173,
    1173,
    1173,
    1173,
    1172,
    1172,
    1172,
    1172,
    1172,
    1172,
    1172,
    1172,
    1171,
    1171,
    1171,
    1171,
    1171,
    1171,
    1171,
    1170,
    1170,
    1170,
    1170,
    1170,
    1170,
    1170,
    1169,
    1169,
    1169,
    1169,
    1169,
    1169,
    1169,
    1169,
    1168,
    1168,
    1168,
    1168,
    1168,
    1168,
    1168,
    1168,
    1167,
    1167,
    1167,
    1167,
    1167,
    1167,
    1167,
    1167,
    1167,
    1166,
    1166,
    1166,
    1166,
    1166,
    1166,
    1166,
    1166,
    1165,
    1165,
    1165,
    1165,
    1165,
    1165,
    1165,
    1165,
    1165,
    1164,
    1164,
    1164,
    1164,
    1164,
    1164,
    1164,
    1164,
    1164,
    1164,
    1163,
    1163,
    1163,
    1163,
    1163,
    1163,
    1163,
    1163,
    1163,
    1163,
    1162,
    1162,
    1162,
    1162,
    1162,
    1162,
    1162,
    1162,
    1162,
    1162,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1161,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1160,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1159,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1158,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1157,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1156,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1155,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1154,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1153,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
    1152,
};

const unsigned int speed_table[SPEED_TABLE_SIZE] = {
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    22,
    22,
    22,
    22,
    22,
    22,
    23,
    23,
    23,
    23,
    23,
    23,
    24,
    24,
    24,
    24,
    25,
    25,
    25,
    25,
    25,
    26,
    26,
    26,
    26,
    27,
    27,
    27,
    27,
    28,
    28,
    28,
    28,
    29,
    29,
    29,
    29,
    30,
    30,
    30,
    30,
    31,
    31,
    31,
    31,
    32,
    32,
    32,
    32,
    33,
    33,
    33,
    34,
    34,
    34,
    34,
    35,
    35,
    35,
    35,
    36,
    36,
    36,
    37,
    37,
    37,
    37,
    38,
    38,
    38,
    38,
    39,
    39,
    39,
    40,
    40,
    40,
    40,
    41,
    41,
    41,
    41,
    42,
    42,
    42,
    42,
    43,
    43,
    43,
    44,
    44,
    44,
    44,
    45,
    45,
    45,
    45,
    46,
    46,
    46,
    47,
    47,
    47,
    47,
    48,
    48,
    48,
    48,
    49,
    49,
    49,
    49,
    50,
    50,
    50,
    51,
    51,
    51,
    51,
    52,
    52,
    52,
    52,
    53,
    53,
    53,
    53,
    54,
    54,
    54,
    55,
    55,
    55,
    55,
    56,
    56,
    56,
    56,
    57,
    57,
    57,
    57,
    58,
    58,
    58,
    58,
    59,
    59,
    59,
    59,
    60,
    60,
    60,
    60,
    61,
    61,
    61,
    61,
    62,
    62,
    62,
    62,
    63,
    63,
    63,
    64,
    64,
    64,
    64,
    65,
    65,
    65,
    65,
    66,
    66,
    66,
    66,
    67,
    67,
    67,
    67,
    68,
    68,
    68,
    68,
    68,
    69,
    69,
    69,
    69,
    70,
    70,
    70,
    70,
    71,
    71,
    71,
    71,
    72,
    72,
    72,
    72,
    73,
    73,
    73,
    73,
    74,
    74,
    74,
    74,
    75,
    75,
    75,
    75,
    76,
    76,
    76,
    76,
    77,
    77,
    77,
    77,
    77,
    78,
    78,
    78,
    78,
    79,
    79,
    79,
    79,
    80,
    80,
    80,
    80,
    81,
    81,
    81,
    81,
    81,
    82,
    82,
    82,
    82,
    83,
    83,
    83,
    83,
    84,
    84,
    84,
    84,
    84,
    85,
    85,
    85,
    85,
    86,
    86,
    86,
    86,
    86,
    86,
    86,
    86,
    86,
    87,
    87,
    87,
    87,
    88,
    88,
    88,
    88,
    88,
    89,
    89,
    89,
    89,
    89,
    90,
    90,
    90,
    90,
    91,
    91,
    91,
    91,
    91,
    92,
    92,
    92,
    92,
    92,
    93,
    93,
    93,
    93,
    94,
    94,
    94,
    94,
    94,
    95,
    95,
    95,
    95,
    95,
    96,
    96,
    96,
    96,
    96,
    97,
    97,
    97,
    97,
    97,
    98,
    98,
    98,
    98,
    98,
    99,
    99,
    99,
    99,
    99,
    100,
    100,
    100,
    100,
    100,
    101,
    101,
    101,
    101,
    101,
    101,
    102,
    102,
    102,
    102,
    102,
    103,
    103,
    103,
    103,
    103,
    104,
    104,
    104,
    104,
    104,
    104,
    105,
    105,
    105,
    105,
    105,
    106,
    106,
    106,
    106,
    106,
    107,
    107,
    107,
    107,
    107,
    107,
    108,
    108,
    108,
    108,
    108,
    109,
    109,
    109,
    109,
    109,
    109,
    110,
    110,
    110,
    110,
    110,
    110,
    111,
    111,
    111,
    111,
    111,
    112,
    112,
    112,
    112,
    112,
    112,
    113,
    113,
    113,
    113,
    113,
    113,
    114,
    114,
    114,
    114,
    114,
    114,
    115,
    115,
    115,
    115,
    115,
    115,
    116,
    116,
    116,
    116,
    116,
    116,
    117,
    117,
    117,
    117,
    117,
    117,
    118,
    118,
    118,
    118,
    118,
    118,
    119,
    119,
    119,
    119,
    119,
    119,
    120,
    120,
    120,
    120,
    120,
    120,
    121,
    121,
    121,
    121,
    121,
    121,
    122,
    122,
    122,
    122,
    122,
    122,
    122,
    123,
    123,
    123,
    123,
    123,
    123,
    124,
    124,
    124,
    124,
    124,
    124,
    125,
    125,
    125,
    125,
    125,
    125,
    125,
    126,
    126,
    126,
    126,
    126,
    126,
    127,
    127,
    127,
    127,
    127,
    127,
    127,
    128,
    128,
    128,
    128,
    128,
    128,
    129,
    129,
    129,
    129,
    129,
    129,
    129,
    130,
    130,
    130,
    130,
    130,
    130,
    130,
    131,
    131,
    131,
    131,
    131,
    131,
    132,
    132,
    132,
    132,
    132,
    132,
    132,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    133,
    134,
    134,
    134,
    134,
    134,
    134,
    134,
    135,
    135,
    135,
    135,
    135,
    135,
    135,
    136,
    136,
    136,
    136,
    136,
    136,
    136,
    137,
    137,
    137,
    137,
    137,
    137,
    137,
    138,
    138,
    138,
    138,
    138,
    138,
    138,
    139,
    139,
    139,
    139,
    139,
    139,
    139,
    139,
    140,
    140,
    140,
    140,
    140,
    140,
    140,
    141,
    141,
    141,
    141,
    141,
    141,
    141,
    141,
    142,
    142,
    142,
    142,
    142,
    142,
    142,
    143,
    143,
    143,
    143,
    143,
    143,
    143,
    143,
    144,
    144,
    144,
    144,
    144,
    144,
    144,
    144,
    145,
    145,
    145,
    145,
    145,
    145,
    145,
    145,
    146,
    146,
    146,
    146,
    146,
    146,
    146,
    146,
    147,
    147,
    147,
    147,
    147,
    147,
    147,
    147,
    147,
    148,
    148,
    148,
    148,
    148,
    148,
    148,
    148,
    149,
    149,
    149,
    149,
    149,
    149,
    149,
    149,
    149,
    150,
    150,
    150,
    150,
    150,
    150,
    150,
    150,
    151,
    151,
    151,
    151,
    151,
    151,
    151,
    151,
    151,
    152,
    152,
    152,
    152,
    152,
    152,
    152,
    152,
    152,
    153,
    153,
    153,
    153,
    153,
    153,
    153,
    153,
    153,
    153,
    154,
    154,
    154,
    154,
    154,
    154,
    154,
    154,
    154,
    155,
    155,
    155,
    155,
    155,
    155,
    155,
    155,
    155,
    155,
    156,
    156,
    156,
    156,
    156,
    156,
    156,
    156,
    156,
    157,
    157,
    157,
    157,
    157,
    157,
    157,
    157,
    157,
    157,
    158,
    158,
    158,
    158,
    158,
    158,
    158,
    158,
    158,
    158,
    159,
    159,
    159,
    159,
    159,
    159,
    159,
    159,
    159,
    159,
    159,
    160,
    160,
    160,
    160,
    160,
    160,
    160,
    160,
    160,
    160,
    161,
    161,
    161,
    161,
    161,
    161,
    161,
    161,
    161,
    161,
    161,
    162,
    162,
    162,
    162,
    162,
    162,
    162,
    162,
    162,
    162,
    162,
    163,
    163,
    163,
    163,
    163,
    163,
    163,
    163,
    163,
    163,
    163,
    164,
    164,
    164,
    164,
    164,
    164,
    164,
    164,
    164,
    164,
    164,
    165,
    165,
    165,
    165,
    165,
    165,
    165,
    165,
    165,
    165,
    165,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    166,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    167,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    168,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    169,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    170,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    171,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    172,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    173,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    174,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    175,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    176,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    177,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    178,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    179,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    180,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    181,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    182,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    183,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    184,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    185,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    186,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    187,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    188,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    189,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    190,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    192,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    194,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    195,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    197,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
    199,
};
