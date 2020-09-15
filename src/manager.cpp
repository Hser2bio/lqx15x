// Copyright (c) 2020 barrystyle (https://github.com/barrystyle)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "manager.h"

#include "chain.h"
#include "chainparams.h"
#include "coins.h"
#include "masternode/masternode-sync.h"
#include "util.h"
#include "validation.h"

UpgradeManager upgradeMan;

void UpgradeManager::actionsAtBlockHeight(int nHeight) {
    internalHeight = nHeight;
    this->collateralPhase();
    LogPrintf("UpgradeManager::%s - height %d\n", __func__, internalHeight);
}

int UpgradeManager::revealInternalHeight() {
    return internalHeight;
}

bool UpgradeManager::isDIP0008Active() {
    if (internalHeight >= forceDmnHeight)
        return true;
    return false;
}

bool UpgradeManager::isGbtAllowed() {
    //! we've been given the ok
    if (gbt_allowed)
        return true;
    //! standard rules apply here
    if (!masternodeSync.IsSynced() || !masternodeSync.IsBlockchainSynced() || masternodeSync.IsFailed())
         return gbt_allowed;
    //! check if height is ok
    if (internalHeight >= forceDmnHeight)
        gbt_allowed = true;
    return gbt_allowed;
}

void UpgradeManager::collateralPhase() {
    if (internalHeight < dualCollatHeight)
        phase_dualcollat = 0;
    if (internalHeight >= dualCollatHeight && internalHeight <= dualCollatHeight + dualCollatLength)
        phase_dualcollat = 1;
    if (internalHeight > dualCollatHeight + dualCollatLength)
        phase_dualcollat = 2;
}

bool UpgradeManager::isValidCollateral(const COutPoint& collateral, std::string &strError, const char* caller) {

    if (revealInternalHeight() < skipAllChecks) {
        LogPrintf("%s - bypassing checks until height %s\n", __func__, skipAllChecks);
        return true;
    }

    LogPrintf("UpgradeManager::%s - called by %s\n", __func__, caller);

    //! does it exist
    Coin coin;
    if (!GetUTXOCoin(collateral, coin)) {
        strError = "referenced collateral does not exist";
        return false;
    }

    //! has it been spent
    if (coin.IsSpent()) {
        strError = "referenced collateral has been spent";
        return false;
    }

    //! does it have sufficient confirms
    if (GetUTXOConfirmations(collateral) < Params().GetConsensus().nMasternodeMinimumConfirmations / 2) {
        strError = "collateral has less than half of required confirms";
        return false;
    }

    //! is it correct amount (height context)
    bool isThousand = (coin.out.nValue == 1000 * COIN);
    bool isTenThousand = (coin.out.nValue == 10000 * COIN);
    if (phase_dualcollat == 0 && !isThousand) {
        strError = "early-phase and isnt 1000 * COIN";
        return false;
    }
    if (phase_dualcollat == 1 && !isThousand && !isTenThousand) {
        strError = "mid-phase and isnt 1000 * COIN or 10000 * COIN";
        return false;
    }
    if (phase_dualcollat == 2 && !isTenThousand) {
        strError = "end-phase and isnt 10000 * COIN";
        return false;
    }

    return true;
}   
