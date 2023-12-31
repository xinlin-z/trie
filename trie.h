/*
 * FileName: trie.h
 * Author:   xinlin-z
 * Desc:     Trie data structure implemented in C++,
 *           with thread-safe and memory pool to speedup.
 * Blog:     https://cs.pynote.net
 * Github:   https://github.com/xinlin-z/trie
 */
#ifndef _TRIE_H
#define _TRIE_H 1
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <utility>
#include <mutex>


// memory allocated for each piece
#define MEM_PIECE  4096


struct Trie{
private:
    struct Node{
        using ump_t = std::unordered_map<char,Node*>;
        char c;
        bool is_word { false };
        bool is_avail { false };
        uint32_t sidx {};
        // sizeof(ump_t) is not fixed cross platform,
        // so here we can only use pointer, which sizeof is fixed
        // in all 64bit environment.
        ump_t *nexts;

        Node(char c);
        ~Node();
    };
    static_assert(sizeof(Node) == 16);
    static_assert(MEM_PIECE%sizeof(Node) == 0);
    uint32_t node_size {};
    uint32_t word_size {};

public:
    Node root {0};
    Trie();
    ~Trie();
    // trie interfaces
    void insert(std::string s);
    bool query(std::string s) noexcept;
    void remove(std::string s);
    void startswith(std::string prefix, std::vector<std::string> &words);
    std::string lcp() noexcept;
    void shrink();
    uint32_t get_node_size() noexcept;
    uint32_t get_word_size() noexcept;

private:
    std::recursive_mutex mtx;
    // speedup startswith
    std::vector<std::pair<std::string,Node*>> _states, _m;
    // memory pool
    const static uint32_t nslot { MEM_PIECE/sizeof(Node) };
    std::vector<Node*> mem;
    std::deque<uint32_t> avail_slot;
    std::pair<Node*,uint32_t> get_mem();
    void del_mem(Node *n) noexcept;
};


#endif

