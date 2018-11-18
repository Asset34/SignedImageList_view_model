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

    const std::shared_ptr<QImage> &getImageAt(int row) const;
    const QString &getSignAt(int row) const;

public slots:
    void addSignedImage(const std::shared_ptr<QImage> &image, const QString &sign);
    void addImage(const std::shared_ptr<QImage> &image);

    void removeAt(int row);
    void removeAt(const QModelIndex &index);

    void setImageAt(int row, const std::shared_ptr<QImage> &image);
    void setSignAt(int row, const QString &sign);

private:
    struct SignedImage
    {
        std::shared_ptr<QImage> image;
        QString sign;
    };

    QVector<SignedImage> m_signedImages;

};

#endif // SIGNEDIMAGELISTMODEL_HPP
