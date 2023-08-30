#include "trie.h"
#include <cassert>
using namespace std;


void Trie::insert(string s){
    if((s=="") || query(s))
        return;
    Node *n {&root};
    for(auto c: s){
        if(n->nexts.find(c) == n->nexts.end()){
            n->nexts[c] = new Node{c};
            ++node_size;
        }
        n = n->nexts[c];
    }
    n->is_word = true;
    ++word_size;
}


bool Trie::query(string s) noexcept{
    Node *n {&root};
    auto not_found {n->nexts.end()};
    for(auto c: s){
        if(n->nexts.find(c) == not_found)
            return false;
        n = n->nexts[c];
    }
    return n->is_word;
}


void Trie::remove(string s){
    if(s == "")
        return;

    Node *n {&root};

    // include the root node,
    // but not include the last node.
    vector<pair<char,Node*>> path;
    path.reserve(s.size());

    auto not_found {n->nexts.end()};
    for(auto c: s){
        if(n->nexts.find(c) == not_found)
            return;
        path.emplace_back(c,n);
        n = n->nexts[c];
    }

    --word_size;

    // if the last node is not leaf
    if(!n->nexts.empty()){
        n->is_word = false;
        return;
    }

    // if it is leaf node
    delete n;
    --node_size;

    // delete nodes until the first non-leaf
    // or leaf but is_word
    // or root
    size_t psize { path.size() };
    while(psize > 1){
        n = path[psize-1].second;

        if((n->nexts.size()>1) || (n->is_word)){
            char c { path[psize-1].first };
            n->nexts.erase(c);
            return;
        }
        else{
            delete n;
            --node_size;
        }

        path.pop_back();
        --psize;
    }

    // if get here, erase entry in root
    root.nexts.erase(path[0].first);
}


void Trie::startswith(string prefix, vector<string> &words){
    Node *n {&root};
    for(auto c: prefix){
        if(n->nexts.find(c) == n->nexts.end())
            return;
        n = n->nexts[c];
    }

    if(n->is_word)
        words.push_back(prefix);

    vector<pair<string,Node*>> states,m;
    for(auto [c,pn]: n->nexts)
        states.emplace_back(prefix+c, pn);
    while(!states.empty()){
        m.clear();
        for(auto [s,pn]: states){
            for(auto [c,ppn]: pn->nexts)
                m.emplace_back(s+c, ppn);
            if(pn->is_word)
                words.push_back(s);
        }
        states.assign(m.begin(), m.end());
    }
}


string Trie::lcp() noexcept{  // longest common prefix
    string r;
    Node *n {&root};
    while((n->nexts.size()==1) && (!n->is_word)){
        char c { n->nexts.begin()->second->c };
        r += c;
        n = n->nexts[c];
    }
    return r;
}


Trie::~Trie(){
    vector<string> words;
    words.reserve(word_size);
    startswith("", words);
    for(auto &w: words)
        remove(w);
    assert(word_size == 0);
    assert(node_size == 0);
}


