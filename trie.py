from collections import namedtuple
import threading


class Trie():
    class Node():
        def __init__(self, v: str, is_word: bool) -> None:
            self.v = v
            self.is_word = is_word
            self.nexts = {}

    def __init__(self) -> None:
        self.root = Trie.Node(None,False)
        self.word_size = 0
        self.node_size = 0
        self.m = threading.Lock()

    def insert(self, word: str) -> None:
        if self.query(word) or word=='':
            return
        n = self.root
        with self.m:
            for c in word:
                if c in n.nexts:
                    n = n.nexts[c]
                else:
                    n.nexts[c] = n = Trie.Node(c,False)
                    self.node_size += 1
            n.is_word = True
            self.word_size += 1

    def query(self, word: str) -> bool:
        n = self.root
        with self.m:
            for c in word:
                if c not in n.nexts:
                    return False
                n = n.nexts[c]
            r = n.is_word
        return r

    def startswith(self, prefix: str) -> list[str]:
        """ non-recursive, inspired by Thompson NFA multi-state algo """
        n = self.root
        with self.m:
            for c in prefix:
                if c not in n.nexts:
                    return []
                n = n.nexts[c]
            wlst = [prefix] if n.is_word else []
            states = [(prefix+c,node) for c,node in n.nexts.items()]
            while states:
                m = []
                for p,node in states:
                    m += [(p+c,n) for c,n in node.nexts.items()]
                    if node.is_word:
                        wlst.append(p)
                states = m
            return wlst

    # namedtuple, used by remove interface
    vnode = namedtuple('vnode', 'v n')

    def remove(self, word: str) -> None:
        assert word != ''
        s = []
        n = self.root
        with self.m:
            for c in word:
                if c not in n.nexts:
                    return
                # include root, but not the last node,
                # n would be the last node.
                s.append(Trie.vnode(c,n))
                n = n.nexts[c]

            self.word_size -= 1
            if n.nexts:  # middle node
                n.is_word = False
                return

            self.node_size -= 1
            while len(s) > 1:
                c, n = s.pop()
                if n.is_word or len(n.nexts)>1:
                    n.nexts.pop(c)
                    return
                self.node_size -= 1
            # delete root entry
            self.root.nexts.pop(s[0].v)

    def lcp(self) -> str:
        """ longest common prefix """
        r = ''
        n = self.root
        with self.m:
            while len(n.nexts)==1 and not n.is_word:
                n = tuple(n.nexts.values())[0]
                r += n.v
            return r


