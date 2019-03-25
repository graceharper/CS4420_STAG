//
// Created by tejun on 3/23/2019.
//


#pragma once


#include "constants.h"
#include "naive_contiguous_mem_tuple_group.h"

#include <array>
#include <memory>

/**
 * Tuple groups are not stored contiguously.
 * Instead, they are created when needed (when a previous tuple group doesn't have space for a new tuple).
 *
 * On DDL operation (copy constructor):
 * - Locks the entire table
 * - Copies each tuple group into new tuple groups
 * - Does an atomic swap (across all tuple groups) after copying is done
 * - Unlocks the entire table
 */
template<int NumAttr>
class AuroraTable {

public:

    AuroraTable();

    ~AuroraTable() = default;

    template<int PrevNumAttr>
    AuroraTable(AuroraTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data);

    void startScan();

    std::array<int, NumAttr> &getNextTuple();

    /**
     * Must be used after a DDL operation
     */
    template<int PrevNumAttr>
    std::array<int, NumAttr> &getNextTuple(AuroraTable<PrevNumAttr> &toCopy);

    bool isFull() const;

    // Getters

    NaiveContiguousMemTupleGroup<NumAttr> *getTupleGroupAtIndex(int i);

    int getLastTupleGroupIndex() const;

    int getScanIndex() const;

protected:

    // Default-initialization of array
    std::array<std::unique_ptr<NaiveContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS> tuple_groups;

    int last_tuple_group_index = 0;

    int scan_index = 0;

    int to_copy_index = NUMBER_TUPLE_GROUPS + 1;

};

// Link to template implementation
#include "aurora_table.tpp"