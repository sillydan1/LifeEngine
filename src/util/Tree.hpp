/**
 * Copyright (C) 2021 Asger Gitz-Johansen

   This file is part of lifeengine.

    lifeengine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lifeengine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef TREE_HPP
#define TREE_HPP
#include <vector>
#include <functional>

//// N-Tree implementation
template<typename T>
struct Tree {
    T root;
    std::vector<Tree<T>> children;

    Tree() = delete;
    explicit Tree(const T& r) : root(r), children{} {}
    explicit Tree(T&& r) : root{std::forward<T>(r)}, children{} {}

    /** * Moves the provided subtree into this tree's children */
    inline void emplace(Tree<T>&& t) { children.emplace_back(std::forward(t)); }
    /** * Copies the provided subtree into this tree's children */
    void insert(const Tree<T>& t) {
        children.push_back(t);
    }

    template<typename F>
    void tree_accumulate(std::function<F(T&, const F&)> f, F start_val) {
        auto r = f(root, start_val);
        for(auto& c : children) c.tree_accumulate(f, r);
    }
    void tree_apply(std::function<void(T&)> f) {
        f(root);
        for(auto& c : children) c.tree_apply(f);
    }
    void tree_apply(std::function<void(const T&)> f) const {
        f(root);
        for(auto& c : children) c.tree_apply(f);
    }
    void tree_apply(std::function<void(const Tree<T>&)> f) const {
        f(*this);
        for(auto& c : children) c.tree_apply(f);
    }
};

template<typename T>
void apply_tree_func_on_non_tree(T* orig, std::function<T*(T*)> getnext, std::function<void(T*)> f) {
    T* n = getnext(orig);
    if(n) apply_tree_func_on_non_tree(n, getnext, f);
    f(orig);
}

#endif //TREE_HPP
