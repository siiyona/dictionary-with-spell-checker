Files
- spellchecker.cpp : C++ console program (Trie + Edit Distance), now with "Add a Word"
- dictionary.txt   : word list, one word per line (keep it next to the program)
- index.html       : web version, works offline (double-click to open)

Run the C++ program
  g++ -std=c++11 spellchecker.cpp -o spellchecker
  ./spellchecker            (on Windows: spellchecker.exe)
  Menu: 1 = Check Spelling, 2 = Add a Word (saved to dictionary.txt), 3 = Exit

Run the website
  Double-click index.html, or run:  python3 -m http.server 8000
  then open http://localhost:8000
  Use the "Add a word" box to grow the dictionary. Words are remembered in your
  browser; "Download dictionary.txt" exports the list for the C++ program.
