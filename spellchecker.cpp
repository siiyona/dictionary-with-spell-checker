#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

// ---------- Trie ----------
struct TrieNode {
    TrieNode* children[26];
    bool isEnd;

    TrieNode() {
        isEnd = false;
        for (int i = 0; i < 26; i++)
            children[i] = NULL;
    }
};

class Trie {
    TrieNode* root;

public:
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (index < 0 || index >= 26)
                return;
            if (current->children[index] == NULL)
                current->children[index] = new TrieNode();
            current = current->children[index];
        }
        current->isEnd = true;
    }

    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (index < 0 || index >= 26)
                return false;
            if (current->children[index] == NULL)
                return false;
            current = current->children[index];
        }
        return current->isEnd;
    }
};

// ---------- Edit Distance (Levenshtein) ----------
int editDistance(const string& a, const string& b) {
    int m = a.length();
    int n = b.length();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({
                    dp[i - 1][j],       // deletion
                    dp[i][j - 1],       // insertion
                    dp[i - 1][j - 1]    // replacement
                });
        }
    }
    return dp[m][n];
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return tolower(c); });
    return s;
}

// NEW: a valid word is non-empty and contains only letters a-z
bool isValidWord(const string& s) {
    if (s.empty()) return false;
    for (char ch : s)
        if (ch < 'a' || ch > 'z') return false;
    return true;
}

// NEW: add a word to the trie, the vector AND the dictionary.txt file
void addWord(Trie& dictionary, vector<string>& words, const string& word) {
    if (!isValidWord(word)) {
        cout << "Invalid word! Use letters a-z only.\n";
        return;
    }
    if (dictionary.search(word)) {
        cout << "\"" << word << "\" is already in the dictionary.\n";
        return;
    }
    ofstream out("dictionary.txt", ios::app);   // append to the end of the file
    if (!out) {
        cout << "Could not save to dictionary.txt!\n";
        return;
    }
    out << word << "\n";
    out.close();

    words.push_back(word);
    dictionary.insert(word);
    cout << "\"" << word << "\" was added to the dictionary.\n";
}

int main() {
    Trie dictionary;
    vector<string> words;

    ifstream file("dictionary.txt");
    if (!file) {
        cout << "Dictionary file not found!" << endl;
        return 1;
    }

    string word;
    while (file >> word) {
        word = toLower(word);
        if (!isValidWord(word)) continue;        // skip bad lines
        words.push_back(word);
        dictionary.insert(word);
    }
    file.close();

    cout << "DICTIONARY WITH SPELL CHECKER\n";
    cout << words.size() << " words loaded.\n";

    int choice;
    do {
        cout << "\n1. Check Spelling";
        cout << "\n2. Add a Word";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice!\n";
            choice = 0;
            continue;
        }

        if (choice == 1) {
            cout << "Enter a word: ";
            cin >> word;
            word = toLower(word);

            if (dictionary.search(word)) {
                cout << "\nCorrect Spelling!";
                cout << "\nThe word exists in the dictionary.\n";
            } else {
                cout << "\nIncorrect Spelling!";
                cout << "\nDid you mean:\n";

                vector<pair<int, string>> suggestions;
                for (const string& w : words) {
                    int d = editDistance(word, w);
                    if (d <= 2)
                        suggestions.push_back({d, w});
                }

                sort(suggestions.begin(), suggestions.end());

                if (suggestions.empty()) {
                    cout << "No suggestions found.\n";
                } else {
                    for (int i = 0; i < min(5, (int)suggestions.size()); i++)
                        cout << suggestions[i].second << endl;
                }
            }
        }
        else if (choice == 2) {
            cout << "Enter the word to add: ";
            cin >> word;
            addWord(dictionary, words, toLower(word));
        }
        else if (choice == 3) {
            cout << "Thank you for using Spell Checker!\n";
        }
        else {
            cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
