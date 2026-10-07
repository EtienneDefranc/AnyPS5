#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <string>

#include "prx/libc/include/General.hpp"

namespace {

std::size_t WideLength(const char16_t* text) {
    std::size_t length = 0;
    while (text[length] != 0) ++length;
    return length;
}

int WideCompare(const char16_t* first, const char16_t* second, std::size_t count) {
    for (; count != 0; ++first, ++second, --count) {
        if (*first != *second) return *first < *second ? -1 : 1;
        if (*first == 0) return 0;
    }
    return 0;
}

bool WideContains(const char16_t* set, char16_t value) {
    for (; *set != 0; ++set) {
        if (*set == value) return true;
    }
    return false;
}

template<typename TValue, typename TConvert>
TValue ConvertWideNumber(const char16_t* text, char16_t** end, TConvert convert) {
    std::string narrow(WideLength(text), '\0');
    for (std::size_t index = 0; index < narrow.size(); ++index) narrow[index] = text[index] < 0x80 ? static_cast<char>(text[index]) : '\x7f';
    char* narrowEnd = nullptr;
    const TValue value = convert(narrow.c_str(), &narrowEnd);
    if (end != nullptr) *end = const_cast<char16_t*>(text) + (narrowEnd - narrow.c_str());
    return value;
}

}

extern "C" {

void* APS5_VABI memset_nid_postfix(void* s, int c, size_t n) {
    // APS5_LOG_OUT("s=%p c=%d n=%zu", s, c, n);
    return std::memset(s, c, n);
}

void APS5_VABI bzero_nid_postfix(void* destination, size_t count) {
    std::memset(destination, 0, count);
}

void* APS5_VABI memcpy_nid_postfix(void* dest, const void* src, size_t n) {
    return std::memcpy(dest, src, n);
}

void* APS5_VABI memmove_nid_postfix(void* dest, const void* src, size_t n) {
    return std::memmove(dest, src, n);
}

int APS5_VABI memcmp_nid_postfix(const void* s1, const void* s2, size_t n) {
    return std::memcmp(s1, s2, n);
}

const void* APS5_VABI memchr_nid_postfix(const void* s, int c, size_t n) {
    return std::memchr(s, c, n);
}

int APS5_VABI strcmp_nid_postfix(const char* s1, const char* s2) {
    return std::strcmp(s1, s2);
}

int APS5_VABI strncmp_nid_postfix(const char* s1, const char* s2, size_t n) {
    return std::strncmp(s1, s2, n);
}

size_t APS5_VABI strlen_nid_postfix(const char* s) {
    return std::strlen(s);
}

char* APS5_VABI strcpy_nid_postfix(char* dest, const char* src) {
    return std::strcpy(dest, src);
}

char* APS5_VABI strncpy_nid_postfix(char* dest, const char* src, size_t count) {
    return std::strncpy(dest, src, count);
}

char* APS5_VABI strcat_nid_postfix(char* dest, const char* src) {
    return std::strcat(dest, src);
}

const char* APS5_VABI strchr_nid_postfix(const char* s, int c) {
    return std::strchr(s, c);
}

char* APS5_VABI strrchr_nid_postfix(const char* s, int c) {
    return std::strrchr(const_cast<char*>(s), c);
}

char* APS5_VABI strstr_nid_postfix(const char* haystack, const char* needle) {
    return std::strstr(const_cast<char*>(haystack), needle);
}

size_t APS5_VABI strlcpy_nid_postfix(char* dest, const char* src, size_t size) {
    const size_t srcLen = std::strlen(src);
    if (size != 0u) {
        const size_t copyLen = srcLen < size - 1u ? srcLen : size - 1u;
        std::memcpy(dest, src, copyLen);
        dest[copyLen] = '\0';
    }
    return srcLen;
}

std::int64_t APS5_VABI strtol_nid_postfix(const char* str, char** endptr, int base) {
    return std::strtoll(str, endptr, base);
}

std::uint64_t APS5_VABI strtoul_nid_postfix(const char* str, char** endptr, int base) {
    return std::strtoull(str, endptr, base);
}

long long APS5_VABI strtoll_nid_postfix(const char* str, char** endptr, int base) {
    return std::strtoll(str, endptr, base);
}

unsigned long long APS5_VABI strtoull_nid_postfix(const char* str, char** endptr, int base) {
    return std::strtoull(str, endptr, base);
}

double APS5_VABI strtod_nid_postfix(const char* str, char** endptr) {
    return std::strtod(str, endptr);
}

double APS5_VABI atof_nid_postfix(const char* str) { return std::atof(str); }
float APS5_VABI strtof_nid_postfix(const char* str, char** endptr) { return std::strtof(str, endptr); }
long double APS5_VABI strtold_nid_postfix(const char* str, char** endptr) {
    static_assert(sizeof(long double) == 16, "Guest long double requires x87 extended precision storage");
    return std::strtold(str, endptr);
}

int APS5_VABI atoi_nid_postfix(const char* str) {
    return std::atoi(str);
}

std::div_t APS5_VABI div_nid_postfix(int numerator, int denominator) {
    return std::div(numerator, denominator);
}

const char16_t* APS5_VABI wmemchr_nid_postfix(const char16_t* s, char16_t c, size_t n) {
    for (; n != 0; ++s, --n) {
        if (*s == c) return s;
    }
    return nullptr;
}

int APS5_VABI wmemcmp_nid_postfix(const char16_t* s1, const char16_t* s2, size_t n) {
    for (; n != 0; ++s1, ++s2, --n) {
        if (*s1 != *s2) return *s1 < *s2 ? -1 : 1;
    }
    return 0;
}

char16_t* APS5_VABI wmemcpy_nid_postfix(char16_t* dest, const char16_t* src, size_t n) {
    if (n != 0) std::memcpy(dest, src, n * sizeof(char16_t));
    return dest;
}

char16_t* APS5_VABI wmemmove_nid_postfix(char16_t* dest, const char16_t* src, size_t n) {
    if (n != 0) std::memmove(dest, src, n * sizeof(char16_t));
    return dest;
}

}


