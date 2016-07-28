// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"

static const int nCheckpointSpan = 10;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0        , hashGenesisBlock )
    		( 1,   uint256("0x00000575ae191098a01aa9611f07394ff863b9659344deabbb6afa3a91de1372") )
        ( 500,   uint256("0x7fe4368968d522f1fcb797fbc1cdfec18f89b1bfb1094927dae06178e92fd241") )
        ( 1000,   uint256("0x00000185915e5bcce9054b8c1b4672db895e1a6277d0240613e7b90c1feb20f9") )
        ( 2000,   uint256("0x00000186ccef0740b7201a0082aebdbf47389d72a2a7df6146bc98e27cf50110") )
        ( 4000,   uint256("0xfaf6a28e54662343513cc0fcfffb9160d09baf1d6894a7b751ef7fcbfbd00034") )
        ( 8000,   uint256("0x9a58fb283b5a4d3265df3ff775712a423d6789bf1f028ce5a861f290f4e235f3") )
        ( 16000,   uint256("0x3f82a6dbe4d79eee1824ddc8fb38e6b36b63b455464961f0d02d2947db0ee284") )
        ( 32000,   uint256("0x35741a97a7675bec2b28788609d349d7405a104d41768ce61b2e5f4cc2671292") )
        ( 64000,   uint256("0x1eec3f0fd8c0d8ebc18f89c2e1b7b91712ecc62dde92d0f63c3218987535611a") )
        ( 90000,   uint256("0x475411e9f960f2d3029ba05d13c8fd172ba7ec71e0e319c7ea886841c9cb6dbd") )
        ( 128000,   uint256("0xdb22ffdc75af412914c86caca9aec2470e1163f786b509a136e6b38d226ab1fe") )
        ( 150000,   uint256("0x1382b2ca27dd99d496efa6066798f88b8705715159ab12fcee5793864ff44ff3") )
        ( 175892,   uint256("0x3c92b5d9f79d00ee56bf7cf537f93fc4febe7abf51849c19ff7bf0fba73fd648") )
        ( 177252,   uint256("0x8fb767d03545a7cbe907c0cade8bb98e6622faecabe4e73335065c310b490d5b") )
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, hashGenesisBlockTestNet )

    ;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
