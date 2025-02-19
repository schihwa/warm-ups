/*
 * Auto-Complete with Trie:
 * Console tool for suggesting word completions.
 * Demonstrates efficient prefix search using a trie.
 *
 */

#include <string>
#include <string_view>
#include <algorithm>
#include <memory_resource>
#include <new> 
#include <memory>

// POD types don't need a destructor.
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
    explicit TSTNode(char ch) : character(ch) {}
};

inline TSTNode *createNode(std::pmr::memory_resource *pool, char ch)
{
    void *mem = pool->allocate(sizeof(TSTNode), alignof(TSTNode));
    TSTNode *new_node = new (mem) TSTNode();
    new_node->character = ch;
    return new_node;
}

// most words aren't really the same 
inline TSTNode *bulkCreateNodes(std::pmr::memory_resource *pool, std::string_view str)
{
    // 
    void *mem = pool->allocate(str.length() * sizeof(TSTNode), alignof(TSTNode));
    TSTNode *nodes = static_cast<TSTNode *>(mem);

    // TODO: set the middle for each node
    for (size_t idx = 0; idx < str.length(); ++idx)
    {
        std::ranges::construct_at(&nodes[idx], str[idx]);
    }

    return nodes;
}

void insertWord(std::string_view word, TSTNode *&root, std::pmr::memory_resource *pool)
{
    //empty words are impossible. 
        // yeah, every word does this dumb check. 
        [[unlikely]]
        if (root == nullptr)
        {
            root = createNode(pool, word[0]);
        }

        TSTNode *currentNode = root;

        // looks for the first occurrence of a unique branch
        for (size_t index = 0; index < word.size(); ++index) {
            char currentChar = word[index];
            TSTNode** childPtr = nullptr;
            
            if (currentChar < currentNode->character) {
                childPtr = &currentNode->left;
            } else if (currentChar > currentNode->character) {
                childPtr = &currentNode->right;
            } else {
                childPtr = &currentNode->middle;
            }
            
            if (*childPtr == nullptr) {
                *childPtr = bulkCreateNodes(pool, word.substr(index));
                currentNode = *childPtr;
                break; 
            }
            
            currentNode = *childPtr;
        }
        
        currentNode->isEndOfWord = true;
}

// TODO: user input with listeners on the typing
int main()
{
    std::pmr::monotonic_buffer_resource pool; 
    TSTNode *root = nullptr;

    std::string word1 = "HELLO";
    std::string word2 = "CAT";

    insertWord(word1, root, &pool);
    insertWord(word2, root, &pool);



    return 0;
}
