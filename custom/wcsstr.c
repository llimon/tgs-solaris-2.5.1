#include <wchar.h>

/**
 * Custom implementation of wcsstr()
 * Locates the first occurrence of the wide-character substring `needle` in the wide-character string `haystack`.
 *
 * @param haystack A pointer to the wide-character string to be searched.
 * @param needle A pointer to the wide-character string containing the sequence of wide characters to match.
 * @return A pointer to the located wide-character string, or a null pointer if the wide-character string is not found.
 *         If `needle` is an empty wide-character string, `haystack` is returned.
 */
wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle) {
    // Handle the case where the needle is an empty string
    if (!*needle) {
        return (wchar_t *)haystack;
    }

    // Loop through the haystack string
    while (*haystack) {
        const wchar_t *h = haystack;
        const wchar_t *n = needle;

        // Check for a match
        while (*n && (*h == *n)) {
            h++;
            n++;
        }

        // If the needle was fully matched, return the current position in haystack
        if (!*n) {
            return (wchar_t *)haystack;
        }

        // Move to the next wide character in the haystack
        haystack++;
    }

    // If no match was found, return NULL
    return NULL;
}
