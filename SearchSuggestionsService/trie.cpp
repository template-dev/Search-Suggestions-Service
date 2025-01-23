#include "trie.h"

void CategoryTrie::collectWords(TrieNode* node, const std::string& prefix, std::vector<std::pair<std::string, int>>& results) {
    if (!node) return;

    for (const auto& entry : node->m_frequency) {
        results.push_back(entry);
    }

    for (auto& [ch, child] : node->m_children) {
        collectWords(child.get(), prefix + ch, results);
    }
}

void CategoryTrie::insert(const std::string& category, const std::string& word, int freq) {
    auto& rootNode = m_categories[category];
    if (!rootNode) {
        rootNode = std::make_shared<TrieNode>();
    }

    auto node = rootNode.get();
    for (char c : word) {
        if (!node->m_children.count(c)) {
            node->m_children[c] = std::make_unique<TrieNode>();
        }
        node = node->m_children[c].get();
    }

    node->m_isEndOfWord = true;
    node->m_frequency[word] += freq;
}

std::vector<std::pair<std::string, int>> CategoryTrie::search(const std::string& category, const std::string& prefix, int limit /* = 5 */) {
    if (m_categories.find(category) == m_categories.end()) return {};

    std::shared_ptr<TrieNode> rootNode = m_categories[category];
    TrieNode* node = rootNode.get();

    for (char c : prefix) {
        auto it = node->m_children.find(c);
        if (it == node->m_children.end()) return {};
        node = it->second.get();
    }

    std::vector<std::pair<std::string, int>> results;
    collectWords(node, prefix, results);

    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (results.size() > limit) {
        results.resize(limit);
    }

    return results;
}
