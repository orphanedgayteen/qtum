#ifndef QTUM_QT_TOKENTRANSACTIONDESC_H
#define QTUM_QT_TOKENTRANSACTIONDESC_H

#include <QObject>
#include <QString>

class TokenTransactionRecord;

class CWallet;
class CTokenTx;

/** Provide a human-readable extended HTML description of a token transaction.
 */
class TokenTransactionDesc: public QObject
{
    Q_OBJECT

public:
    static QString toHTML(CWallet *wallet, CTokenTx &wtx, TokenTransactionRecord *rec, int unit);

private:
    TokenTransactionDesc() {}

    static QString FormatTxStatus(const CTokenTx& wtx);
};

#endif // QTUM_QT_TOKENTRANSACTIONDESC_H