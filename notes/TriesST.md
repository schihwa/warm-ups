**Populate from File:**
- Verify the tree contains all words from a non-empty file.
- Verify an empty file results in an empty tree.
- Confirm duplicate words are stored only once.

**Remove Word:**
- Remove a word that’s a prefix (e.g., remove "app" while "apple" and "application" remain) and ensure autocomplete still returns the longer words.
- Remove a word and verify it no longer exists.
- Attempt to remove a word twice; ensure the second removal has no effect.
- Try removing a non-existent word to check structural integrity.

**Autocomplete:**
- Confirm a valid prefix returns the correct completions.
- Ensure an invalid prefix returns an empty list.