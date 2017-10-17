#include "receivetokenpage.h"
#include "ui_receivetokenpage.h"

#include "guiutil.h"
#include "guiconstants.h"
#include "receiverequestdialog.h"

ReceiveTokenPage::ReceiveTokenPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiveTokenPage)
{
    ui->setupUi(this);
    ui->lineEditAmount->setStyleSheet(STYLE_UNDERLINE);
    connect(ui->copyAddressButton, SIGNAL(clicked()), this, SLOT(on_copyAddressClicked()));
}

ReceiveTokenPage::~ReceiveTokenPage()
{
    delete ui;
}

void ReceiveTokenPage::setAddress(QString address)
{
    m_address = address;
    createQRCode();
}

void ReceiveTokenPage::on_copyAddressClicked()
{
    if(!m_address.isEmpty())
        GUIUtil::setClipboard(m_address);
}

void ReceiveTokenPage::createQRCode()
{
    SendCoinsRecipient info;
    info.address = m_address;
    if(ReceiveRequestDialog::createQRCode(ui->lblQRCode, info))
    {
        ui->lblQRCode->setScaledContents(true);
    }
}
