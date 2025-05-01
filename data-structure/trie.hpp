#pragma once
#include <cassert>
#include <cstring>
#include <vector>
#include <functional>

////////////////////////////////////////////
// Trie
////////////////////////////////////////////

template <int char_size>
struct TrieNode
{
    int nxt[char_size];
    int prev;
    int prev_char;
    int prefix_count;
    vector<int> accept;
    TrieNode() : prev(-1), prev_char(-1), prefix_count(0) { memset(nxt, -1, sizeof(nxt)); }
};

template <int char_size, int margin>
struct Trie
{
    using Node = TrieNode<char_size>;
    vector<Node> nodes;
    int root;
    Trie() : root(0) { nodes.push_back(Node()); }

    void add(const string &str)
    {
        int node_index = root;
        int str_index = count();
        for (char ch : str)
        {
            const int c = ch - margin;
            assert(0 <= c && c < char_size && "character out of bounds: check char_size and margin");
            if (nodes[node_index].nxt[c] == -1)
            {
                nodes[node_index].nxt[c] = (int)nodes.size();
                nodes.push_back(Node());
                nodes.back().prev = node_index;
                nodes.back().prev_char = c;
            }
            ++nodes[node_index].prefix_count;
            node_index = nodes[node_index].nxt[c];
        }
        ++nodes[node_index].prefix_count;
        nodes[node_index].accept.push_back(str_index);
    }
    bool search(const string &str)
    {
        int node_index = root;
        for (char ch : str)
        {
            const int c = ch - margin;
            assert(0 <= c && c < char_size && "character out of bounds: check char_size and margin");
            if (nodes[node_index].nxt[c] == -1)
            {
                return false;
            }
            node_index = nodes[node_index].nxt[c];
        }
        return !nodes[node_index].accept.empty();
    }
    int count_start_with(const string &str)
    {
        int node_index = root;
        for (char ch : str)
        {
            const int c = ch - margin;
            assert(0 <= c && c < char_size && "character out of bounds: check char_size and margin");
            if (nodes[node_index].nxt[c] == -1)
            {
                return 0;
            }
            node_index = nodes[node_index].nxt[c];
        }
        return nodes[node_index].prefix_count;
    }

    void query(const string &str, const function<void(int)> &f)
    {
        int node_index = root;
        for (int idx : nodes[node_index].accept)
        {
            f(idx);
        }
        for (char ch : str)
        {
            const int c = ch - margin;
            assert(0 <= c && c < char_size && "character out of bounds: check char_size and margin");
            if (nodes[node_index].nxt[c] == -1)
            {
                return;
            }
            node_index = nodes[node_index].nxt[c];
            for (int idx : nodes[node_index].accept)
            {
                f(idx);
            }
        }
    }
    int count() const { return (nodes[0].prefix_count); }
};
