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
    LogPrintf("UpgradeManager::%s - height %d\n", __func__, internalHeight);
}

int UpgradeManager::revealInternalHeight() {
    return internalHeight;
}

int UpgradeManager::minProtocolActive() {
    if (internalHeight >= forceDmnHeight)
        return 70220;
    return 70219;
}

bool UpgradeManager::keepExistingClient(int nVersion) {
    int currentVersion = this->minProtocolActive();
    if (internalHeight >= forceDmnHeight)
        if (nVersion < currentVersion)
            return false;
    return true;
}

bool UpgradeManager::isDIP0008Active() {
    if (internalHeight >= forceDmnHeight)
        return true;
    return false;
}
