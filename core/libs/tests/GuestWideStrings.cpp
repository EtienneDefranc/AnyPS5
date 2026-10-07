#include "prx/libc/include/general/VabiMacros.hpp"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>

extern "C" {
int* APS5_VABI __error_nid_postfix();
std::size_t APS5_VABI wcslen_nid_postfix(const char16_t*);
int APS5_VABI wcscmp_nid_postfix(const char16_t*, const char16_t*);
int APS5_VABI wcsncmp_nid_postfix(const char16_t*, const char16_t*, std::size_t);
char16_t* APS5_VABI wcscpy_nid_postfix(char16_t*, const char16_t*);
char16_t* APS5_VABI wcsncpy_nid_postfix(char16_t*, const char16_t*, std::size_t);
const char16_t* APS5_VABI wcschr_nid_postfix(const char16_t*, char16_t);
const char16_t* APS5_VABI wcsrchr_nid_postfix(const char16_t*, char16_t);
const char16_t* APS5_VABI wcsstr_nid_postfix(const char16_t*, const char16_t*);
const char16_t* APS5_VABI wcspbrk_nid_postfix(const char16_t*, const char16_t*);
std::size_t APS5_VABI wcsspn_nid_postfix(const char16_t*, const char16_t*);
const char16_t* APS5_VABI wmemchr_nid_postfix(const char16_t*, char16_t, std::size_t);
int APS5_VABI wmemcmp_nid_postfix(const char16_t*, const char16_t*, std::size_t);
char16_t* APS5_VABI wmemcpy_nid_postfix(char16_t*, const char16_t*, std::size_t);
char16_t* APS5_VABI wmemmove_nid_postfix(char16_t*, const char16_t*, std::size_t);
char16_t* APS5_VABI wmemset_nid_postfix(char16_t*, char16_t, std::size_t);
double APS5_VABI wcstod_nid_postfix(const char16_t*, char16_t**);
float APS5_VABI wcstof_nid_postfix(const char16_t*, char16_t**);
long double APS5_VABI wcstold_nid_postfix(const char16_t*, char16_t**);
std::int64_t APS5_VABI wcstol_nid_postfix(const char16_t*, char16_t**, int);
std::int64_t APS5_VABI wcstoll_nid_postfix(const char16_t*, char16_t**, int);
std::uint64_t APS5_VABI wcstoul_nid_postfix(const char16_t*, char16_t**, int);
std::uint64_t APS5_VABI wcstoull_nid_postfix(const char16_t*, char16_t**, int);
int APS5_VABI wcscoll_nid_postfix(const char16_t*, const char16_t*);
std::size_t APS5_VABI wcsxfrm_nid_postfix(char16_t*, const char16_t*, std::size_t);
std::size_t APS5_VABI wcsrtombs_nid_postfix(char*, const char16_t**, std::size_t, void*);
int APS5_VABI swprintf_nid_postfix(char16_t*, std::size_t, const char16_t*, ...);
}

static void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "Guest wide string check failed: %s\n", message);
        std::abort();
    }
}

static int Sign(int value) {
    return value < 0 ? -1 : value > 0 ? 1 : 0;
}

