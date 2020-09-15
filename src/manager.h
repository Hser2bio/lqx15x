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
     int dualCollatHeight{600000};
     int dualCollatLength{15000};
     int skipAllChecks{599999};

     //! event based toggles
     bool gbt_allowed{false};

     //! mode/phase based
     int phase_dualcollat{0};

  public:

     void actionsAtBlockHeight(int nHeight);
     int revealInternalHeight();
     bool isDIP0008Active();
     bool isGbtAllowed();
     void collateralPhase();
     bool isValidCollateral(const COutPoint& collateral, std::string &strError, const char* caller = __builtin_FUNCTION());
};
