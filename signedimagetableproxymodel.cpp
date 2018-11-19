#include "signedimagetableproxymodel.hpp"

SignedImageTableProxyModel::SignedImageTableProxyModel(int columnCount, QObject *parent)
    : QAbstractProxyModel(parent),
      m_columnCount(columnCount),
      m_rowWasAdded(false),
      m_rowWasRemoved(false),
      m_columnWasAdded(false),
      m_columnWasRemoved(false),
      m_model(nullptr)
{
}

void SignedImageTableProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    SignedImageListModel *model = dynamic_cast<SignedImageListModel*>(sourceModel);
    if (!model) {
        return;
    }

    beginResetModel();
    destroyConnections();

    m_model = model;
    QAbstractProxyModel::setSourceModel(sourceModel);

    createConnections();
    endResetModel();
}

QModelIndex SignedImageTableProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid()) {
        return QModelIndex();
    }

    int row = toRow(sourceIndex.row());
    int column = toColumn(sourceIndex.row());

    return createIndex(row, column, sourceIndex.internalPointer());
}

QModelIndex SignedImageTableProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid()) {
        return QModelIndex();
    }

    int row = toSoure(proxyIndex.row(), proxyIndex.column());

    return createIndex(row, 0, proxyIndex.internalPointer());
}

QModelIndex SignedImageTableProxyModel::index(
        int row,
        int column,
        const QModelIndex &/*parent*/
        ) const
{
    int sourceRow = toSoure(row, column);
    QModelIndex sourceIndex = m_model->index(sourceRow);

    return mapFromSource(sourceIndex);
}

int SignedImageTableProxyModel::rowCount(const QModelIndex &/*parent*/) const
{
    return computeRowCount(m_model->rowCount());
}

int SignedImageTableProxyModel::columnCount(const QModelIndex &/*parent*/) const
{
    return computeColumnCount(m_model->rowCount());
}

QVariant SignedImageTableProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    QModelIndex sourceIndex = mapToSource(proxyIndex);

    return m_model->data(sourceIndex, role);
}

bool SignedImageTableProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex sourceIndex = mapToSource(index);

    return m_model->setData(sourceIndex, value, role);
}

Qt::ItemFlags SignedImageTableProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    QModelIndex sourceIndex = mapToSource(index);

    return sourceModel()->flags(sourceIndex) |
           Qt::ItemIsEnabled |
           Qt::ItemIsSelectable;
}

int SignedImageTableProxyModel::getColumnCount() const
{
    return m_columnCount;
}

std::shared_ptr<QImage> SignedImageTableProxyModel::getImageAt(int row, int column) const
{
    int sourceRow = toSoure(row, column);

    return m_model->getImageAt(sourceRow);
}

const QString &SignedImageTableProxyModel::getSignAt(int row, int column) const
{
    int sourceRow = toSoure(row, column);

    return m_model->getSignAt(sourceRow);
}

void SignedImageTableProxyModel::setColumnCount(int count)
{
    m_columnCount = count;
}

void SignedImageTableProxyModel::addSignedImage(std::shared_ptr<QImage> image,
        const QString &sign
        )
{
    m_model->addSignedImage(image, sign);
}

void SignedImageTableProxyModel::removeAt(int row, int column)
{
    int sourceRow = toSoure(row, column);

    m_model->removeAt(sourceRow);
}

QModelIndex SignedImageTableProxyModel::parent(const QModelIndex &/*child*/) const
{
    return QModelIndex();
}

void SignedImageTableProxyModel::createConnections()
{
    if (m_model) {
        connect(
            m_model,
            &SignedImageListModel::dataChanged,
            this,
            &SignedImageTableProxyModel::onDataChanged
            );
        connect(
            m_model,
            &SignedImageListModel::rowsAboutToBeInserted,
            this,
            &SignedImageTableProxyModel::onRowsAboutToBeInsedrted
            );
        connect(
            m_model,
            &SignedImageListModel::rowsInserted,
            this,
            &SignedImageTableProxyModel::onRowsInserted
            );
        connect(
            m_model,
            &SignedImageListModel::rowsAboutToBeRemoved,
            this,
            &SignedImageTableProxyModel::onRowsAboutToBeRemoved
            );
        connect(
            m_model,
            &SignedImageListModel::rowsRemoved,
            this,
            &SignedImageTableProxyModel::onRowsRemoved
            );
    }
}

void SignedImageTableProxyModel::destroyConnections()
{
    if (m_model) {
        disconnect(
            m_model,
            &SignedImageListModel::dataChanged,
            this,
            &SignedImageTableProxyModel::onDataChanged
            );
        disconnect(
            m_model,
            &SignedImageListModel::rowsAboutToBeInserted,
            this,
            &SignedImageTableProxyModel::onRowsAboutToBeInsedrted
            );
        disconnect(
            m_model,
            &SignedImageListModel::rowsInserted,
            this,
            &SignedImageTableProxyModel::onRowsInserted
            );
        disconnect(
            m_model,
            &SignedImageListModel::rowsAboutToBeRemoved,
            this,
            &SignedImageTableProxyModel::onRowsAboutToBeRemoved
            );
        disconnect(
            m_model,
            &SignedImageListModel::rowsRemoved,
            this,
            &SignedImageTableProxyModel::onRowsRemoved
            );
    }
}

