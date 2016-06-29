/**
Lightweight profiler library for c++
Copyright(C) 2016  Sergey Yagovtsev

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef PROFILER_READER____H
#define PROFILER_READER____H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <map>
#include "profiler/profiler.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace profiler {

    typedef uint32_t calls_number_t;

    struct BlockStatistics
    {
        ::profiler::timestamp_t               total_duration; ///< Summary duration of all block calls
        ::profiler::timestamp_t                 min_duration; ///< Cached block->duration() value. TODO: Remove this if memory consumption will be too high
        ::profiler::timestamp_t                 max_duration; ///< Cached block->duration() value. TODO: Remove this if memory consumption will be too high
        const ::profiler::SerilizedBlock* min_duration_block; ///< Will be used in GUI to jump to the block with min duration
        const ::profiler::SerilizedBlock* max_duration_block; ///< Will be used in GUI to jump to the block with max duration
        ::profiler::calls_number_t              calls_number; ///< Block calls number

        // TODO: It is better to replace SerilizedBlock* with BlocksTree*, but this requires to store pointers in children list.

        BlockStatistics()
            : total_duration(0)
            , min_duration(0)
            , max_duration(0)
            , min_duration_block(nullptr)
            , max_duration_block(nullptr)
            , calls_number(1)
        {
        }

        BlockStatistics(::profiler::timestamp_t _duration, const ::profiler::SerilizedBlock* _block)
            : total_duration(_duration)
            , min_duration(_duration)
            , max_duration(_duration)
            , min_duration_block(_block)
            , max_duration_block(_block)
            , calls_number(1)
        {
        }

        inline ::profiler::timestamp_t average_duration() const
        {
            return total_duration / calls_number;
        }

    }; // END of struct BlockStatistics.

    inline void release(BlockStatistics*& _stats)
    {
        if (!_stats)
        {
            return;
        }

        if (--_stats->calls_number == 0)
        {
            delete _stats;
        }

        _stats = nullptr;
    }

} // END of namespace profiler.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BlocksTree
{
    typedef ::std::list<BlocksTree> children_t;

    children_t                           children;
    ::profiler::SerilizedBlock*              node;
    ::profiler::BlockStatistics* frame_statistics; ///< Pointer to statistics for this block within the parent (may be nullptr for top-level blocks)
    ::profiler::BlockStatistics* total_statistics; ///< Pointer to statistics for this block within the bounds of all frames per current thread
    unsigned int            total_children_number; ///< Number of all children including number of grandchildren (and so on)

    BlocksTree() : node(nullptr), frame_statistics(nullptr), total_statistics(nullptr), total_children_number(0)
	{

    }

    BlocksTree(BlocksTree&& that) : BlocksTree()
    {
        makeMove(::std::forward<BlocksTree&&>(that));
    }

    BlocksTree& operator=(BlocksTree&& that)
    {
        makeMove(::std::forward<BlocksTree&&>(that));
        return *this;
    }

    ~BlocksTree()
    {
        if (node)
        {
            delete node;
        }

        release(total_statistics);
        release(frame_statistics);
    }

    bool operator < (const BlocksTree& other) const
    {
        if (!node || !other.node)
        {
            return false;
        }
        return node->block()->getBegin() < other.node->block()->getBegin();
    }

private:

    void makeMove(BlocksTree&& that)
    {
        if (node && node != that.node)
        {
            delete node;
        }

        if (total_statistics != that.total_statistics)
        {
            release(total_statistics);
        }

        if (frame_statistics != that.frame_statistics)
        {
            release(frame_statistics);
        }

        children = ::std::move(that.children);
        node = that.node;
        frame_statistics = that.frame_statistics;
        total_statistics = that.total_statistics;
        total_children_number = that.total_children_number;

        that.node = nullptr;
        that.frame_statistics = nullptr;
        that.total_statistics = nullptr;
    }

}; // END of struct BlocksTree.


typedef ::std::map<::profiler::thread_id_t, BlocksTree> thread_blocks_tree_t;

extern "C"{
    unsigned int PROFILER_API fillTreesFromFile(const char* filename, thread_blocks_tree_t& threaded_trees, bool gather_statistics = false);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PROFILER_READER____H
