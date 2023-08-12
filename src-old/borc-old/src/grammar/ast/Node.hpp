
#pragma once 

#ifndef __ast_node_hpp__
#define __ast_node_hpp__

#include <memory>
#include <string>
#include <vector>

namespace ast {
    struct Buildable;

    typedef std::vector<std::unique_ptr<Buildable>> BuildableArray;

    struct Node {
        virtual ~Node() {}
    };

    /*
    template<typename T>
    struct Value : public Node {
        T value;
    };
    */

    struct Buildable : public Node {
        std::string name;
    };
    
    // struct Library : public Buildable {};
    // struct Program : public Buildable {};

    struct Project : public Node {
        std::string name;
    };

    struct Borcfile : public Node {
        Project project;
        BuildableArray buildables;
    };
}

#endif
