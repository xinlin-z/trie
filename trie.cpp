/*
 * FileName: trie.cpp
 * Author:   xinlin-z
 * Desc:     Trie data structure implemented in C++,
 *           with thread-safe and memory pool to speedup.
 * Blog:     https://cs.pynote.net
 * Github:   https://github.com/xinlin-z/trie
 */
#include "trie.h"
#include <cassert>
using namespace std;


pair<Trie::Node*,uint32_t> Trie::get_mem(){
    if(avail_slot.empty()){
        Node *pm { (Node*)(new char[nslot*sizeof(Node)]) };
        mem.push_back(pm);
        uint32_t msize { (uint32_t)mem.size() };
        for(uint32_t i{(msize-1)*nslot+1}; i<msize*nslot; ++i)
            avail_slot.push_back(i);
        for(uint32_t i{1}; i<nslot; ++i)
            (pm+i)->is_avail = true;
        return {pm, (msize-1)*nslot};
    }

    uint32_t sidx { avail_slot[0] };
    uint32_t a { sidx / nslot };
    uint32_t b { sidx % nslot };
    avail_slot.pop_front();
    return {mem[a]+b, sidx};
}


void Trie::del_mem(Trie::Node *n) noexcept{
    // Here call dtor to free inner memory,
    // but the memory for Node itself is not freed.
    // So, we can still assign it's member.
    n->~Node();
    n->is_avail = true;
    avail_slot.push_back(n->sidx);
}


void Trie::insert(string s){
    if((s=="") || query(s))
        return;
    Node *n {&root};
    mtx.lock();
    for(auto c: s){
        if(n->nexts->find(c) == n->nexts->end()){
            auto [p,sidx] { get_mem() };
            (*n->nexts)[c] = new(p) Node{c};
            assert((*n->nexts)[c]->is_avail == false);
            (*n->nexts)[c]->sidx = sidx;
            ++node_size;
        }
        n = (*n->nexts)[c];
    }
    n->is_word = true;
    ++word_size;
    mtx.unlock();
}


bool Trie::query(string s) noexcept{
    Node *n {&root};
    mtx.lock();
    auto not_found {n->nexts->end()};
    for(auto c: s){
        if(n->nexts->find(c) == not_found){
            mtx.unlock();
            return false;
        }
        n = (*n->nexts)[c];
    }
    bool r = n->is_word;
    mtx.unlock();
    return r;
}


void Trie::remove(string s){
    if(s == "")
        return;

    Node *n {&root};
    // include the root node,
    // but not include the last node.
    vector<pair<char,Node*>> path;
    path.reserve(s.size());

    mtx.lock();
    auto not_found {n->nexts->end()};
    for(auto c: s){
        if(n->nexts->find(c) == not_found){
            mtx.unlock();
            return;
        }
        path.emplace_back(c,n);
        n = (*n->nexts)[c];
    }

    --word_size;
    // if the last node is not leaf
    if(!n->nexts->empty()){
        n->is_word = false;
        mtx.unlock();
        return;
    }
    // if it is leaf node
    del_mem(n);
    --node_size;
    // delete nodes until the first non-leaf
    // or leaf but is_word
    // or root
    size_t psize { path.size() };
    while(psize > 1){
        n = path[psize-1].second;
        if((n->nexts->size()>1) || (n->is_word)){
            char c { path[psize-1].first };
            n->nexts->erase(c);
            mtx.unlock();
            return;
        }
        else{
            del_mem(n);
            --node_size;
        }
        path.pop_back();
        --psize;
    }

    // if get here, erase entry in root
    root.nexts->erase(path[0].first);
    mtx.unlock();
}


void Trie::startswith(string prefix, vector<string> &words){
    Node *n {&root};
    mtx.lock();
    for(auto c: prefix){
        if(n->nexts->find(c) == n->nexts->end()){
            mtx.unlock();
            return;
        }
        n = (*n->nexts)[c];
    }

    if(n->is_word)
        words.push_back(prefix);

    _states.clear();
    for(auto [c,pn]: *n->nexts)
        _states.emplace_back(prefix+c, pn);

    while(!_states.empty()){
        _m.clear();
        for(auto [s,pn]: _states){
            for(auto [c,ppn]: *pn->nexts)
                _m.emplace_back(s+c, ppn);
            if(pn->is_word)
                words.push_back(s);
        }
        swap(_states, _m);
    }
    mtx.unlock();
}


string Trie::lcp() noexcept{  // longest common prefix
    string r;
    Node *n {&root};
    mtx.lock();
    while((n->nexts->size()==1) && (!n->is_word)){
        char c { n->nexts->begin()->second->c };
        r += c;
        n = (*n->nexts)[c];
    }
    mtx.unlock();
    return r;
}


void Trie::shrink(){
    // mtx is recursive mutex
    mtx.lock();
    // collect all words
    vector<string> words;
    startswith("", words);
    // delete all memory
    for(auto it{mem.begin()}; it!=mem.end(); ++it){
        for(uint32_t j{}; j<nslot; ++j)
            if((*it+j)->is_avail == false)
                (*it+j)->~Node();
        delete[] (char*)*it;
    }
    // zero out
    mem.clear();
    avail_slot.clear();
    word_size = 0;
    node_size = 0;
    root.nexts->clear();
    // unlock here
    for(auto &w: words)
        insert(w);
    mtx.unlock();
}


Trie::~Trie(){
    for(auto it{mem.begin()}; it!=mem.end(); ++it){
        for(uint32_t j{}; j<nslot; ++j)
            if((*it+j)->is_avail == false)
                (*it+j)->~Node();
        delete[] (char*)*it;
    }
}


