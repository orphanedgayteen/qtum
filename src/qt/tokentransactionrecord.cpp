#include "tokentransactionrecord.h"

#include "base58.h"
#include "consensus/consensus.h"
#include "validation.h"
#include "timedata.h"
#include "wallet/wallet.h"

#include <stdint.h>

#include <boost/foreach.hpp>

/*
 * Decompose CWallet transaction to model transaction records.
 */
QList<TokenTransactionRecord> TokenTransactionRecord::decomposeTransaction(const CWallet *wallet, const CTokenTx &wtx)
{
    // Initialize variables
    QList<TokenTransactionRecord> parts;
    uint256 credit;
    uint256 debit;
    std::string tokenSymbol;
    uint8_t decimals = 18;
    if(wallet && wallet->GetTokenTxDetails(wtx, credit, debit, tokenSymbol, decimals))
    {
        // Get token transaction data
        TokenTransactionRecord rec;
        rec.time = wtx.nTime;
        rec.credit = dev::u2s(uintTou256(credit));
        rec.debit = -dev::u2s(uintTou256(debit));
        rec.hash = wtx.transactionHash;
        rec.tokenSymbol = tokenSymbol;
        rec.decimals = decimals;
        dev::s256 net = rec.credit + rec.debit;

        // Determine type
        if(wtx.strSenderAddress == wtx.strReceiverAddress)
        {
            rec.type = SendToSelf;
        }
        else
        {
            if(net > 0)
                rec.type = RecvWithAddress;

            if(net < 0)
                rec.type = SendToAddress;
        }

        // Set address
        switch (rec.type) {
        case SendToAddress:
        case SendToOther:
        case SendToSelf:
            rec.address = wtx.strSenderAddress;
            break;
        case RecvWithAddress:
        case RecvFromOther:
            rec.address = wtx.strReceiverAddress;
        default:
            break;
        }

        // Append record
        if(rec.type != Other)
            parts.append(rec);
    }

    return parts;
}

void TokenTransactionRecord::updateStatus(const CTokenTx &wtx)
{
    AssertLockHeld(cs_main);
    // Determine transaction status
}

bool TokenTransactionRecord::statusUpdateNeeded()
{
    AssertLockHeld(cs_main);
    return status.cur_num_blocks != chainActive.Height();
}

QString TokenTransactionRecord::getTxID() const
{
    return QString::fromStdString(hash.ToString());
}