int SignedImageTableProxyModel::toSoure(int row, int column) const
{
    return row * m_columnCount + column;
}

int SignedImageTableProxyModel::toRow(int row) const
{
    return row / m_columnCount;
}

int SignedImageTableProxyModel::toColumn(int row) const
{
    return row % m_columnCount;
}

int SignedImageTableProxyModel::computeRowCount(int sourceRowCount) const
{
    if (sourceRowCount < 0) {
        return 0;
    }

    // Compute number of full rows
    int fullRowCount = sourceRowCount / m_columnCount;

    // Compute number of patrial rows
    int restColumnCount = sourceRowCount % m_columnCount;
    int partialRowCount = 0;
    if (restColumnCount) {
        partialRowCount = 1;
    }

    return fullRowCount + partialRowCount;
}

int SignedImageTableProxyModel::computeColumnCount(int sourceRowCount) const
{
    if (sourceRowCount < 0) {
        return 0;
    }

    return (sourceRowCount < m_columnCount) ? sourceRowCount : m_columnCount;
}

bool SignedImageTableProxyModel::addRowNeeded() const
{
    int curRowCount = computeRowCount(m_model->rowCount());
    int newRowCount = computeRowCount(m_model->rowCount() + 1);

    return (newRowCount > curRowCount) ? true : false;
}

bool SignedImageTableProxyModel::removeRowNeeded() const
{
    int curRowCount = computeRowCount(m_model->rowCount());
    int newRowCount = computeRowCount(m_model->rowCount() - 1);

    return (newRowCount < curRowCount) ? true : false;
}

bool SignedImageTableProxyModel::addColumnNeeded() const
{
    int curColumnCount = computeColumnCount(m_model->rowCount());
    int newColumnCount = computeColumnCount(m_model->rowCount() + 1);

    return (newColumnCount > curColumnCount) ? true : false;
}

bool SignedImageTableProxyModel::removeColumnNeeded() const
{
    int curColumnCount = computeColumnCount(m_model->rowCount());
    int newColumnCount = computeColumnCount(m_model->rowCount() - 1);

    return (newColumnCount < curColumnCount) ? true : false;
}

void SignedImageTableProxyModel::onDataChanged(
        const QModelIndex &topLeft,
        const QModelIndex &bottomRight,
        const QVector<int> &roles
        )
{
    QModelIndex proxyTopLeft = mapFromSource(topLeft);
    QModelIndex proxyRightBottom = mapFromSource(bottomRight);

    emit dataChanged(proxyTopLeft, proxyRightBottom, roles);
}

void SignedImageTableProxyModel::onRowsAboutToBeInsedrted(
        const QModelIndex &parent,
        int start,
        int /*end*/
        )
{
    // Check for row
    if (addRowNeeded()) {
        int row = toRow(start);
        beginInsertRows(parent, row, row);

        m_rowWasAdded = true;
    }

    // Check for column
    if (addColumnNeeded()) {
        int column = toColumn(start);
        beginInsertColumns(parent, column, column);

        m_columnWasAdded = true;
    }
}

void SignedImageTableProxyModel::onRowsInserted(
        const QModelIndex &/*parent*/,
        int first,
        int /*last*/
        )
{
    // Check for row was added
    if (m_rowWasAdded) {
        endInsertRows();

        m_rowWasAdded = false;
    }

    // Check for column was added
    if (m_columnWasAdded) {
        endInsertColumns();

        m_columnWasAdded = false;
    }

    // Update
    QModelIndex sourceIndex = m_model->index(first, 0);
    QModelIndex proxyIndex = mapFromSource(sourceIndex);
    emit dataChanged(proxyIndex, proxyIndex);
}

void SignedImageTableProxyModel::onRowsAboutToBeRemoved(
        const QModelIndex &parent,
        int start,
        int /*end*/
        )
{
    // Check for row
    if (removeRowNeeded()) {
        int row = toRow(start);
        beginRemoveRows(parent, row, row);

        m_rowWasRemoved = true;
    }

    // Check for column
    if (removeColumnNeeded()) {
        int column = toColumn(start);
        beginRemoveColumns(parent, column, column);

        m_columnWasRemoved = true;
    }
}

void SignedImageTableProxyModel::onRowsRemoved(
        const QModelIndex &/*parent*/,
        int first,
        int /*last*/
        )
{
    // Check for row was removed
    if (m_rowWasRemoved) {
        endRemoveRows();
        m_rowWasRemoved = false;
    }

    // Check for column was removed
    if (m_columnWasRemoved) {
        endRemoveColumns();
        m_columnWasRemoved = false;
    }

    // Update
    QModelIndex sourceIndex = m_model->index(first, 0);
    QModelIndex proxyIndex = mapFromSource(sourceIndex);
    emit dataChanged(proxyIndex, proxyIndex);
}
