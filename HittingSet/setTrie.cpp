#include <bits/stdc++.h>
#include "setTrie.h"
using namespace std;

// Defines the nodes of the Trie Node class
TrieNode::TrieNode() {

    is_end_of_word = false;
    qtd = 0;

}

// Constructor for the SetTrie Class
SetTrie::SetTrie() {
    root = new TrieNode();
}

// Insert a set in the trie
void SetTrie::insert(vector<bool> &word) {
    TrieNode* current = root;

    for (int i = 0; i < word.size(); i++) {
        if (!word[i]) continue;

        if (current->children.find(i) == current->children.end()) {
            current->children[i] = new TrieNode();
        }

        current->qtd++;
        current = current->children[i];
    }

    current->qtd++;
    current->is_end_of_word = true;
}

// Returns if there is a subset of word in the trie
bool SetTrie::subsetSearch(vector<bool> &word) {

    queue<TrieNode*> fila;
    fila.push(root);

    while (!fila.empty()) {
        TrieNode* at = fila.front();
        fila.pop();

        if (!at->qtd) continue;
        if (at->is_end_of_word) return true;

        for (auto x : at->children) {
            if (word[x.first]) {
                fila.push(x.second);
            }
        }
    }

    return false;
}

// Returns if there is a superset of word in the trie
bool SetTrie::supersetSearch(vector<bool> &word) {

    queue<pair<TrieNode*, int>> fila;
    fila.push({root, 0});

    while (!fila.empty()) {
        pair<TrieNode*, int> aux = fila.front();
        fila.pop();

        TrieNode* at = aux.first;
        int index = aux.second;

        if (!at->qtd) continue;
        
        while (index < word.size() && !word[index]) index++;
        if (index == word.size()) return true;

        for (auto x : at->children) {
            if (x.first < index) {
                fila.push({x.second, index});
            }
            else if (x.first == index) {
                fila.push({x.second, index + 1});
            }
        }
    }

    return false;
}