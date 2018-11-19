#include "signedimagetablemanager.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <QFileDialog>

#include <memory>

#include "signedimagetablewidget.hpp"

SignedImageTableManager::SignedImageTableManager(int columnCount, QWidget *parent)
    : QWidget(parent)
{
    // Create buttons
    m_addButton = new QPushButton("Add");
    m_removeButton = new QPushButton("Delete");

    // Create control layout
    m_controlLayout = new QVBoxLayout;
    m_controlLayout->setContentsMargins(0, 0, 0, 0);
    m_controlLayout->addWidget(m_addButton);
    m_controlLayout->addWidget(m_removeButton);
    m_controlLayout->addStretch(1);

    // Create signed image table widget
    m_tableWidget = new SignedImageTableWidget(columnCount);

    // Create main layout
    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addLayout(m_controlLayout);
    m_mainLayout->addWidget(m_tableWidget);

    // Create widget
    setLayout(m_mainLayout);
    setContentsMargins(5, 5, 5, 5);

    // Create connections
    connect(m_addButton, &QPushButton::clicked, this, &SignedImageTableManager::add);
    connect(m_removeButton, &QPushButton::clicked, this, &SignedImageTableManager::remove);
}

std::shared_ptr<QImage> SignedImageTableManager::getImageAt(int row, int column) const
{
    return m_tableWidget->getImageAt(row, column);
}

const QString &SignedImageTableManager::getSignAt(int row, int column) const
{
    return m_tableWidget->getSignAt(row, column);
}

std::shared_ptr<QImage> SignedImageTableManager::getSelectedImage() const
{
    return m_tableWidget->getSelectedImage();
}

QString SignedImageTableManager::getSelectedSign() const
{
    return m_tableWidget->getSelectedSign();
}

void SignedImageTableManager::addSignedImage(
        std::shared_ptr<QImage> image,
        const QString &sign
        )
{
    m_tableWidget->addSignedImage(image, sign);
}

void SignedImageTableManager::add()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Image"),
                "",
                tr("Image files (*.png *.jpg *jpeg *.png *.bmp)")
                );

    if (!path.isNull()) {
        QImage image(path);

        addSignedImage(std::make_shared<QImage>(image));
    }
}

void SignedImageTableManager::remove()
{
    m_tableWidget->removeSelected();
}
