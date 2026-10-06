#include <LyntraNet/Memory.h>

#define COPY_CASE(N) case N: Copy<N>(_dest, _src); break;

template<size_t Size>
void LT::Memory::Copy(
    _Out_writes_bytes_all_(Size)    void* _dest,
    _In_                            const void* _src
)
{
    static_assert(Size <= 256);

    auto* d = static_cast<uint8_t*>(_dest);
    auto* s = static_cast<const uint8_t*>(_src);

    auto* d256 = reinterpret_cast<__m256i*>(d);
    auto* s256 = reinterpret_cast<const __m256i*>(s);

    if constexpr (Size & 256)
    {
        __m256i v0 = _mm256_loadu_si256(s256);
        __m256i v1 = _mm256_loadu_si256(s256 + 1);
        __m256i v2 = _mm256_loadu_si256(s256 + 2);
        __m256i v3 = _mm256_loadu_si256(s256 + 3);
        __m256i v4 = _mm256_loadu_si256(s256 + 4);
        __m256i v5 = _mm256_loadu_si256(s256 + 5);
        __m256i v6 = _mm256_loadu_si256(s256 + 6);
        __m256i v7 = _mm256_loadu_si256(s256 + 7);

        _mm256_storeu_si256(d256, v0);
        _mm256_storeu_si256(d256 + 1, v1);
        _mm256_storeu_si256(d256 + 2, v2);
        _mm256_storeu_si256(d256 + 3, v3);
        _mm256_storeu_si256(d256 + 4, v4);
        _mm256_storeu_si256(d256 + 5, v5);
        _mm256_storeu_si256(d256 + 6, v6);
        _mm256_storeu_si256(d256 + 7, v7);

        d += 256;
        s += 256;
    }

    if constexpr (Size & 128)
    {
        __m256i v0 = _mm256_loadu_si256(s256);
        __m256i v1 = _mm256_loadu_si256(s256 + 1);
        __m256i v2 = _mm256_loadu_si256(s256 + 2);
        __m256i v3 = _mm256_loadu_si256(s256 + 3);

        _mm256_storeu_si256(d256, v0);
        _mm256_storeu_si256(d256 + 1, v1);
        _mm256_storeu_si256(d256 + 2, v2);
        _mm256_storeu_si256(d256 + 3, v3);

        d += 128;
        s += 128;
    }

    if constexpr (Size & 64)
    {
        __m256i v0 = _mm256_loadu_si256(s256);
        __m256i v1 = _mm256_loadu_si256(s256 + 1);

        _mm256_storeu_si256(d256, v0);
        _mm256_storeu_si256(d256 + 1, v1);

        d += 64;
        s += 64;
    }

    if constexpr (Size & 32)
    {
        __m256i v = _mm256_loadu_si256(s256);

        _mm256_storeu_si256(d256, v);

        d += 32;
        s += 32;
    }

    if constexpr (Size & 16)
    {
        __m128i v = _mm_loadu_si128(
            reinterpret_cast<const __m128i*>(s));

        _mm_storeu_si128(
            reinterpret_cast<__m128i*>(d), v);

        d += 16;
        s += 16;
    }

    if constexpr (Size & 8)
    {
        *reinterpret_cast<uint64_t*>(d) =
            *reinterpret_cast<const uint64_t*>(s);

        d += 8;
        s += 8;
    }

    if constexpr (Size & 4)
    {
        *reinterpret_cast<uint32_t*>(d) =
            *reinterpret_cast<const uint32_t*>(s);

        d += 4;
        s += 4;
    }

    if constexpr (Size & 2)
    {
        *reinterpret_cast<uint16_t*>(d) =
            *reinterpret_cast<const uint16_t*>(s);

        d += 2;
        s += 2;
    }

    if constexpr (Size & 1)
    {
        *d = *s;
    }
}
void LT::Memory::Copy(
    _Out_writes_bytes_all_(_size)   void* _dest,
    _In_                            const void* _src,
    _In_                            size_t _size
)
{
    switch (_size)
    {
        COPY_CASE(1)   COPY_CASE(2)   COPY_CASE(3)   COPY_CASE(4)
        COPY_CASE(5)   COPY_CASE(6)   COPY_CASE(7)   COPY_CASE(8)
        COPY_CASE(9)   COPY_CASE(10)  COPY_CASE(11)  COPY_CASE(12)
        COPY_CASE(13)  COPY_CASE(14)  COPY_CASE(15)  COPY_CASE(16)

        COPY_CASE(17)  COPY_CASE(18)  COPY_CASE(19)  COPY_CASE(20)
        COPY_CASE(21)  COPY_CASE(22)  COPY_CASE(23)  COPY_CASE(24)
        COPY_CASE(25)  COPY_CASE(26)  COPY_CASE(27)  COPY_CASE(28)
        COPY_CASE(29)  COPY_CASE(30)  COPY_CASE(31)  COPY_CASE(32)

        COPY_CASE(33)  COPY_CASE(34)  COPY_CASE(35)  COPY_CASE(36)
        COPY_CASE(37)  COPY_CASE(38)  COPY_CASE(39)  COPY_CASE(40)
        COPY_CASE(41)  COPY_CASE(42)  COPY_CASE(43)  COPY_CASE(44)
        COPY_CASE(45)  COPY_CASE(46)  COPY_CASE(47)  COPY_CASE(48)

        COPY_CASE(49)  COPY_CASE(50)  COPY_CASE(51)  COPY_CASE(52)
        COPY_CASE(53)  COPY_CASE(54)  COPY_CASE(55)  COPY_CASE(56)
        COPY_CASE(57)  COPY_CASE(58)  COPY_CASE(59)  COPY_CASE(60)
        COPY_CASE(61)  COPY_CASE(62)  COPY_CASE(63)  COPY_CASE(64)

        COPY_CASE(65)  COPY_CASE(66)  COPY_CASE(67)  COPY_CASE(68)
        COPY_CASE(69)  COPY_CASE(70)  COPY_CASE(71)  COPY_CASE(72)
        COPY_CASE(73)  COPY_CASE(74)  COPY_CASE(75)  COPY_CASE(76)
        COPY_CASE(77)  COPY_CASE(78)  COPY_CASE(79)  COPY_CASE(80)

        COPY_CASE(81)  COPY_CASE(82)  COPY_CASE(83)  COPY_CASE(84)
        COPY_CASE(85)  COPY_CASE(86)  COPY_CASE(87)  COPY_CASE(88)
        COPY_CASE(89)  COPY_CASE(90)  COPY_CASE(91)  COPY_CASE(92)
        COPY_CASE(93)  COPY_CASE(94)  COPY_CASE(95)  COPY_CASE(96)

        COPY_CASE(97)  COPY_CASE(98)  COPY_CASE(99)  COPY_CASE(100)
        COPY_CASE(101) COPY_CASE(102) COPY_CASE(103) COPY_CASE(104)
        COPY_CASE(105) COPY_CASE(106) COPY_CASE(107) COPY_CASE(108)
        COPY_CASE(109) COPY_CASE(110) COPY_CASE(111) COPY_CASE(112)

        COPY_CASE(113) COPY_CASE(114) COPY_CASE(115) COPY_CASE(116)
        COPY_CASE(117) COPY_CASE(118) COPY_CASE(119) COPY_CASE(120)
        COPY_CASE(121) COPY_CASE(122) COPY_CASE(123) COPY_CASE(124)
        COPY_CASE(125) COPY_CASE(126) COPY_CASE(127) COPY_CASE(128)

        COPY_CASE(129) COPY_CASE(130) COPY_CASE(131) COPY_CASE(132)
        COPY_CASE(133) COPY_CASE(134) COPY_CASE(135) COPY_CASE(136)
        COPY_CASE(137) COPY_CASE(138) COPY_CASE(139) COPY_CASE(140)
        COPY_CASE(141) COPY_CASE(142) COPY_CASE(143) COPY_CASE(144)

        COPY_CASE(145) COPY_CASE(146) COPY_CASE(147) COPY_CASE(148)
        COPY_CASE(149) COPY_CASE(150) COPY_CASE(151) COPY_CASE(152)
        COPY_CASE(153) COPY_CASE(154) COPY_CASE(155) COPY_CASE(156)
        COPY_CASE(157) COPY_CASE(158) COPY_CASE(159) COPY_CASE(160)

        COPY_CASE(161) COPY_CASE(162) COPY_CASE(163) COPY_CASE(164)
        COPY_CASE(165) COPY_CASE(166) COPY_CASE(167) COPY_CASE(168)
        COPY_CASE(169) COPY_CASE(170) COPY_CASE(171) COPY_CASE(172)
        COPY_CASE(173) COPY_CASE(174) COPY_CASE(175) COPY_CASE(176)

        COPY_CASE(177) COPY_CASE(178) COPY_CASE(179) COPY_CASE(180)
        COPY_CASE(181) COPY_CASE(182) COPY_CASE(183) COPY_CASE(184)
        COPY_CASE(185) COPY_CASE(186) COPY_CASE(187) COPY_CASE(188)
        COPY_CASE(189) COPY_CASE(190) COPY_CASE(191) COPY_CASE(192)

        COPY_CASE(193) COPY_CASE(194) COPY_CASE(195) COPY_CASE(196)
        COPY_CASE(197) COPY_CASE(198) COPY_CASE(199) COPY_CASE(200)
        COPY_CASE(201) COPY_CASE(202) COPY_CASE(203) COPY_CASE(204)
        COPY_CASE(205) COPY_CASE(206) COPY_CASE(207) COPY_CASE(208)

        COPY_CASE(209) COPY_CASE(210) COPY_CASE(211) COPY_CASE(212)
        COPY_CASE(213) COPY_CASE(214) COPY_CASE(215) COPY_CASE(216)
        COPY_CASE(217) COPY_CASE(218) COPY_CASE(219) COPY_CASE(220)
        COPY_CASE(221) COPY_CASE(222) COPY_CASE(223) COPY_CASE(224)

        COPY_CASE(225) COPY_CASE(226) COPY_CASE(227) COPY_CASE(228)
        COPY_CASE(229) COPY_CASE(230) COPY_CASE(231) COPY_CASE(232)
        COPY_CASE(233) COPY_CASE(234) COPY_CASE(235) COPY_CASE(236)
        COPY_CASE(237) COPY_CASE(238) COPY_CASE(239) COPY_CASE(240)

        COPY_CASE(241) COPY_CASE(242) COPY_CASE(243) COPY_CASE(244)
        COPY_CASE(245) COPY_CASE(246) COPY_CASE(247) COPY_CASE(248)
        COPY_CASE(249) COPY_CASE(250) COPY_CASE(251) COPY_CASE(252)
        COPY_CASE(253) COPY_CASE(254) COPY_CASE(255) COPY_CASE(256)
    default:
        std::memcpy(_dest, _src, _size);
        break;
    }
}