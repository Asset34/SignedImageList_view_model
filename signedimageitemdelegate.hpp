#ifndef SIGNEDIMAGEITEMDELEGATE_HPP
#define SIGNEDIMAGEITEMDELEGATE_HPP

#include <QStyledItemDelegate>
#include <QColor>
#include <QFont>

class SignedImageItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SignedImageItemDelegate(QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            )const override;

    QWidget *createEditor(
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(
            QWidget *editor,
            QAbstractItemModel *model,
            const QModelIndex &index
            ) const override;

    void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

public slots:
    void setImageHeight(int height);
    void setSignHeight(int height);
    void setOffset(int offset);

    void setMargin(int margin);
    void setPadding(int padding);
    void setBorderWidth(int width);

    void setAspectRatio(Qt::AspectRatioMode flag);
    void setAlignment(Qt::Alignment flag);

    void setBorderColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setSignColor(const QColor &color);
    void setHighlightColor(const QColor &color);

    void setFont(const QFont &font);

    void setScaleEnabled(bool enabled);

    void zoom(float factor);

private:
    void drawHighlight(QPainter *painter, const QRect &rect) const;
    void drawMain(QPainter *painter, const QRect &rect) const;
    void drawImage(QPainter *painter, const QRect &rect, const QImage &image) const;
    void drawSign(QPainter *painter, const QRect &rect, const QString &sign) const;

    QImage scaleImage(const QRect &rect, const QImage &image) const;

    int computeImageHOffset(const QRect &rect, const QImage &image) const;
    int computeImageVOffest(const QRect &rect, const QImage &image) const;

    int computeSignHOffest(const QRect &rect, const QString &sign) const;
    int computeSignVOffest(const QRect &rect, const QString &sign) const;

    void toMainRect(QRect &rect) const;
    void toContentRect(QRect &rect) const;
    void toImageRect(QRect &rect) const;
    void toSignRect(QRect &rect) const;

    constexpr static const int IMAGE_HEIGHT_MIN = 10;

    int m_imageHeight;
    int m_signHeight;
    int m_offset;

    int m_margin;
    int m_padding;
    int m_borderWidth;

    Qt::AspectRatioMode m_aspectRatio;
    Qt::Alignment m_alignment;

    QColor m_borderColor;
    QColor m_backgroundColor;
    QColor m_signColor;
    QColor m_highlightColor;

    QFont m_font;

    bool m_isScaled;

};

#endif // SIGNEDIMAGEITEMDELEGATE_HPP