extern "C" {

int APS5_VABI strcasecmp_nid_postfix(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        unsigned char a = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*s1)));
        unsigned char b = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*s2)));
        if (a != b) return a - b;
        ++s1; ++s2;
    }
    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

int APS5_VABI strncasecmp_nid_postfix(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && *s2) {
        unsigned char a = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*s1)));
        unsigned char b = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*s2)));
        if (a != b) return a - b;
        ++s1; ++s2; --n;
    }
    if (!n) return 0;
    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

int APS5_VABI bcmp_nid_postfix(const void* s1, const void* s2, size_t n) {
    return std::memcmp(s1, s2, n);
}

size_t APS5_VABI strspn_nid_postfix(const char* s, const char* accept) {
    return std::strspn(s, accept);
}

int APS5_VABI strcoll_nid_postfix(const char* s1, const char* s2) {
    return std::strcmp(s1, s2);
}

int APS5_VABI strncpy_s_nid_postfix(char* dest, size_t destsz, const char* src, size_t count) {
    constexpr int GuestEinval = 22;
    constexpr int GuestErange = 34;
    if (!dest || destsz == 0) return GuestEinval;
    if (!src) {
        dest[0] = '\0';
        return GuestEinval;
    }
    size_t length = 0;
    while (length < count && src[length] != '\0') ++length;
    if (length >= destsz) {
        dest[0] = '\0';
        return GuestErange;
    }
    std::memcpy(dest, src, length);
    dest[length] = '\0';
    return 0;
}

int APS5_VABI strcpy_s_nid_postfix(char* dest, size_t destsz, const char* src) {
    return strncpy_s_nid_postfix(dest, destsz, src, static_cast<size_t>(-1));
}

