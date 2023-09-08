#include "trie.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
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

    t.shrink();
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
}


void test_2(){
    time_t tm;
    time(&tm);
    srand((unsigned int)(tm%0xFFFFFFFF));

    Trie t {};
    for(int i{}; i<1000; ++i){
        string s { to_string(rand()) };
        t.insert(s);
        HOPE_TRUE(t.query(s));
    }

    vector<string> words;
    t.startswith("", words);
    HOPE_EQ(t.word_size, words.size());

    sort(words.begin(), words.end());
    for(auto &w: words){
        t.remove(w);
        HOPE_FALSE(t.query(w));
        t.shrink();
    }
    HOPE_EQ(t.word_size, 0);
    HOPE_EQ(t.node_size, 0);
    HOPE_TRUE(t.root.nexts.empty());
}


void test_3(){
    time_t tm;
    time(&tm);
    srand((unsigned int)(tm%0xFFFFFFFF));

    Trie t {};
    for(int i{}; i<200; ++i){
        string s { to_string(rand()) };
        string s2 = 'a'+s+s;
        string s3 = 'b'+s+s+s;
        string s4 = 'c'+s+s+s+s;
        string s5 = 'd'+s+s+s+s+s;
        string s6 = 'e'+s+s+s+s+s+s;
        t.insert(s6);
        t.insert(s5);
        t.insert(s4);
        t.insert(s3);
        t.insert(s2);
        HOPE_TRUE(t.query(s6));
        HOPE_TRUE(t.query(s5));
        HOPE_TRUE(t.query(s4));
        HOPE_TRUE(t.query(s3));
        HOPE_TRUE(t.query(s2));
        t.remove(s6);
        t.remove(s5);
        t.remove(s4);
        t.remove(s3);
        t.remove(s2);
        HOPE_FALSE(t.query(s6));
        HOPE_FALSE(t.query(s5));
        HOPE_FALSE(t.query(s4));
        HOPE_FALSE(t.query(s3));
        HOPE_FALSE(t.query(s2));
        t.insert(s);
        HOPE_TRUE(t.query(s));
        t.insert(s6);
        HOPE_TRUE(t.query(s6));
    }

    // the same with test_2
    vector<string> words;
    t.startswith("", words);
    HOPE_EQ(t.word_size, words.size());

    sort(words.begin(), words.end());
    for(auto &w: words){
        t.remove(w);
        HOPE_FALSE(t.query(w));
        t.shrink();
    }
    HOPE_EQ(t.word_size, 0);
    HOPE_EQ(t.node_size, 0);
    HOPE_TRUE(t.root.nexts.empty());
}


int main() {
    test_1();
    test_2();
    test_3();
    cout << "Test OK!\n";
    return 0;
}

