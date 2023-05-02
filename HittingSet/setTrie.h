#ifndef SETTRIE_H
#define SETTRIE_H
#include <bits/stdc++.h>
using namespace std;

// Defines the nodes of the Trie Node class
class TrieNode {
    public:
        unordered_map<int, TrieNode*> children;
        bool is_end_of_word;
        int qtd;

        TrieNode();
};

// Defines the SetTrie class
class SetTrie {
    private:
        TrieNode* root;

    public:
        SetTrie();

        // Insert a set in the trie
        void insert(vector<bool> &word);

        // Returns if there is a subset of word in the trie
        bool subsetSearch(vector<bool> &word);

        // Returns if there is a superset of word in the trie
        bool supersetSearch(vector<bool> &word);

};

#endif