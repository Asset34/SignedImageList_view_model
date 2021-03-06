#ifndef SIGNEDIMAGELISTMODEL_HPP
#define SIGNEDIMAGELISTMODEL_HPP

#include <QAbstractListModel>
#include <QImage>
#include <QString>
#include <QVector>

#include <memory>

class SignedImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SignedImageListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    std::shared_ptr<QImage> getImageAt(int row) const;
    const QString &getSignAt(int row) const;

public slots:
    void addSignedImage(std::shared_ptr<QImage> image, const QString &sign = QString());
    void removeAt(int row);

private:
    struct SignedImage
    {
        std::shared_ptr<QImage> image;
        QString sign;
    };

    QVector<SignedImage> m_signedImages;

};

#endif // SIGNEDIMAGELISTMODEL_HPP