int APS5_VABI strncat_s_nid_postfix(char* dest, size_t destsz, const char* src, size_t count) {
    constexpr int GuestEinval = 22;
    constexpr int GuestErange = 34;
    if (!dest || destsz == 0) return GuestEinval;
    size_t used = 0;
    while (used < destsz && dest[used] != '\0') ++used;
    if (used == destsz || !src) {
        dest[0] = '\0';
        return used == destsz ? GuestErange : GuestEinval;
    }
    size_t length = 0;
    while (length < count && src[length] != '\0') ++length;
    if (length >= destsz - used) {
        dest[0] = '\0';
        return GuestErange;
    }
    std::memcpy(dest + used, src, length);
    dest[used + length] = '\0';
    return 0;
}

int APS5_VABI strcat_s_nid_postfix(char* dest, size_t destsz, const char* src) {
    return strncat_s_nid_postfix(dest, destsz, src, static_cast<size_t>(-1));
}

int APS5_VABI memcpy_s_nid_postfix(void* dest, size_t destsz, const void* src, size_t count) {
    constexpr int GuestEinval = 22;
    constexpr int GuestErange = 34;
    if (!dest) return GuestEinval;
    if (!src || count > destsz) {
        std::memset(dest, 0, destsz);
        return src ? GuestErange : GuestEinval;
    }
    const auto destination = reinterpret_cast<std::uintptr_t>(dest);
    const auto source = reinterpret_cast<std::uintptr_t>(src);
    const auto distance = destination < source ? source - destination : destination - source;
    if (count != 0 && distance < count) {
        std::memset(dest, 0, destsz);
        return GuestEinval;
    }
    std::memcpy(dest, src, count);
    return 0;
}

int APS5_VABI memmove_s_nid_postfix(void* dest, size_t destsz, const void* src, size_t count) {
    constexpr int GuestEinval = 22;
    constexpr int GuestErange = 34;
    if (!dest) return GuestEinval;
    if (!src || count > destsz) {
        std::memset(dest, 0, destsz);
        return src ? GuestErange : GuestEinval;
    }
    std::memmove(dest, src, count);
    return 0;
}

int APS5_VABI memset_s_nid_postfix(void* dest, size_t destsz, int value, size_t count) {
    constexpr int GuestEinval = 22;
    constexpr int GuestErange = 34;
    if (!dest) return GuestEinval;
    const auto length = count > destsz ? destsz : count;
    auto* bytes = static_cast<volatile unsigned char*>(dest);
    for (size_t index = 0; index < length; ++index) bytes[index] = static_cast<unsigned char>(value);
    return count > destsz ? GuestErange : 0;
}

char* APS5_VABI strnstr_nid_postfix(const char* haystack, const char* needle, size_t length) {
    const size_t needleLength = std::strlen(needle);
    if (needleLength == 0) return const_cast<char*>(haystack);
    for (size_t index = 0; index < length && haystack[index] != '\0'; ++index) {
        if (needleLength > length - index) break;
        if (std::strncmp(haystack + index, needle, needleLength) == 0) return const_cast<char*>(haystack + index);
    }
    return nullptr;
}

unsigned long long APS5_VABI _Stoull_nid_postfix(const char* str, char** endptr, int base) {
    return std::strtoull(str, endptr, base);
}

size_t APS5_VABI wcslen_nid_postfix(const char16_t* s) {
    return WideLength(s);
}

int APS5_VABI wcscmp_nid_postfix(const char16_t* s1, const char16_t* s2) {
    return WideCompare(s1, s2, std::numeric_limits<size_t>::max());
}

int APS5_VABI wcsncmp_nid_postfix(const char16_t* s1, const char16_t* s2, size_t n) {
    return WideCompare(s1, s2, n);
}

char16_t* APS5_VABI wcscpy_nid_postfix(char16_t* dest, const char16_t* src) {
    std::memcpy(dest, src, (WideLength(src) + 1) * sizeof(char16_t));
    return dest;
}

char16_t* APS5_VABI wcsncpy_nid_postfix(char16_t* dest, const char16_t* src, size_t n) {
    size_t index = 0;
    for (; index < n && src[index] != 0; ++index) dest[index] = src[index];
    for (; index < n; ++index) dest[index] = 0;
    return dest;
}

