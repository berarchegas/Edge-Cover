#include <iostream>
#include <unordered_map>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool is_end_of_word;

    TrieNode() {
        is_end_of_word = false;
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* current = root;

        for (int i = 0; i < word.length(); i++) {
            char c = word[i];

            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }

            current = current->children[c];
        }

        current->is_end_of_word = true;
    }

    bool search(string word) {
        TrieNode* current = root;

        for (int i = 0; i < word.length(); i++) {
            char c = word[i];

            if (current->children.find(c) == current->children.end()) {
                return false;
            }

            current = current->children[c];
        }

        return current->is_end_of_word;
    }

    bool startsWith(string prefix) {
        TrieNode* current = root;

        for (int i = 0; i < prefix.length(); i++) {
            char c = prefix[i];

            if (current->children.find(c) == current->children.end()) {
                return false;
            }

            current = current->children[c];
        }

        return true;
    }
};

int main() {
    Trie trie = Trie();

    trie.insert("apple");
    cout << trie.search("apple") << endl;   // output: 1
    cout << trie.search("app") << endl;     // output: 0
    cout << trie.startsWith("app") << endl; // output: 1

    trie.insert("banana");
    cout << trie.search("banana") << endl;  // output: 1
    cout << trie.search("ban") << endl;     // output: 0
    cout << trie.startsWith("ban") << endl; // output: 1

    return 0;
}