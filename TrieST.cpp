#include <string>
#include <vector>
#include <memory>

// OPTIMIZE: providing iterator support could help with speed unless tail recursion is enabled. 
namespace TrieST
{
    class Trie final
    {
    public:
        void addWordsFromFile(const std::string_view filename);
        void addWord(const std::string_view word);
        void addWords(const std::vector<std::string> word);

        void removeWord(const std::string_view word);
        void removeWords(const std::vector<std::string> word);

        std::vector<std::string> const autoComplete(const std::string_view prefix);

        // NOTE: Copying would lead to two trees pointing to the same nodes
        // therefore they have been deleted for now.
        Trie(const Trie &) = delete;
        Trie &operator=(const Trie &) = delete;

        // NOTE: Moving is trivial.
        Trie(Trie &&) noexcept = default;
        Trie &operator=(Trie &&) noexcept = default;

        // OPTIMIZE: You can use pointer offsets to reduce node space.
    private:
        struct Node
        {
            // NOTE: ascii is ordered therefore naturally allows comparisons 
            char character_ = '\0';
            std::unique_ptr<Node> left_, middle_, right_;  // Automatic deletion!
        };

        std::unique_ptr<Node> root_ = nullptr; // Root node of the trie.

        // NOTE: Public functions all need search functionality to operate
        std::unique_ptr<Node> search(std::string_view word);
    };
}
