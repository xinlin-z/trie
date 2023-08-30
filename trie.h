#ifndef _TRIE_H
#define _TRIE_H 1
#include <string>
#include <unordered_map>
#include <vector>


struct Trie{
    struct Node{
        using mt = std::unordered_map<char,Node*>;
        char c;
        bool is_word { false };
        mt nexts;
        Node(char c): c{c} {}
    };

    Node root {0};
    uint64_t node_size {};
    uint64_t word_size {};

    ~Trie();

    // trie interfaces
    void insert(std::string s);
    bool query(std::string s) noexcept;
    void remove(std::string s);
    void startswith(std::string prefix, std::vector<std::string> &words);
    std::string lcp() noexcept;
};


#endif

