#ifndef SIGNEDIMAGELISTMODEL_HPP
#define SIGNEDIMAGELISTMODEL_HPP

#include <QAbstractListModel>
#include <QImage>
#include <QString>
#include <QVector>

class SignedImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SignedImageListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    const QImage &getImageAt(int row) const;
    const QString &getSignAt(int row) const;

public slots:
    void addSignedImage(const QImage &image, const QString &sign);
    void addImage(const QImage &image);

    void removeAt(int row);
    void removeAt(const QModelIndex &index);

    void setImageAt(int row, const QImage &image);
    void setSignAt(int row, const QString &sign);

    void updateRow(int row);

private:
    struct SignedImage
    {
        QImage image;
        QString sign;
    };

    QVector<SignedImage> m_signedImages;

};

#endif // SIGNEDIMAGELISTMODEL_HPP
