/*
 * Auto-Complete with Trie:
 * Console tool for suggesting word completions.
 * Demonstrates efficient prefix search using a trie.
 *
 */

#include <memory_resource>
#include <string>
#include <iostream>
#include <cassert>

// POD types don't need an explicit destructor.
struct TSTNode
{
    // TODO: RELATIVE OFFSETS for struct packing
    TSTNode *left = nullptr;
    TSTNode *middle = nullptr;
    TSTNode *right = nullptr;
    char character = '\0';
    bool isEndOfWord = false;
    TSTNode() = default;
    // TODO: Add ctor value for middle variable
    TSTNode(char ch)
        : character(ch) {}
};

inline TSTNode *createNode(std::pmr::memory_resource *pool, char ch)
{
    void *mem = pool->allocate(sizeof(TSTNode), alignof(TSTNode));
    TSTNode *new_node = new (mem) TSTNode();
    new_node->character = ch;

    return new_node;
}

// most words aren't really the same
inline void bulkCreateNodes(std::pmr::memory_resource *pool, std::string_view str, TSTNode *branchPTR)
{
    void *mem = pool->allocate(str.length() * sizeof(TSTNode), alignof(TSTNode));


    // Interpret that memory as TST nodes
    TSTNode *nodes = static_cast<TSTNode *>(mem);

    size_t idx = 0;

    TSTNode *currentBranch = branchPTR;

    // Loop through each character in the string
    for (; idx < str.length(); ++idx)
    {

        // make the new branch.
        std::construct_at(&nodes[idx], str[idx]);

        // which direction for new branch? (for old branch)
        char letter = str[idx];
        bool LEFT = (str[idx] < branchPTR->character);
        bool RIGHT = (str[idx] > branchPTR->character);
        bool MIDDLE = !LEFT && !RIGHT;
        TSTNode *leftNode = LEFT ? &nodes[idx] : nullptr;
        TSTNode *rightNode = RIGHT ? &nodes[idx] : nullptr;
        TSTNode *middleNode = MIDDLE ? &nodes[idx] : nullptr;

        branchPTR->left = leftNode;
        branchPTR->middle = middleNode;
        branchPTR->right = rightNode;

        // go to the next branch.
        branchPTR = &nodes[idx];
    }
    nodes[idx - 1].isEndOfWord = true; // Set it as the end of the word
    return;
}

void insertWord(std::string_view word, TSTNode *&root, std::pmr::memory_resource *pool)
{
    //  empty words are impossible.
    //  yeah, every word does this dumb check.
    [[unlikely]]
    if (root == nullptr)
    {
        root = createNode(pool, word[0]);
    }

    TSTNode *currentNode = root;

    // looks for the first occurrence of a unique branch
    for (size_t index = 0; index != word.length(); index++)
    {
        char currentChar = word[index];
        TSTNode **childPtr = nullptr;

        // Ternary statements for direction checks
        bool LEFT = (currentChar < currentNode->character) ? true : false;
        bool RIGHT = (currentChar > currentNode->character) ? true : false;
        bool MIDDLE = (!LEFT && !RIGHT) ? true : false;

        // IF THE CURRENT LETTER IS SMALLER BUT NO NODE EXISTS / CREATE AND RETURN
        if (LEFT && !currentNode->left)
        {
            bulkCreateNodes(pool, word.substr(index + 1), currentNode);
            return;
        }

        // IF THE LETTER IS SMALLER BUT A NODE EXISTS ALREADY
        if (LEFT && currentNode->left)
        {
            currentNode = currentNode->left;
            continue;
        }

        // IF THE MIDDLE EXISTS BUT THE CURRENT NODE HAS NO MIDDLE / CREATE AND RETURN.
        if (MIDDLE && !currentNode->middle)
        {
            bulkCreateNodes(pool, word.substr(index + 1), currentNode);
            return;
        }

        // IF THE MIDDLE EXISTS BUT THERES A NODE ALREADY.
        if (MIDDLE && currentNode->middle)
        {
            currentNode = currentNode->middle;
            continue;
        }

        // if the right exists but theres no node.
        if (RIGHT && !currentNode->right)
        {
            bulkCreateNodes(pool, word.substr(index + 1), currentNode);
            return;
        }

        // if the right exists and theres a node
        if (RIGHT && currentNode->right)
        {
            currentNode = currentNode->right;
            continue;
        }
    }
}

void search(std::string word, TSTNode *&root)
{
    // NOTE: there are no letters dude.
    if (root == nullptr)
    {
        std::cout << "Empty tree.\n";
        return;
    }

    int wordIndex = 0;
    TSTNode *currentNode = root;

    while (currentNode && wordIndex < word.size())
    {
        char currentChar = word[wordIndex];
        bool LEFT = (currentChar < currentNode->character);
        bool RIGHT = (currentChar > currentNode->character);
        bool MIDDLE = (!LEFT && !RIGHT);

        if (LEFT && !currentNode->left)
            break;
        else if (LEFT)
        {
            currentNode = currentNode->left;
            continue;
        }

        if (MIDDLE && !currentNode->middle)
            break;
        else if (MIDDLE)
        {
            ++wordIndex;
            currentNode = currentNode->middle;
            continue;
        }

        if (RIGHT && !currentNode->right)
            break;
        else if (RIGHT)
        {
            currentNode = currentNode->right;
            continue;
        }
    }

    if (!currentNode)
    {
        std::cout << "No completions found for prefix: " << word << "\n";
        return;
    }

    auto traverse = [&](TSTNode *node, const auto &self, std::string prefix) -> void
    {
        if (!node)
            return;

        prefix.push_back(node->character);

        if (node->isEndOfWord)
        {
            std::cout << "Word: " << prefix << "\n";
        }

        // FIX: allow it.
        for (TSTNode *child : {node->left, node->middle, node->right})
        {
            if (child)
                self(child, self, prefix);
        }
    };

    std::string initialPrefix = word;
    traverse(currentNode, traverse, initialPrefix);
}

int main()
{

    std::pmr::monotonic_buffer_resource pool;
    TSTNode *root = nullptr;

    // multiple inserts is broken.
    insertWord("seth", root, &pool);

    // Now perform some searches.
    search("seth", root);

    return 0;
}