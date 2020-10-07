// Copyright (c) 2020 barrystyle (https://github.com/barrystyle)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "util.h"

class COutPoint;
class UpgradeManager;

extern UpgradeManager upgradeMan;

class UpgradeManager {

  private:

     //! height registers
     int internalHeight{0};
     int forceDmnHeight{600000};

  public:

     void actionsAtBlockHeight(int nHeight);
     int revealInternalHeight();
     bool isDIP0008Active();
};
