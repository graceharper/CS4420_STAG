/*
** Created by tejun on 3/18/2019.
***********************************
** This header file defines the interface for naive tuple groups. 
** Stores all tuples in contiguous memory. Memory is pre-allocated at compile time.
*/


#include "includes/tuple/db_tuple.h"
#include "version.h"
#include "constants.h"

#include <array>

#pragma once


template<int NumAttr>
class VersionedContiguousMemTupleGroup {

public:

    VersionedContiguousMemTupleGroup() = default;

    ~VersionedContiguousMemTupleGroup() = default;

    /**
     * 
    *
    * On DDL operation (copy constructor):
    * - Copies each tuple into new tuples
    * - Does an atomic swap (across all tuples) after copying is done
    */
    template<int PrevNumAttr>
    VersionedContiguousMemTupleGroup(VersionedContiguousMemTupleGroup<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data);

    void startScan();

    std::array<int, NumAttr> &getNextTuple();

    bool isFull() const;

    // Getters

    DbTuple<NumAttr> &getTupleAtIndex(int i);

    int getLastTupleIndex() const;

    int getScanIndex() const;

    Version getVersion() const;

    void setVersion(Version v);

protected:

    // Default-initialization of array
    std::array<DbTuple<NumAttr>, NUMBER_TUPLES_PER_GROUP> tuples;

    int last_tuple_index = 0;

    int scan_index = 0;

    Version version = Version::PRE_DDL;

};

// Link to template implementation
#include "tuple_group/versioned_contiguous_mem_tuple_group.tpp"

