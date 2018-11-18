#include "signedimagelistmodel.hpp"

SignedImageListModel::SignedImageListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int SignedImageListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_signedImages.count();
}

QVariant SignedImageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
         index.row() >= m_signedImages.size()) {
        return QVariant();
    }

    switch (role)
    {
        case Qt::DisplayRole: {
            return *m_signedImages.at(index.row()).image;
        }
        case Qt::DecorationRole: {
            return m_signedImages.at(index.row()).sign;
        }
        default: {
            return QVariant();
        }
    }
}

bool SignedImageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_signedImages[index.row()].sign = value.toString();

        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

Qt::ItemFlags SignedImageListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

const std::shared_ptr<QImage> &SignedImageListModel::getImageAt(int row) const
{
    return m_signedImages.at(row).image;
}

const QString &SignedImageListModel::getSignAt(int row) const
{
    return m_signedImages.at(row).sign;
}

void SignedImageListModel::addSignedImage(
        const std::shared_ptr<QImage> &image,
        const QString &sign
        )
{
    beginInsertRows(QModelIndex(), m_signedImages.count(), m_signedImages.count());

    m_signedImages.push_back({image, sign});

    endInsertRows();
}

void SignedImageListModel::addImage(const std::shared_ptr<QImage> &image)
{
    addSignedImage(image, QString());
}

void SignedImageListModel::removeAt(int row)
{
    beginRemoveRows(QModelIndex(), row, row);

    m_signedImages.removeAt(row);

    endRemoveRows();
}

void SignedImageListModel::removeAt(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    removeAt(index.row());
}

void SignedImageListModel::setImageAt(int row, const std::shared_ptr<QImage> &image)
{
    m_signedImages[row].image = image;
}

void SignedImageListModel::setSignAt(int row, const QString &sign)
{
    m_signedImages[row].sign = sign;
}
