#include <string.h>
#include <stdbool.h>

int countPalindromicSubsequence(char* s) {
    int n = strlen(s);

    int first[26], last[26];

    for (int i = 0; i < 26; i++) {
        first[i] = -1;
        last[i] = -1;
    }

    // Find first and last occurrence of each character
    for (int i = 0; i < n; i++) {
        int c = s[i] - 'a';

        if (first[c] == -1)
            first[c] = i;

        last[c] = i;
    }

    int ans = 0;

    // For every possible outer character
    for (int c = 0; c < 26; c++) {

        if (first[c] == -1 || first[c] == last[c])
            continue;

        bool seen[26] = {false};

        for (int i = first[c] + 1; i < last[c]; i++) {
            seen[s[i] - 'a'] = true;
        }

        for (int i = 0; i < 26; i++) {
            if (seen[i])
                ans++;
        }
    }

    return ans;
}