int main() {
    static_assert(sizeof(char16_t) == 2);
    const char16_t text[] = u"hello, world";

    Require(wcslen_nid_postfix(u"") == 0, "wcslen empty");
    Require(wcslen_nid_postfix(text) == 12, "wcslen");
    const char16_t paired[] = {u'a', u'b', 0, u'c', u'd', 0};
    Require(wcslen_nid_postfix(paired) == 2, "wcslen stops at a 16-bit null");

    Require(wcscmp_nid_postfix(u"abc", u"abc") == 0, "wcscmp equal");
    Require(Sign(wcscmp_nid_postfix(u"abc", u"abd")) == -1, "wcscmp less");
    Require(Sign(wcscmp_nid_postfix(u"abd", u"abc")) == 1, "wcscmp greater");
    Require(Sign(wcscmp_nid_postfix(u"ab", u"abc")) == -1, "wcscmp prefix");
    Require(Sign(wcscmp_nid_postfix(u"\xffff", u"a")) == 1, "wcscmp is unsigned");
    Require(wcsncmp_nid_postfix(u"abcx", u"abcy", 3) == 0, "wcsncmp limited");
    Require(Sign(wcsncmp_nid_postfix(u"abcx", u"abcy", 4)) == -1, "wcsncmp differs");
    Require(wcsncmp_nid_postfix(u"a", u"b", 0) == 0, "wcsncmp zero length");
    Require(wcscoll_nid_postfix(u"abc", u"abc") == 0, "wcscoll equal");
    Require(Sign(wcscoll_nid_postfix(u"B", u"a")) == -1, "wcscoll uses code unit order");

    char16_t buffer[16];
    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(wcscpy_nid_postfix(buffer, u"copy") == buffer, "wcscpy result");
    Require(std::u16string(buffer) == u"copy" && buffer[5] == 0xaaaa, "wcscpy contents");
    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(wcsncpy_nid_postfix(buffer, u"ab", 5) == buffer, "wcsncpy result");
    Require(buffer[0] == u'a' && buffer[1] == u'b' && buffer[2] == 0 && buffer[3] == 0 && buffer[4] == 0 && buffer[5] == 0xaaaa, "wcsncpy pads with nulls");
    std::memset(buffer, 0xaa, sizeof(buffer));
    wcsncpy_nid_postfix(buffer, u"abcdef", 3);
    Require(buffer[0] == u'a' && buffer[2] == u'c' && buffer[3] == 0xaaaa, "wcsncpy truncates without a null");

    Require(wcschr_nid_postfix(text, u'o') == text + 4, "wcschr");
    Require(wcschr_nid_postfix(text, u'z') == nullptr, "wcschr missing");
    Require(wcschr_nid_postfix(text, 0) == text + 12, "wcschr null");
    Require(wcsrchr_nid_postfix(text, u'o') == text + 8, "wcsrchr");
    Require(wcsrchr_nid_postfix(text, u'z') == nullptr, "wcsrchr missing");
    Require(wcsrchr_nid_postfix(text, 0) == text + 12, "wcsrchr null");
    Require(wcsstr_nid_postfix(text, u"world") == text + 7, "wcsstr");
    Require(wcsstr_nid_postfix(text, u"worlds") == nullptr, "wcsstr missing");
    Require(wcsstr_nid_postfix(text, u"") == text, "wcsstr empty needle");
    Require(wcspbrk_nid_postfix(text, u" ,") == text + 5, "wcspbrk");
    Require(wcspbrk_nid_postfix(text, u"xyz") == nullptr, "wcspbrk missing");
    Require(wcsspn_nid_postfix(text, u"ehlo") == 5, "wcsspn");
    Require(wcsspn_nid_postfix(text, u"") == 0, "wcsspn empty set");

    Require(wmemchr_nid_postfix(paired, 0, 6) == paired + 2, "wmemchr finds a null");
    Require(wmemchr_nid_postfix(paired, u'd', 6) == paired + 4, "wmemchr past a null");
    Require(wmemchr_nid_postfix(paired, u'd', 4) == nullptr, "wmemchr limited");
    Require(wmemcmp_nid_postfix(paired, paired, 6) == 0, "wmemcmp equal");
    const char16_t other[] = {u'a', u'b', 0, u'c', u'e', 0};
    Require(Sign(wmemcmp_nid_postfix(paired, other, 6)) == -1, "wmemcmp past a null");
    Require(wmemcmp_nid_postfix(paired, other, 4) == 0, "wmemcmp limited");
    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(wmemset_nid_postfix(buffer, u'x', 3) == buffer, "wmemset result");
    Require(buffer[0] == u'x' && buffer[2] == u'x' && buffer[3] == 0xaaaa, "wmemset contents");
    Require(wmemcpy_nid_postfix(buffer, paired, 6) == buffer && wmemcmp_nid_postfix(buffer, paired, 6) == 0 && buffer[6] == 0xaaaa, "wmemcpy");
    Require(wmemmove_nid_postfix(buffer + 1, buffer, 5) == buffer + 1, "wmemmove result");
    Require(buffer[0] == u'a' && buffer[1] == u'a' && buffer[2] == u'b' && buffer[3] == 0 && buffer[5] == u'd', "wmemmove overlap");

    char16_t* end = nullptr;
    const char16_t number[] = u"  -12.5e1xyz";
    Require(wcstod_nid_postfix(number, &end) == -125.0 && end == number + 9, "wcstod");
    Require(wcstof_nid_postfix(number, &end) == -125.0f && end == number + 9, "wcstof");
    Require(wcstold_nid_postfix(number, &end) == -125.0L && end == number + 9, "wcstold");
    const char16_t none[] = u"\x00e9" u"12";
    Require(wcstod_nid_postfix(none, &end) == 0.0 && end == none, "wcstod stops at a non-ASCII character");
    Require(std::isinf(wcstod_nid_postfix(u"inf", nullptr)), "wcstod infinity");
    const char16_t hex[] = u"0x7fz";
    Require(wcstol_nid_postfix(hex, &end, 16) == 0x7f && end == hex + 4, "wcstol");
    Require(wcstoll_nid_postfix(u"-9223372036854775808", nullptr, 10) == std::numeric_limits<std::int64_t>::min(), "wcstoll minimum");
    Require(wcstoul_nid_postfix(u"18446744073709551615", nullptr, 10) == std::numeric_limits<std::uint64_t>::max(), "wcstoul maximum");
    const char16_t octal[] = u"0777 rest";
    Require(wcstoull_nid_postfix(octal, &end, 0) == 0777 && end == octal + 4, "wcstoull base 0");
    *__error_nid_postfix() = 0;
    Require(wcstoll_nid_postfix(u"99999999999999999999", nullptr, 10) == std::numeric_limits<std::int64_t>::max() && *__error_nid_postfix() == 34, "wcstoll overflow");
    const char16_t empty[] = u"";
    Require(wcstol_nid_postfix(empty, &end, 10) == 0 && end == empty, "wcstol empty");

    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(wcsxfrm_nid_postfix(buffer, u"key", 16) == 3 && std::u16string(buffer) == u"key", "wcsxfrm");
    Require(wcsxfrm_nid_postfix(nullptr, u"measure", 0) == 7, "wcsxfrm measures");
    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(wcsxfrm_nid_postfix(buffer, u"longer", 3) == 6 && buffer[0] == u'l' && buffer[2] == u'n' && buffer[3] == 0xaaaa, "wcsxfrm truncates");

    char bytes[16];
    const char16_t latin[] = u"caf\x00e9!";
    const char16_t* source = latin;
    std::memset(bytes, 0x55, sizeof(bytes));
    Require(wcsrtombs_nid_postfix(bytes, &source, sizeof(bytes), nullptr) == 5 && source == nullptr, "wcsrtombs");
    Require(std::memcmp(bytes, "caf\xe9!", 6) == 0, "wcsrtombs contents");
    source = latin;
    Require(wcsrtombs_nid_postfix(nullptr, &source, 0, nullptr) == 5 && source == latin, "wcsrtombs measures");
    std::memset(bytes, 0x55, sizeof(bytes));
    Require(wcsrtombs_nid_postfix(bytes, &source, 3, nullptr) == 3 && source == latin + 3 && bytes[3] == 0x55, "wcsrtombs stops at the limit");
    Require(wcsrtombs_nid_postfix(bytes, &source, 3, nullptr) == 2 && source == nullptr && bytes[2] == 0, "wcsrtombs continues");
    const char16_t wide[] = u"a\x4e2d" u"b";
    source = wide;
    *__error_nid_postfix() = 0;
    Require(wcsrtombs_nid_postfix(bytes, &source, sizeof(bytes), nullptr) == static_cast<std::size_t>(-1) && *__error_nid_postfix() == 86 && source == wide + 1, "wcsrtombs unrepresentable");
    source = wide;
    *__error_nid_postfix() = 0;
    Require(wcsrtombs_nid_postfix(nullptr, &source, 0, nullptr) == static_cast<std::size_t>(-1) && *__error_nid_postfix() == 86, "wcsrtombs measures unrepresentable");

    std::memset(buffer, 0xaa, sizeof(buffer));
    Require(swprintf_nid_postfix(buffer, 16, u"%d-%s-%ls", 42, "ab", u"cd") == 8 && std::u16string(buffer) == u"42-ab-cd", "swprintf");
    Require(swprintf_nid_postfix(buffer, 4, u"%d", 12345) < 0, "swprintf truncation");
    return 0;
}
