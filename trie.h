#ifndef _TRIE_H
#define _TRIE_H 1
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <utility>


struct Trie{
    struct Node{
        using mt = std::unordered_map<char,Node*>;
        char c;
        bool is_word { false };
        uint32_t sidx {};
        mt nexts;
        Node(char c): c{c} {}
    };
    static_assert(sizeof(Node) == 64);

    const static uint32_t nslot { 4096/sizeof(Node) }; // 64
    Node root {0};
    uint32_t node_size {};
    uint32_t word_size {};

    ~Trie();

    // trie interfaces
    void insert(std::string s);
    bool query(std::string s) noexcept;
    void remove(std::string s);
    void startswith(std::string prefix, std::vector<std::string> &words);
    std::string lcp() noexcept;

    // memory pool
    std::vector<Node*> mem;
    std::deque<uint32_t> avail_slot;
    std::pair<Node*,uint32_t> get_mem();
    void del_mem(Node *n) noexcept;
};


#endif

