import threading
import random
random.seed()
from trie import Trie


# basic test
words = ['a','ab','abc','abcd','abcde','abcde12345','abc']
t = Trie()
for w in words:
    t.insert(w)
    assert t.query(w)
assert t.query('bbb') is False
assert t.query('aaa') is False
assert t.query('123') is False
assert t.query('') is False
assert t.lcp() == 'a'
assert t.node_size == 10
assert len(t) == 6

t.insert('cs.pynote.net')
words.append('cs.pynote.net')
t.insert('123456')
words.append('123456')
t.insert('happy coding')
words.append('happy coding')
assert len(t.lcp()) == 0
assert len(t) == 9


words.remove('abc')
assert sorted(t.startswith('')) == sorted(words)
words2 = words[:]
random.shuffle(words2)
for w in words2:
    t.remove(w)
    words.remove(w)
    assert sorted(t.startswith('')) == sorted(words)
assert t.node_size == 0
assert len(t) == 0


# stress test
t = Trie()
n = 1000
for i in range(n):
    s = ''.join(str(random.randint(0,999)) for i in range(100))
    t.insert(s)
    assert t.query(s)
words = t.startswith('')
assert len(words) == len(t)

ks = ('1','2','3','4','5','6','7','8','9')
for k in ks:
    num = sum(1 for w in words if w.startswith(k))
    assert num == len(t.startswith(k))

random.shuffle(words)
for i,w in enumerate(words):
    t.remove(w)
    assert t.query(w) is False
    assert len(t) == len(words)-i-1
assert t.node_size == 0
assert len(t) == 0


# multithread test
def rw_trie(s, t):
    for i in range(1000):
        t.remove(s)
        assert t.query(s) is False
        t.insert(s)
        assert t.query(s)

t = Trie()
threads = []
teststr = ('1234567890', 'abcdefgjkluiop', '123qweasdzxc5678rtyfghvbn',
                '234@#$wer&*()(4578')
for i in range(4):
    xc = threading.Thread(target=rw_trie,
                          args=(teststr[i],t), daemon=True);
    xc.start()
    threads.append(xc)
for i in range(4):
    threads[i].join()
for i in range(4):
    assert t.query(teststr[i])
assert len(t) == 4


print('Test OK!')

