#ifndef SIGNEDIMAGETABLEPROXYMODEL_HPP
#define SIGNEDIMAGETABLEPROXYMODEL_HPP

#include <QAbstractProxyModel>

#include "signedimagelistmodel.hpp"

class SignedImageTableProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    explicit SignedImageTableProxyModel(int columnCount, QObject *parent = nullptr);

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    QModelIndex index(
            int row,
            int column,
            const QModelIndex &parent = QModelIndex()
            ) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &proxyIndex, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int getColumnCount() const;

    std::shared_ptr<QImage> getImageAt(int row, int column) const;
    const QString &getSignAt(int row, int column) const;

public slots:
    void setColumnCount(int count);

    void addSignedImage(std::shared_ptr<QImage> image, const QString &sign = QString());
    void removeAt(int row, int column);

private:
    QModelIndex parent(const QModelIndex &child) const override;

    void createConnections();
    void destroyConnections();

    int toSoure(int row, int column) const;
    int toRow(int row) const;
    int toColumn(int row) const;

    int computeRowCount(int sourceRowCount) const;
    int computeColumnCount(int sourceRowCount) const;

    bool addRowNeeded() const;
    bool removeRowNeeded() const;
    bool addColumnNeeded() const;
    bool removeColumnNeeded() const;

    int m_columnCount;

    bool m_rowWasAdded;
    bool m_rowWasRemoved;
    bool m_columnWasAdded;
    bool m_columnWasRemoved;

    SignedImageListModel *m_model;

private slots:
    void onDataChanged(
            const QModelIndex &topLeft,
            const QModelIndex &bottomRight,
            const QVector<int> &roles = QVector<int>()
            );

    void onRowsAboutToBeInsedrted(const QModelIndex &parent, int start, int end);
    void onRowsInserted(const QModelIndex &parent, int first, int last);

    void onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
};

#endif // SIGNEDIMAGETABLEPROXYMODEL_HPP
