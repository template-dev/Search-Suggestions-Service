
#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

class CategoryTrie
{
public:
    CategoryTrie();
    void insert(const std::string& category, const std::string& word, int freq);
    std::vector<std::pair<std::string, int>> search(const std::string& category, const std::string& prefix, int limit = 5);
private:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> m_children;
        std::unordered_map<std::string, int> m_frequency; // Частота слов
        bool m_isEndOfWord = false;
    };
    void collectWords(TrieNode* node, const std::string& prefix, std::vector<std::pair<std::string, int>>& results);
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> m_categories;
};

#endif // TRIE_H
