// Copyright (c) 2020 barrystyle (https://github.com/barrystyle)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "util.h"

class UpgradeManager;
extern UpgradeManager upgradeMan;

class UpgradeManager {

  private:

     int internalHeight{0};
     int forceDmnHeight{605000};

  public:

     void actionsAtBlockHeight(int nHeight);
     int revealInternalHeight();
     bool keepExistingClient(int nHeight);
     int minProtocolActive();
     bool isDIP0008Active();
};
