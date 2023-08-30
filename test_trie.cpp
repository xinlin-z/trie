#include "trie.h"
#include <iostream>
#include <cassert>
using namespace std;


int main() {
    Trie t;
    t.insert("ab");
    t.insert("abc");
    t.insert("abcd");
    assert(t.lcp() == "ab");
    t.insert("123");
    
    vector<string> words;
    t.startswith("a", words);
    cout << "start:" << endl;
    for(auto w{words.begin()}; w!=words.end(); ++w)
        cout << *w << " ";
    cout << endl;
    words.clear();
    t.startswith("", words);
    for(auto w{words.begin()}; w!=words.end(); ++w)
        cout << *w << " ";
    cout << endl;

    assert(t.query("ab"));
    assert(t.query("abc"));
    assert(t.query("abcd"));
    assert(!t.query("a"));
    assert(t.query("123"));
    assert(!t.query("1234"));
    assert(t.word_size == 4);
    assert(t.node_size == 7);
    t.remove("abc");
    assert(!t.query("abc"));
    assert(t.word_size == 3);
    assert(t.node_size == 7);
    t.remove("abcd");
    assert(!t.query("abcd"));
    assert(t.word_size == 2);
    assert(t.node_size == 5);
    t.remove("ab");
    assert(!t.query("ab"));
    assert(t.word_size == 1);
    assert(t.node_size == 3);
    t.insert("f");
    assert(t.query("f"));
    assert(t.word_size == 2);
    assert(t.node_size == 4);
    t.remove("f");
    assert(!t.query("f"));
    assert(t.word_size == 1);
    assert(t.node_size == 3);
    t.remove("kkk");
    assert(t.word_size == 1);
    assert(t.node_size == 3);
    t.remove("123");
    assert(!t.query("123"));
    assert(t.word_size == 0);
    assert(t.node_size == 0);
    assert(t.root.nexts.empty());
    cout << "Test OK!\n";
    return 0;
}

