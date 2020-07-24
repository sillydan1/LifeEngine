#ifndef LIFE_ENGINE_TREE_HPP
#define LIFE_ENGINE_TREE_HPP
#include <vector>
#include <functional>

namespace life {

    //// N-Tree implementation
    template<typename T>
    struct Tree {
        T root;
        std::vector<Tree<T>> children;

        Tree() = delete;
        Tree(const T& r) : root(r), children{} {}
        Tree(T&& r) : root{std::forward(r)}, children{} {}

        /** * Moves the provided subtree into this tree's children */
        inline void emplace(Tree<T>&& t) { children.emplace_back(std::move(t)); }
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
    };

    template<typename T>
    void apply_tree_func_on_non_tree(T* orig, std::function<T*(T*)> getnext, std::function<void(T*)> f) {
        T* n = getnext(orig);
        if(n) apply_tree_func_on_non_tree(n, getnext, f);
        f(orig);
    }
}

#endif //LIFE_ENGINE_TREE_HPP
