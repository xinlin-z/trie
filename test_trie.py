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
assert t.word_size == 6

t.insert('cs.pynote.net')
words.append('cs.pynote.net')
t.insert('123456')
words.append('123456')
t.insert('happy coding')
words.append('happy coding')
assert len(t.lcp()) == 0
assert t.word_size == 9


words.remove('abc')
assert sorted(t.startswith('')) == sorted(words)
words2 = words[:]
random.shuffle(words2)
for w in words2:
    t.remove(w)
    words.remove(w)
    assert sorted(t.startswith('')) == sorted(words)
assert t.node_size == 0
assert t.word_size == 0


# stress test
t = Trie()
n = 1000
for i in range(n):
    s = ''.join(str(random.randint(0,999)) for i in range(100))
    t.insert(s)
    assert t.query(s)
words = t.startswith('')
assert len(words) == t.word_size

ks = ('1','2','3','4','5','6','7','8','9')
for k in ks:
    num = sum(1 for w in words if w.startswith(k))
    assert num == len(t.startswith(k))

random.shuffle(words)
for i,w in enumerate(words):
    t.remove(w)
    assert t.query(w) is False
    assert t.word_size == len(words)-i-1
assert t.node_size == 0
assert t.word_size == 0


print('Test OK!')

