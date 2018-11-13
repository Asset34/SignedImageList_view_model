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
    SignedImageTableProxyModel *getProxyModel() const;

public slots:
    void setColumnCount(int count);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    constexpr static const float ZOOMUP_FACTOR = 1.1;
    constexpr static const float ZOOMDOWN_FACTOR = 0.9;

    SignedImageListModel *m_sourceModel;
    SignedImageTableProxyModel *m_proxyModel;
    SignedImageItemDelegate *m_delegate;

    int m_columnCount;
};

#endif // SIGNEDIMAGETABLEWIDGET_HPP
