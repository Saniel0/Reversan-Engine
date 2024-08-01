#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>
#include <unordered_map>

#define NOT_FOUND 1111

/**
 * @brief Class representing a transposition table for storing game states.
 * 
 * The Transposition_table class is used to store and retrieve game states
 * identified by their unique hash and alpha-beta values. It uses an internal
 * unordered map to maintain the mapping from hash values to their corresponding
 * entries.
 */
class Transposition_table {
    private:
        /**
         * @brief Structure representing an entry in the transposition table.
         * 
         * The Entry struct holds the score and type information for a given
         * game state.
         */
        struct Entry {
            int score; /**< The score associated with the game state. */
            int type; /**< The type of the entry (exact, lower bound, upper bound). */
        };

        std::unordered_map<uint64_t, Entry> map; /**< The internal map storing hash-entry pairs. */

    public:
        /**
         * @brief Clears the transposition table.
         * 
         * This method removes all entries from the transposition table.
         */
        void clear();

        /**
         * @brief Inserts a new entry into the transposition table.
         * 
         * @param hash The unique hash value identifying the game state.
         * @param score The score associated with the game state.
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * 
         * This method inserts a new entry into the transposition table with
         * the given score and alpha-beta values.
         */
        void insert(uint64_t hash, int score, int alpha, int beta);

        /**
         * @brief Retrieves an entry from the transposition table.
         * 
         * @param hash The unique hash value identifying the game state.
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * @return int The score associated with the game state, or NOT_FOUND if the entry is not found.
         * 
         * This method retrieves the score of the game state identified by the
         * given hash value. If the entry is not found, it returns NOT_FOUND.
         */
        int get(uint64_t hash, int alpha, int beta);
};

#endif
