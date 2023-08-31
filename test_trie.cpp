#include "trie.h"
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;


#define HOPE_TRUE(a) \
    do{ \
        if(!a)\
            cerr << "TRUE FAILED: " << __FILE__ << ": " << __LINE__ << endl;\
    } while(0);


#define HOPE_FALSE(a) \
    do{ \
        if(a)\
            cerr << "FALSE FAILED: " << __FILE__ << ": " << __LINE__ << endl;\
    } while(0);


#define HOPE_EQ(a,b) \
    do {\
        if(a != b)\
            cerr << "EQ FAILED: " << __FILE__ << ": " << __LINE__ << endl;\
    } while(0);


void test_1(){
    Trie t;
    t.insert("ab");
    t.insert("abc");
    t.insert("abcd");
    assert(t.lcp() == "ab");
    t.insert("123");
    
    vector<string> words;
    t.startswith("a", words);
    sort(words.begin(), words.end());
    vector<string> words_2 {"ab","abc","abcd"};
    HOPE_EQ(words, words_2);
    words.clear();
    t.startswith("", words);
    sort(words.begin(), words.end());
    words_2.insert(words_2.begin(), "123");
    HOPE_EQ(words, words_2);

    HOPE_TRUE(t.query("ab"));
    HOPE_TRUE(t.query("abc"));
    HOPE_TRUE(t.query("abcd"));
    HOPE_FALSE(t.query("a"));
    HOPE_TRUE(t.query("123"));
    HOPE_FALSE(t.query("1234"));
    HOPE_EQ(t.word_size, 4);
    HOPE_EQ(t.node_size, 7);

    t.remove("abc");
    HOPE_FALSE(t.query("abc"));
    HOPE_EQ(t.word_size, 3);
    HOPE_EQ(t.node_size, 7);
    t.remove("abcd");
    HOPE_FALSE(t.query("abcd"));
    HOPE_EQ(t.word_size, 2);
    HOPE_EQ(t.node_size, 5);
    t.remove("ab");
    HOPE_FALSE(t.query("ab"));
    HOPE_EQ(t.word_size, 1);
    HOPE_EQ(t.node_size, 3);
    t.insert("f");
    HOPE_TRUE(t.query("f"));
    HOPE_EQ(t.word_size, 2);
    HOPE_EQ(t.node_size, 4);
    t.remove("f");
    HOPE_FALSE(t.query("f"));
    HOPE_EQ(t.word_size, 1);
    HOPE_EQ(t.node_size, 3);
    t.remove("kkk");
    HOPE_EQ(t.word_size, 1);
    HOPE_EQ(t.node_size, 3);
    t.remove("123");
    HOPE_FALSE(t.query("123"));
    HOPE_EQ(t.word_size, 0);
    HOPE_EQ(t.node_size, 0);
    HOPE_TRUE(t.root.nexts.empty());
    cout << "Test OK!\n";
}


int main() {
    test_1();
    return 0;
}

