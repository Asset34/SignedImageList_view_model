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

    std::shared_ptr<QImage> getImageAt(int row, int column) const;
    const QString &getSignAt(int row, int column) const;

    QModelIndexList getSelectedIndexes() const;
    QModelIndex getSelectedIndex() const;

    std::shared_ptr<QImage> getSelectedImage() const;
    QString getSelectedSign() const;

public slots:
    void setColumnCount(int count);

    void addSignedImage(std::shared_ptr<QImage> image, const QString &sign = QString());
    void removeAt(int row, int column);
    void removeSelected();

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
