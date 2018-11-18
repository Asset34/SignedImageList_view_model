#ifndef SIGNEDIMAGETABLEWIDGET_HPP
#define SIGNEDIMAGETABLEWIDGET_HPP

#include <QTableView>

#include "signedimagelistmodel.hpp"
#include "signedimagetableproxymodel.hpp"
#include "signedimageitemdelegate.hpp"

class SignedImageTableWidget : public QTableView
{
public:
    explicit SignedImageTableWidget(int columnCount, QWidget *parent = nullptr);

    SignedImageListModel *getSourceModel() const;

public slots:
    void setColumnCount(int count);

    const std::shared_ptr<QImage> &getImageAt(int row, int column) const;
    const QString &getSignAt(int row, int column) const;

    void addSignedImage(const std::shared_ptr<QImage> &image, const QString &sign);
    void addImage(const std::shared_ptr<QImage> &image);

    void removeAt(int row, int column);
    void removeAt(const QModelIndex &index);

    void setImageAt(int row, int column, const std::shared_ptr<QImage> &image);
    void setSignAt(int row, int column, const QString &sign);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    constexpr static const float ZOOMUP_FACTOR = 1.1;
    constexpr static const float ZOOMDOWN_FACTOR = 0.9;

    SignedImageListModel *m_sourceModel;
    SignedImageTableProxyModel *m_proxyModel;
    SignedImageItemDelegate *m_delegate;
};

#endif // SIGNEDIMAGETABLEWIDGET_HPP
