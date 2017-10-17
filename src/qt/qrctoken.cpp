#include "qrctoken.h"
#include "ui_qrctoken.h"

#include <QPainter>
#include <QAbstractItemDelegate>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QActionGroup>

#define DECORATION_SIZE 54
#define SYMBOL_WIDTH 80
#define MARGIN 5

class TokenViewDelegate : public QAbstractItemDelegate
{
public:
    enum DataRole{
        AddressRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        SymbolRole = Qt::UserRole + 3,
        DecimalsRole = Qt::UserRole + 4,
        BalanceRole = Qt::UserRole + 5,
    };

    TokenViewDelegate(QObject *parent) :
        QAbstractItemDelegate(parent)
    {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {
        painter->save();

        QIcon tokenIcon;
        QString tokenSybol = index.data(SymbolRole).toString();
        QString tokenBalance = index.data(BalanceRole).toString();

        QRect mainRect = option.rect;
        mainRect.setWidth(option.rect.width());
        QColor rowColor = index.row() % 2 ? QColor("#ededed") : QColor("#e3e3e3");
        painter->fillRect(mainRect, rowColor);

        bool selected = option.state & QStyle::State_Selected;
        if(selected)
        {
            painter->fillRect(mainRect,QColor("#cbcbcb"));
        }

        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        tokenIcon.paint(painter, decorationRect);

        QColor foreground = option.palette.color(QPalette::Text);
        painter->setPen(foreground);
        QRect tokenSymbolRect(decorationRect.right() + MARGIN, mainRect.top(), SYMBOL_WIDTH, DECORATION_SIZE);
        painter->drawText(tokenSymbolRect, Qt::AlignLeft|Qt::AlignVCenter, tokenSybol);

        int amountWidth = (mainRect.width() - decorationRect.width() - 2 * MARGIN - tokenSymbolRect.width());
        QRect tokenBalanceRect(tokenSymbolRect.right(), mainRect.top(), amountWidth, DECORATION_SIZE);
        painter->drawText(tokenBalanceRect, Qt::AlignRight|Qt::AlignVCenter, tokenBalance);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }
};

QRCToken::QRCToken(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCToken),
    m_model(0),
    m_tokenDelegate(0),
    m_tokenModel(0)
{
    ui->setupUi(this);

    m_sendTokenPage = new SendTokenPage(this);
    m_receiveTokenPage = new ReceiveTokenPage(this);
    m_addTokenPage = new AddTokenPage(this);
    m_tokenDelegate = new TokenViewDelegate(this);
    m_tokenModel = new QStandardItemModel(this);

    ui->stackedWidget->addWidget(m_sendTokenPage);
    ui->stackedWidget->addWidget(m_receiveTokenPage);
    ui->stackedWidget->addWidget(m_addTokenPage);

    ui->tokensList->setItemDelegate(m_tokenDelegate);
    ui->tokensList->setModel(m_tokenModel);

    QActionGroup *actionGroup = new QActionGroup(this);
    m_sendAction = new QAction(tr("Send"), actionGroup);
    m_receiveAction = new QAction(tr("Receive"), actionGroup);
    m_addTokenAction = new QAction(tr("AddToken"), actionGroup);
    actionGroup->setExclusive(true);

    m_sendAction->setCheckable(true);
    m_receiveAction->setCheckable(true);
    m_addTokenAction->setCheckable(true);

    ui->sendButton->setDefaultAction(m_sendAction);
    ui->receiveButton->setDefaultAction(m_receiveAction);
    ui->addTokenButton->setDefaultAction(m_addTokenAction);

    connect(m_addTokenPage, SIGNAL(on_addNewToken(QString,QString,QString,int,double)),this, SLOT(on_addToken(QString,QString,QString,int,double)));
    connect(m_sendAction, SIGNAL(triggered()), this, SLOT(on_goToSendTokenPage()));
    connect(m_receiveAction, SIGNAL(triggered()), this, SLOT(on_goToReceiveTokenPage()));
    connect(m_addTokenAction, SIGNAL(triggered()), this, SLOT(on_goToAddTokenPage()));

    on_goToSendTokenPage();
}

QRCToken::~QRCToken()
{
    delete ui;
}

void QRCToken::setModel(WalletModel *_model)
{
    m_model = _model;
}

void QRCToken::on_goToSendTokenPage()
{
    m_sendAction->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void QRCToken::on_goToReceiveTokenPage()
{
    m_receiveAction->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);
}

void QRCToken::on_goToAddTokenPage()
{
    m_addTokenAction->setChecked(true);
    ui->stackedWidget->setCurrentIndex(2);
}

void QRCToken::on_addToken(QString _address, QString _name, QString _symbol, int _decimals, double _balance)
{
    QStandardItem *item = new QStandardItem();

    item->setData(_address, TokenViewDelegate::AddressRole);
    item->setData(_name, TokenViewDelegate::NameRole);
    item->setData(_symbol, TokenViewDelegate::SymbolRole);
    item->setData(_decimals, TokenViewDelegate::DecimalsRole);
    item->setData(_balance, TokenViewDelegate::BalanceRole);

    m_tokenModel->appendRow(item);
}
