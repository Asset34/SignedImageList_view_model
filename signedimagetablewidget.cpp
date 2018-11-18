#include "signedimagetablewidget.hpp"

#include <QWheelEvent>
#include <QHeaderView>
#include <Qt>

SignedImageTableWidget::SignedImageTableWidget(int columnCount, QWidget *parent)
    : QTableView(parent)
{
    m_sourceModel = new SignedImageListModel;

    m_proxyModel = new SignedImageTableProxyModel(columnCount);
    m_proxyModel->setSourceModel(m_sourceModel);

    m_delegate = new SignedImageItemDelegate;
    m_delegate->setImageHeight(150);
    m_delegate->setSignHeight(25);
    m_delegate->setOffset(10);
    m_delegate->setMargin(20);
    m_delegate->setPadding(5);
    m_delegate->setBorderWidth(2);
    m_delegate->setAspectRatio(Qt::KeepAspectRatio);
    m_delegate->setAlignment(Qt::AlignCenter);
    m_delegate->setBorderColor(Qt::black);
    m_delegate->setBackgroundColor(Qt::white);
    m_delegate->setSignColor(Qt::lightGray);
    m_delegate->setHighlightColor(Qt::blue);
    m_delegate->setScaleEnabled(true);

    setModel(m_proxyModel);
    setItemDelegate(m_delegate);

    horizontalHeader()->hide();
    verticalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setShowGrid(false);
}

SignedImageListModel *SignedImageTableWidget::getSourceModel() const
{
    return m_sourceModel;
}

void SignedImageTableWidget::setColumnCount(int count)
{
    m_proxyModel->setColumnCount(count);

    setModel(nullptr);
    setModel(m_proxyModel);
}

const std::shared_ptr<QImage> &SignedImageTableWidget::getImageAt(int row, int column) const
{
    return m_proxyModel->getImageAt(row, column);
}

const QString &SignedImageTableWidget::getSignAt(int row, int column) const
{
    return m_proxyModel->getSignAt(row, column);
}

void SignedImageTableWidget::addSignedImage(
        const std::shared_ptr<QImage> &image,
        const QString &sign
        )
{
    m_proxyModel->addSignedImage(image, sign);
}

void SignedImageTableWidget::addImage(const std::shared_ptr<QImage> &image)
{
    m_proxyModel->addImage(image);
}

void SignedImageTableWidget::removeAt(int row, int column)
{
    m_proxyModel->removeAt(row, column);
}

void SignedImageTableWidget::removeAt(const QModelIndex &index)
{
    m_proxyModel->removeAt(index);
}

void SignedImageTableWidget::setImageAt(int row, int column, const std::shared_ptr<QImage> &image)
{
    m_proxyModel->setImageAt(row, column, image);
}

void SignedImageTableWidget::setSignAt(int row, int column, const QString &sign)
{
    m_proxyModel->setSignAt(row, column, sign);
}

void SignedImageTableWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->delta() > 0) {
            m_delegate->zoom(ZOOMUP_FACTOR);
        }
        else {
            m_delegate->zoom(ZOOMDOWN_FACTOR);
        }

        // Crutch to force repaint
        setItemDelegate(nullptr);
        setItemDelegate(m_delegate);
    }
    else {
        QTableView::wheelEvent(event);
    }
}
