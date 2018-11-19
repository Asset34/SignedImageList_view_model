#include "signedimagetablewidget.hpp"

#include <QWheelEvent>
#include <QHeaderView>
#include <Qt>

#include <algorithm>

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

int SignedImageTableWidget::getCount() const
{
    return m_sourceModel->rowCount();
}

std::shared_ptr<QImage> SignedImageTableWidget::getImageAt(int row, int column) const
{
    return m_proxyModel->getImageAt(row, column);
}

const QString &SignedImageTableWidget::getSignAt(int row, int column) const
{
    return m_proxyModel->getSignAt(row, column);
}

QModelIndexList SignedImageTableWidget::getSelectedIndexes() const
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();

    std::sort(indexes.begin(), indexes.end(),
              [&indexes](
              const QModelIndex &index1,
              const QModelIndex &index2
              ) -> bool {
        return index2 < index1;
    });

    return indexes;
}

QModelIndex SignedImageTableWidget::getSelectedIndex() const
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();

    if (indexes.count() <= 0) {
        return QModelIndex();
    }

    return indexes.at(0);
}

std::shared_ptr<QImage> SignedImageTableWidget::getSelectedImage() const
{
    QModelIndex index = getSelectedIndex();

    if (!index.isValid()) {
        return nullptr;
    }

    return getImageAt(index.row(), index.column());
}

QString SignedImageTableWidget::getSelectedSign() const
{
    QModelIndex index = getSelectedIndex();

    if (!index.isValid()) {
        return QString();
    }

    return getSignAt(index.row(), index.column());
}

void SignedImageTableWidget::setColumnCount(int count)
{
    m_proxyModel->setColumnCount(count);

    setModel(nullptr);
    setModel(m_proxyModel);
}

void SignedImageTableWidget::addSignedImage(
        std::shared_ptr<QImage> image,
        const QString &sign
        )
{
    m_proxyModel->addSignedImage(image, sign);
}

void SignedImageTableWidget::removeAt(int row, int column)
{
    m_proxyModel->removeAt(row, column);
}

void SignedImageTableWidget::removeSelected()
{
    QModelIndexList indexes = getSelectedIndexes();

    for (QModelIndex index : indexes) {
        m_proxyModel->removeAt(index.row(), index.column());
    }

    selectionModel()->clear();
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
