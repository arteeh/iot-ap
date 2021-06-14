#ifndef OING_H
#define OING_H

// const int test_sampleRate = 8000;
const int oingLength = 3320;

// 8 bits per sample

const unsigned char oingData[] ={
/*    0 */ 125, 125, 123, 126, 134, 136, 134, 130, 130, 127, 127, 122, 116, 125, 133, 134, 
/*   16 */ 131, 126, 124, 123, 123, 121, 124, 127, 133, 132, 127, 129, 128, 128, 126, 122, 
/*   32 */ 120, 125, 129, 129, 127, 128, 135, 131, 127, 125, 127, 128, 125, 141, 115,  67, 
/*   48 */  45,  35,  54,  87, 128, 171, 204, 222, 215, 191, 154, 101,  35,   8,  48, 125, 
/*   64 */ 201, 238, 240, 215, 167, 118,  74,  36,  23,  40,  82, 131, 179, 225, 246, 238, 
/*   80 */ 203, 153, 107,  65,  40,  43,  62,  99, 144, 182, 206, 211, 197, 166, 128,  93, 
/*   96 */  67,  54,  64,  83, 107, 138, 170, 219, 253, 249, 194,  67,   0,   0,  15,  77, 
/*  112 */ 149, 219, 255, 252, 255, 207, 133,  60,   3,   1,   0,  21,  83, 154, 224, 254, 
/*  128 */ 254, 247, 198, 130,  63,  14,   0,  10,  59, 114, 176, 231, 253, 249, 215, 168, 
/*  144 */  93,  21,   0,  14,  93, 174, 220, 234, 219, 180, 131,  85,  42,  21,  25,  52, 
/*  160 */  93, 137, 183, 214, 218, 200, 168, 130,  91,  60,  48,  53,  75, 109, 140, 170, 
/*  176 */ 190, 189, 176, 149, 120,  95,  72,  64,  75,  91, 111, 134, 172, 231, 252, 205, 
/*  192 */ 110,  22,   0,  18,  64, 127, 186, 234, 255, 242, 198, 145,  92,  39,   4,   2, 
/*  208 */  32,  76, 130, 185, 226, 239, 220, 181, 135,  87,  46,  27,  34,  66, 108, 150, 
/*  224 */ 190, 214, 216, 195, 157, 112,  65,  23,  12,  50, 119, 178, 209, 212, 192, 161, 
/*  240 */ 124,  87,  55,  37,  43,  67, 102, 142, 181, 203, 202, 184, 152, 120,  88,  63, 
/*  256 */  54,  62,  86, 114, 144, 172, 190, 189, 171, 142, 113,  87,  71,  66,  76,  99, 
/*  272 */ 124, 151, 171, 206, 241, 220, 138,  36,   0,   7,  42, 109, 168, 229, 255, 253, 
/*  288 */ 227, 162, 100,  40,   3,   0,  18,  69, 127, 186, 231, 251, 238, 198, 147,  94, 
/*  304 */  46,  17,  21,  51,  98, 149, 195, 225, 229, 211, 172, 126,  78,  31,   2,  17, 
/*  320 */  79, 154, 213, 239, 232, 200, 155, 107,  61,  29,  25,  48,  88, 132, 178, 211, 
/*  336 */ 222, 209, 178, 142, 103,  71,  54,  56,  78, 109, 143, 175, 197, 197, 180, 153, 
/*  352 */ 121,  94,  72,  69,  78, 102, 129, 156, 176, 194, 230, 234, 182,  78,   8,   0, 
/*  368 */  10,  77, 146, 219, 255, 252, 255, 200, 121,  46,   0,   3,   0,  41, 109, 185, 
/*  384 */ 250, 254, 255, 225, 158,  92,  18,   0,   0,  24,  89, 153, 222, 255, 254, 235, 
/*  400 */ 183, 124,  54,   6,   0,  15,  82, 170, 233, 255, 250, 207, 153,  94,  33,   2, 
/*  416 */   1,  33,  86, 141, 203, 246, 253, 227, 180, 126,  72,  30,  16,  32,  71, 121, 
/*  432 */ 173, 219, 238, 226, 191, 143,  93,  49,  26,  32,  63, 105, 148, 187, 217, 244, 
/*  448 */ 255, 226, 119,  18,   0,   0,  30, 106, 192, 250, 254, 255, 231, 157,  72,   3, 
/*  464 */   3,   0,   4,  76, 156, 240, 255, 252, 244, 181, 114,  30,   0,   0,   7,  71, 
/*  480 */ 137, 212, 254, 254, 245, 193, 130,  55,   6,   0,   9,  68, 145, 212, 252, 255, 
/*  496 */ 223, 172, 117,  58,  13,   0,  18,  62, 117, 177, 229, 247, 229, 187, 134,  83, 
/*  512 */  37,  18,  31,  64, 110, 161, 205, 230, 224, 193, 150, 100,  57,  33,  38,  65, 
/*  528 */ 107, 150, 186, 209, 213, 224, 219, 174,  77,   2,   0,   4,  61, 144, 226, 255, 
/*  544 */ 251, 251, 194, 114,  36,   0,   0,   0,  57, 130, 211, 255, 252, 249, 194, 130, 
/*  560 */  53,   2,   0,   8,  61, 126, 194, 248, 255, 239, 189, 131,  65,  12,   0,  15, 
/*  576 */  64, 120, 173, 219, 239, 228, 190, 143,  90,  39,  13,  18,  51,  98, 151, 203, 
/*  592 */ 233, 227, 195, 150, 100,  56,  31,  37,  63, 105, 151, 194, 219, 218, 194, 153, 
/*  608 */ 108,  65,  43,  43,  68, 107, 148, 187, 204, 203, 173, 160, 181, 183, 115,  18, 
/*  624 */   0,   3,  45, 131, 216, 255, 255, 252, 200, 118,  32,   0,   2,   5,  67, 140, 
/*  640 */ 224, 255, 255, 247, 178, 110,  37,   0,   0,  22,  82, 146, 222, 254, 253, 228, 
/*  656 */ 166, 105,  41,   1,   3,  41,  91, 136, 188, 236, 253, 227, 177, 119,  59,  12, 
/*  672 */   1,  29,  76, 134, 193, 238, 246, 216, 167, 109,  54,  20,  24,  56, 103, 156, 
/*  688 */ 204, 234, 231, 200, 153, 101,  54,  28,  35,  66, 112, 163, 206, 225, 208, 167, 
/*  704 */ 109,  94, 160, 208, 137,  34,   0,   5,  51, 143, 241, 253, 254, 254, 184,  84, 
/*  720 */  10,   0,   0,  22,  97, 190, 255, 253, 254, 217, 140,  55,   1,   2,   0,  43, 
/*  736 */ 120, 210, 255, 255, 249, 185, 111,  39,   0,   0,  22,  88, 145, 198, 239, 255, 
/*  752 */ 238, 182, 122,  48,   1,   0,  22,  75, 140, 209, 250, 250, 216, 161,  95,  31, 
/*  768 */   0,  12,  58, 117, 183, 236, 252, 231, 185, 128,  69,  24,  15,  42,  89, 142, 
/*  784 */ 199, 234, 233, 195, 139,  73,  30,  77, 187, 232, 137,  26,   0,  19,  91, 192, 
/*  800 */ 247, 255, 253, 207, 118,  30,   0,   0,  20,  85, 172, 244, 255, 254, 214, 139, 
/*  816 */  63,   7,   0,  13,  70, 144, 216, 255, 252, 222, 161,  87,  18,   0,  14,  57, 
/*  832 */ 127, 188, 223, 222, 212, 187, 140,  82,  28,   9,  28,  75, 132, 191, 232, 235, 
/*  848 */ 205, 156, 103,  47,  17,  27,  66, 119, 179, 227, 237, 212, 163, 109,  55,  29, 
/*  864 */  37,  69, 116, 163, 205, 218, 203, 164, 117,  68,  33,  39,  74, 165, 255, 253, 
/*  880 */ 159,  42,   0,  14,  70, 159, 232, 255, 245, 200, 131,  51,   0,   0,  25,  80, 
/*  896 */ 155, 221, 255, 241, 194, 130,  65,  17,  10,  45,  98, 163, 218, 245, 228, 185, 
/*  912 */ 129,  66,  27,  26,  58, 107, 163, 209, 218, 191, 155, 124,  90,  58,  46,  62, 
/*  928 */  97, 142, 184, 205, 196, 165, 127,  88,  60,  52,  71, 105, 147, 185, 204, 195, 
/*  944 */ 164, 126,  84,  59,  58,  81, 117, 154, 185, 192, 180, 148, 116,  84,  65,  70, 
/*  960 */  88, 122, 150, 190, 233, 248, 193,  81,  10,   0,  23,  96, 199, 255, 255, 248, 
/*  976 */ 177,  76,   9,   0,   8,  56, 150, 228, 255, 251, 215, 138,  52,   0,   0,  34, 
/*  992 */  96, 182, 243, 255, 235, 175, 103,  31,   0,  14,  68, 134, 203, 247, 237, 189, 
/* 1008 */ 131,  81,  42,  26,  42,  86, 144, 200, 231, 217, 176, 121,  66,  32,  35,  70, 
/* 1024 */ 118, 171, 210, 219, 193, 152, 102,  55,  41,  58,  98, 147, 192, 210, 197, 161, 
/* 1040 */ 117,  77,  52,  62,  89, 130, 166, 184, 175, 149, 165, 203, 199,  92,   8,   0, 
/* 1056 */   9, 104, 202, 254, 255, 235, 164,  40,   0,   0,  21,  97, 199, 255, 255, 249, 
/* 1072 */ 167,  70,   7,   0,  22,  77, 181, 240, 255, 239, 179,  92,  15,   0,  11,  72, 
/* 1088 */ 154, 232, 255, 230, 175, 103,  45,   6,  12,  54, 118, 196, 249, 249, 201, 136, 
/* 1104 */  60,   9,  12,  58, 128, 199, 242, 230, 185, 121,  58,  26,  39,  83, 139, 197, 
/* 1120 */ 224, 210, 168, 114,  65,  43,  60, 100, 148, 190, 202, 181, 138,  87,  59,  75, 
/* 1136 */ 155, 236, 223, 118,  11,   0,  42, 129, 240, 255, 242, 192,  78,  13,   0,  12, 
/* 1152 */  93, 188, 255, 255, 215, 143,  47,   2,   5,  53, 140, 219, 255, 243, 181, 103, 
/* 1168 */  27,   0,  33,  97, 174, 235, 244, 200, 132,  62,  34,  58,  90, 114, 141, 174, 
/* 1184 */ 198, 191, 159, 115,  71,  50,  62,  98, 142, 180, 193, 173, 132,  89,  63,  69, 
/* 1200 */ 100, 140, 174, 183, 164, 131,  97,  78,  85, 113, 145, 169, 172, 151, 122,  93, 
/* 1216 */  80,  89, 111, 138, 159, 172, 194, 215, 177,  77,   6,   0,  42, 152, 235, 255, 
/* 1232 */ 236, 144,  47,   0,   3,  61, 160, 248, 255, 225, 151,  45,   0,  14,  68, 166, 
/* 1248 */ 244, 253, 218, 136,  44,   1,  18,  84, 172, 242, 248, 198, 122,  45,   7,  34, 
/* 1264 */ 106, 188, 229, 204, 147,  89,  51,  52,  90, 145, 192, 205, 179, 131,  81,  59, 
/* 1280 */  73, 112, 158, 188, 188, 160, 120,  84,  72,  87, 122, 158, 179, 174, 148, 116, 
/* 1296 */  89,  86, 103, 130, 153, 161, 151, 128, 106,  92, 101, 120, 156, 211, 227, 164, 
/* 1312 */  47,   0,  19, 104, 210, 255, 247, 160,  53,   3,   2,  72, 181, 246, 252, 195, 
/* 1328 */  94,  15,   5,  60, 151, 237, 255, 204, 122,  38,   8,  48, 125, 208, 246, 214, 
/* 1344 */ 142,  62,  18,  43, 107, 181, 228, 212, 157,  88,  35,  34,  80, 153, 216, 225, 
/* 1360 */ 181, 110,  47,  34,  73, 141, 201, 213, 176, 111,  58,  49,  87, 147, 196, 200, 
/* 1376 */ 160, 104,  62,  63, 102, 156, 192, 188, 152, 101,  64,  66, 102, 149, 181, 177, 
/* 1392 */ 144, 101,  95, 156, 205, 159,  53,   0,  34, 137, 238, 255, 201,  99,  10,   4, 
/* 1408 */  67, 174, 249, 244, 177,  73,   6,  23, 101, 198, 250, 220, 137,  49,  11,  51, 
/* 1424 */ 133, 214, 238, 190, 108,  38,  31,  83, 161, 217, 210, 153,  84,  53,  70, 112, 
/* 1440 */ 157, 186, 180, 144,  99,  74,  87, 124, 164, 177, 154, 116,  85,  82, 108, 143, 
/* 1456 */ 168, 167, 143, 113,  93,  95, 117, 141, 155, 150, 134, 120, 112, 117, 127, 134, 
/* 1472 */ 131, 125, 119, 119, 126, 131, 133, 134, 146, 174, 177, 117,  38,  24,  87, 181, 
/* 1488 */ 235, 207, 123,  38,  21,  79, 168, 231, 215, 141,  59,  33,  81, 160, 217, 204, 
/* 1504 */ 135,  63,  41,  85, 156, 206, 195, 137,  75,  55,  88, 145, 185, 176, 134,  89, 
/* 1520 */  74, 101, 142, 168, 160, 130, 101,  88, 102, 134, 162, 164, 143, 113,  96, 106, 
/* 1536 */ 128, 146, 148, 134, 118, 112, 125, 140, 145, 136, 118, 108, 113, 130, 146, 149, 
/* 1552 */ 139, 123, 112, 114, 126, 136, 134, 128, 121, 119, 127, 132, 135, 133, 141, 170, 
/* 1568 */ 168,  99,  29,  39, 126, 219, 233, 161,  62,  19,  68, 166, 228, 204, 120,  46, 
/* 1584 */  47, 118, 194, 213, 161,  87,  54,  88, 156, 196, 175, 117,  75,  80, 126, 169, 
/* 1600 */ 173, 137,  98,  89, 115, 151, 160, 140, 111,  95, 105, 136, 162, 156, 122,  91, 
/* 1616 */  96, 131, 163, 164, 133,  99,  92, 116, 150, 164, 144, 114,  99, 111, 139, 154, 
/* 1632 */ 147, 123, 104, 110, 130, 148, 148, 133, 118, 113, 123, 132, 133, 129, 126, 127, 
/* 1648 */ 131, 134, 131, 125, 128, 157, 176, 127,  51,  40, 115, 207, 222, 148,  58,  37, 
/* 1664 */ 106, 194, 212, 149,  71,  54, 110, 182, 196, 144,  81,  67, 113, 168, 180, 140, 
/* 1680 */  92,  84, 116, 155, 161, 134, 106, 103, 127, 147, 145, 127, 112, 116, 128, 125, 
/* 1696 */ 118, 134, 157, 151, 114,  89, 106, 145, 167, 148, 112,  95, 114, 146, 154, 138, 
/* 1712 */ 111, 103, 122, 146, 152, 136, 117, 110, 119, 135, 138, 134, 129, 128, 129, 128, 
/* 1728 */ 122, 118, 123, 131, 136, 134, 127, 124, 127, 131, 135, 157, 173, 126,  52,  48, 
/* 1744 */ 131, 215, 208, 117,  42,  64, 157, 217, 177,  87,  52, 106, 183, 192, 127,  66, 
/* 1760 */  77, 145, 187, 160,  98,  75, 110, 159, 167, 131,  97, 102, 137, 158, 141, 111, 
/* 1776 */ 105, 128, 151, 144, 113,  89, 102, 156, 189, 152,  84,  64, 117, 180, 182, 126, 
/* 1792 */  77,  93, 146, 172, 141,  96,  93, 132, 164, 153, 118, 100, 116, 141, 139, 121, 
/* 1808 */ 113, 127, 148, 146, 123, 104, 110, 131, 143, 137, 121, 118, 128, 138, 134, 122, 
/* 1824 */ 124, 155, 171, 116,  54,  75, 161, 214, 165,  73,  52, 123, 197, 187, 107,  60, 
/* 1840 */ 102, 171, 176, 117,  74, 100, 160, 176, 132,  85,  92, 138, 163, 141, 107, 106, 
/* 1856 */ 134, 152, 134, 108, 109, 131, 152, 143, 118, 108, 115, 121, 130, 155, 161, 127, 
/* 1872 */  86,  92, 141, 171, 150, 106,  98, 129, 152, 139, 110, 109, 135, 150, 136, 115, 
/* 1888 */ 113, 131, 139, 128, 115, 119, 135, 142, 132, 119, 118, 129, 136, 132, 122, 124, 
/* 1904 */ 132, 135, 131, 123, 122, 130, 140, 151, 153, 117,  72,  88, 154, 192, 152,  83, 
/* 1920 */  78, 139, 183, 154,  96,  84, 130, 167, 147, 102,  97, 136, 162, 143, 105, 100, 
/* 1936 */ 132, 152, 134, 110, 114, 138, 146, 127, 108, 117, 138, 142, 128, 117, 122, 132, 
/* 1952 */ 129, 111, 107, 139, 169, 144,  91,  85, 135, 172, 149, 101,  96, 136, 159, 136, 
/* 1968 */ 103, 105, 137, 152, 132, 110, 116, 137, 140, 120, 110, 124, 142, 139, 124, 117, 
/* 1984 */ 126, 134, 132, 124, 123, 131, 133, 127, 124, 128, 135, 134, 128, 131, 150, 141, 
/* 2000 */  93,  80, 133, 183, 162,  98,  81, 131, 173, 147,  97,  95, 139, 162, 134, 102, 
/* 2016 */ 111, 145, 150, 120, 103, 120, 145, 141, 119, 113, 130, 140, 130, 116, 117, 133, 
/* 2032 */ 140, 129, 118, 124, 134, 132, 120, 104, 105, 144, 178, 147,  87,  83, 139, 176, 
/* 2048 */ 146,  96, 101, 145, 156, 122,  99, 119, 149, 145, 116, 108, 130, 146, 132, 112, 
/* 2064 */ 116, 136, 142, 130, 121, 126, 135, 132, 123, 122, 132, 136, 129, 124, 127, 134, 
/* 2080 */ 131, 124, 124, 137, 161, 142,  84,  78, 145, 191, 149,  84,  90, 151, 172, 126, 
/* 2096 */  89, 117, 158, 149, 112, 102, 131, 149, 131, 109, 118, 140, 139, 123, 116, 127, 
/* 2112 */ 136, 130, 118, 120, 132, 137, 129, 122, 126, 133, 131, 124, 117, 110, 117, 152, 
/* 2128 */ 168, 127,  85, 102, 153, 163, 123, 100, 124, 151, 136, 108, 112, 138, 144, 122, 
/* 2144 */ 112, 127, 140, 132, 117, 119, 132, 134, 127, 125, 131, 133, 127, 124, 124, 131, 
/* 2160 */ 134, 130, 127, 127, 131, 130, 128, 128, 130, 144, 154, 125,  83, 103, 163, 173, 
/* 2176 */ 120,  86, 120, 163, 147, 106, 106, 140, 151, 125, 109, 126, 142, 134, 117, 122, 
/* 2192 */ 135, 134, 123, 119, 129, 133, 128, 122, 126, 131, 129, 127, 124, 127, 129, 128, 
/* 2208 */ 128, 123, 120, 114, 123, 153, 157, 118,  90, 117, 157, 149, 113, 110, 137, 145, 
/* 2224 */ 125, 113, 128, 141, 130, 116, 122, 136, 135, 124, 121, 128, 131, 127, 127, 129, 
/* 2240 */ 131, 128, 125, 127, 130, 131, 129, 128, 129, 128, 129, 131, 130, 128, 129, 137, 
/* 2256 */ 148, 132,  94, 100, 151, 169, 127,  94, 118, 154, 143, 111, 111, 136, 141, 123, 
/* 2272 */ 115, 128, 137, 129, 122, 126, 132, 129, 123, 125, 128, 129, 127, 128, 131, 129, 
/* 2288 */ 129, 128, 126, 126, 127, 129, 129, 128, 124, 116, 115, 137, 160, 137,  98, 104, 
/* 2304 */ 145, 151, 120, 110, 131, 146, 130, 115, 125, 137, 131, 121, 125, 134, 133, 126, 
/* 2320 */ 124, 129, 129, 128, 129, 130, 129, 128, 129, 128, 130, 130, 128, 129, 128, 129, 
/* 2336 */ 131, 130, 129, 128, 128, 133, 146, 144, 106,  89, 132, 169, 142, 100, 113, 149, 
/* 2352 */ 144, 113, 112, 135, 140, 124, 119, 130, 134, 126, 124, 129, 131, 127, 123, 127, 
/* 2368 */ 131, 127, 127, 129, 128, 128, 126, 127, 129, 128, 129, 128, 128, 129, 124, 115, 
/* 2384 */ 112, 134, 162, 141,  96, 103, 146, 153, 119, 111, 136, 143, 124, 114, 128, 136, 
/* 2400 */ 126, 121, 129, 133, 128, 124, 126, 130, 129, 126, 129, 131, 129, 128, 129, 129, 
/* 2416 */ 128, 129, 128, 129, 128, 128, 130, 131, 129, 127, 128, 135, 148, 142, 103,  90, 
/* 2432 */ 139, 170, 133,  96, 120, 154, 138, 108, 119, 141, 135, 120, 122, 133, 130, 124, 
/* 2448 */ 126, 131, 130, 122, 125, 131, 129, 125, 125, 129, 128, 124, 125, 128, 129, 127, 
/* 2464 */ 128, 130, 130, 126, 122, 115, 115, 142, 158, 127,  94, 115, 151, 141, 114, 120, 
/* 2480 */ 141, 137, 119, 121, 133, 132, 124, 125, 132, 131, 125, 126, 128, 129, 127, 127, 
/* 2496 */ 130, 130, 128, 129, 128, 129, 128, 129, 130, 129, 128, 128, 130, 130, 128, 128, 
/* 2512 */ 130, 138, 146, 130,  99, 107, 151, 158, 117, 104, 136, 148, 123, 114, 131, 137, 
/* 2528 */ 125, 122, 130, 131, 125, 125, 130, 131, 125, 125, 130, 130, 128, 127, 129, 129, 
/* 2544 */ 127, 126, 128, 129, 127, 126, 127, 129, 127, 124, 123, 118, 124, 145, 148, 117, 
/* 2560 */ 102, 127, 147, 131, 117, 129, 138, 126, 119, 130, 133, 127, 124, 129, 131, 127, 
/* 2576 */ 126, 127, 129, 127, 127, 129, 128, 129, 129, 128, 129, 128, 129, 128, 129, 128, 
/* 2592 */ 129, 128, 129, 128, 129, 129, 131, 136, 143, 130, 102, 113, 151, 150, 116, 111, 
/* 2608 */ 138, 141, 119, 116, 133, 135, 122, 124, 131, 129, 124, 126, 130, 127, 125, 127, 
/* 2624 */ 129, 128, 127, 128, 129, 129, 127, 127, 129, 128, 126, 127, 128, 129, 128, 127, 
/* 2640 */ 125, 119, 120, 142, 151, 120, 102, 126, 148, 132, 117, 131, 139, 127, 120, 130, 
/* 2656 */ 132, 124, 126, 131, 131, 126, 125, 129, 128, 127, 129, 128, 129, 128, 129, 128, 
/* 2672 */ 129, 128, 128, 129, 128, 129, 128, 129, 128, 129, 128, 128, 130, 134, 143, 131, 
/* 2688 */ 101, 109, 150, 153, 116, 110, 140, 141, 117, 119, 138, 134, 121, 125, 134, 128, 
/* 2704 */ 124, 130, 131, 128, 125, 128, 129, 126, 128, 128, 129, 127, 127, 129, 128, 129, 
/* 2720 */ 128, 129, 129, 128, 129, 127, 123, 115, 121, 148, 151, 114, 102, 135, 147, 124, 
/* 2736 */ 117, 135, 138, 121, 123, 135, 129, 123, 127, 132, 127, 124, 129, 131, 129, 127, 
/* 2752 */ 128, 129, 128, 129, 128, 129, 128, 129, 129, 128, 129, 128, 129, 128, 129, 128, 
/* 2768 */ 129, 128, 130, 137, 143, 123,  97, 120, 156, 142, 108, 118, 145, 132, 114, 126, 
/* 2784 */ 138, 127, 121, 132, 132, 124, 125, 131, 129, 124, 128, 129, 129, 127, 127, 129, 
/* 2800 */ 128, 129, 128, 129, 128, 128, 129, 128, 129, 128, 129, 126, 121, 117, 130, 151, 
/* 2816 */ 138, 107, 112, 142, 140, 119, 123, 137, 131, 120, 129, 133, 126, 124, 131, 130, 
/* 2832 */ 125, 128, 131, 129, 127, 128, 129, 128, 129, 128, 129, 128, 129, 129, 128, 129, 
/* 2848 */ 128, 129, 128, 130, 130, 128, 129, 129, 131, 138, 139, 118, 103, 129, 153, 133, 
/* 2864 */ 112, 128, 141, 125, 119, 132, 133, 123, 125, 131, 129, 124, 127, 131, 128, 126, 
/* 2880 */ 127, 129, 128, 127, 129, 128, 129, 128, 128, 129, 128, 129, 127, 127, 129, 129, 
/* 2896 */ 128, 123, 121, 121, 131, 145, 132, 110, 119, 139, 135, 123, 128, 134, 128, 124, 
/* 2912 */ 130, 130, 127, 128, 131, 129, 127, 128, 129, 128, 129, 128, 128, 129, 128, 129, 
/* 2928 */ 128, 129, 129, 128, 129, 128, 129, 128, 128, 129, 128, 129, 128, 129, 130, 136, 
/* 2944 */ 138, 122, 107, 126, 149, 133, 114, 127, 138, 125, 119, 131, 132, 124, 127, 132, 
/* 2960 */ 128, 125, 129, 130, 127, 127, 129, 128, 129, 129, 128, 129, 128, 129, 128, 129, 
/* 2976 */ 128, 129, 128, 129, 128, 129, 128, 126, 122, 118, 129, 146, 136, 111, 117, 139, 
/* 2992 */ 135, 121, 127, 135, 128, 124, 130, 131, 127, 127, 131, 128, 127, 129, 128, 129, 
/* 3008 */ 129, 128, 129, 128, 129, 128, 129, 128, 129, 128, 128, 129, 128, 129, 128, 129, 
/* 3024 */ 128, 129, 128, 128, 130, 134, 140, 124, 104, 123, 150, 136, 113, 126, 138, 125, 
/* 3040 */ 120, 131, 132, 124, 127, 132, 127, 125, 129, 131, 128, 127, 128, 129, 128, 129, 
/* 3056 */ 128, 129, 129, 128, 129, 128, 129, 128, 127, 128, 129, 129, 127, 125, 121, 118, 
/* 3072 */ 132, 148, 132, 108, 121, 141, 130, 120, 130, 134, 125, 124, 130, 128, 126, 128, 
/* 3088 */ 131, 127, 127, 129, 128, 129, 128, 129, 128, 128, 129, 128, 129, 128, 129, 128, 
/* 3104 */ 129, 128, 128, 129, 128, 129, 128, 129, 128, 129, 132, 137, 139, 118, 107, 134, 
/* 3120 */ 150, 126, 114, 133, 136, 122, 124, 132, 128, 124, 131, 131, 126, 127, 130, 129, 
/* 3136 */ 128, 129, 128, 129, 128, 129, 129, 128, 129, 128, 129, 128, 129, 128, 127, 128, 
/* 3152 */ 129, 129, 126, 123, 119, 121, 139, 145, 123, 110, 130, 139, 125, 124, 133, 132, 
/* 3168 */ 125, 127, 131, 127, 127, 129, 128, 127, 128, 129, 128, 129, 128, 129, 128, 129, 
/* 3184 */ 129, 128, 129, 128, 129, 128, 129, 129, 128, 129, 128, 129, 128, 129, 128, 129, 
/* 3200 */ 132, 137, 134, 115, 113, 140, 145, 122, 120, 135, 132, 122, 126, 132, 127, 125, 
/* 3216 */ 131, 129, 124, 128, 129, 128, 129, 128, 129, 128, 129, 128, 128, 129, 128, 129, 
/* 3232 */ 128, 128, 129, 128, 129, 128, 129, 128, 126, 124, 121, 124, 136, 138, 123, 116, 
/* 3248 */ 129, 135, 126, 125, 131, 130, 126, 129, 130, 128, 129, 128, 129, 128, 129, 129, 
/* 3264 */ 128, 129, 128, 129, 128, 129, 128, 129, 128, 128, 129, 128, 129, 128, 129, 128, 
/* 3280 */ 129, 128, 129, 128, 129, 128, 130, 136, 136, 120, 112, 133, 145, 126, 120, 132, 
/* 3296 */ 133, 123, 125, 132, 129, 126, 130, 130, 127, 128, 129, 128, 129, 128, 129, 129, 
/* 3312 */ 128, 129, 128, 129, 128, 129, 129, 128 };

#endif // OING_H