const char16_t* APS5_VABI wcschr_nid_postfix(const char16_t* s, char16_t c) {
    for (;; ++s) {
        if (*s == c) return s;
        if (*s == 0) return nullptr;
    }
}

const char16_t* APS5_VABI wcsrchr_nid_postfix(const char16_t* s, char16_t c) {
    const char16_t* found = nullptr;
    for (;; ++s) {
        if (*s == c) found = s;
        if (*s == 0) return found;
    }
}

const char16_t* APS5_VABI wcsstr_nid_postfix(const char16_t* haystack, const char16_t* needle) {
    const size_t length = WideLength(needle);
    for (;; ++haystack) {
        if (WideCompare(haystack, needle, length) == 0) return haystack;
        if (*haystack == 0) return nullptr;
    }
}

const char16_t* APS5_VABI wcspbrk_nid_postfix(const char16_t* s, const char16_t* accept) {
    for (; *s != 0; ++s) {
        if (WideContains(accept, *s)) return s;
    }
    return nullptr;
}

size_t APS5_VABI wcsspn_nid_postfix(const char16_t* s, const char16_t* accept) {
    size_t length = 0;
    while (s[length] != 0 && WideContains(accept, s[length])) ++length;
    return length;
}

char16_t* APS5_VABI wmemset_nid_postfix(char16_t* s, char16_t c, size_t n) {
    for (size_t index = 0; index < n; ++index) s[index] = c;
    return s;
}

double APS5_VABI wcstod_nid_postfix(const char16_t* str, char16_t** endptr) {
    return ConvertWideNumber<double>(str, endptr, [](const char* text, char** end) { return std::strtod(text, end); });
}

float APS5_VABI wcstof_nid_postfix(const char16_t* str, char16_t** endptr) {
    return ConvertWideNumber<float>(str, endptr, [](const char* text, char** end) { return std::strtof(text, end); });
}

long double APS5_VABI wcstold_nid_postfix(const char16_t* str, char16_t** endptr) {
    static_assert(sizeof(long double) == 16);
    static_assert(std::numeric_limits<long double>::digits == 64);
    return ConvertWideNumber<long double>(str, endptr, [](const char* text, char** end) { return std::strtold(text, end); });
}

long long APS5_VABI wcstol_nid_postfix(const char16_t* str, char16_t** endptr, int base) {
    return ConvertWideNumber<long long>(str, endptr, [base](const char* text, char** end) { return std::strtoll(text, end, base); });
}

long long APS5_VABI wcstoll_nid_postfix(const char16_t* str, char16_t** endptr, int base) {
    return ConvertWideNumber<long long>(str, endptr, [base](const char* text, char** end) { return std::strtoll(text, end, base); });
}

unsigned long long APS5_VABI wcstoul_nid_postfix(const char16_t* str, char16_t** endptr, int base) {
    return ConvertWideNumber<unsigned long long>(str, endptr, [base](const char* text, char** end) { return std::strtoull(text, end, base); });
}

unsigned long long APS5_VABI wcstoull_nid_postfix(const char16_t* str, char16_t** endptr, int base) {
    return ConvertWideNumber<unsigned long long>(str, endptr, [base](const char* text, char** end) { return std::strtoull(text, end, base); });
}

int APS5_VABI wcscoll_nid_postfix(const char16_t* first, const char16_t* second) {
    return WideCompare(first, second, std::numeric_limits<size_t>::max());
}

size_t APS5_VABI wcsxfrm_nid_postfix(char16_t* destination, const char16_t* source, size_t count) {
    const size_t length = WideLength(source);
    if (count != 0) std::memcpy(destination, source, std::min(count, length + 1) * sizeof(char16_t));
    return length;
}

size_t APS5_VABI strxfrm_nid_postfix(char* destination, const char* source, size_t count) {
    return std::strxfrm(destination, source, count);
}

}
