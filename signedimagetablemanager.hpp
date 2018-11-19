#ifndef SIGNEDIMAGETABLEMANAGER_HPP
#define SIGNEDIMAGETABLEMANAGER_HPP

#include <QWidget>

#include "signedimagelistmodel.hpp"

#include <memory>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class SignedImageTableWidget;

class SignedImageTableManager : public QWidget
{
    Q_OBJECT

public:
    explicit SignedImageTableManager(int columnCount, QWidget *parent = nullptr);

    SignedImageListModel *getModel() const;

    std::shared_ptr<QImage> getImageAt(int row, int column) const;
    const QString &getSignAt(int row, int column) const;

    std::shared_ptr<QImage> getSelectedImage() const;
    QString getSelectedSign() const;

public slots:
    void addSignedImage(std::shared_ptr<QImage> image, const QString &sign = QString());

private:
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_controlLayout;

    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    SignedImageTableWidget *m_tableWidget;

private slots:
    void add();
    void remove();

};

#endif // SIGNEDIMAGETABLEMANAGER_HPP
