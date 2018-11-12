#include "signedimageitemdelegate.hpp"

#include <QPainter>
#include <QFontMetrics>
#include <QLineEdit>

SignedImageItemDelegate::SignedImageItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent),
      m_imageHeight(50),
      m_signHeight(25),
      m_offset(10),
      m_margin(10),
      m_padding(10),
      m_borderWidth(2),
      m_aspectRatio(Qt::KeepAspectRatio),
      m_alignment(Qt::AlignCenter),
      m_borderColor(Qt::black),
      m_backgroundColor(Qt::white),
      m_signColor(Qt::white),
      m_highlightColor(Qt::blue),
      m_font("Times", 10, QFont::Bold),
      m_isScaled(true)
{
}

QSize SignedImageItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.data().canConvert<QImage>()) {
        return QStyledItemDelegate::sizeHint(option, index);
    }

    QImage image = qvariant_cast<QImage>(index.data());

    // Compute sizes
    int aspectRatio = static_cast<double>(image.width() / image.height());
    int height = m_imageHeight + m_offset + m_signHeight + 2*m_padding;
    int width = aspectRatio * m_imageHeight + 2*m_padding;

    return QSize(width, height);
}

void SignedImageItemDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const
{
    QRect rect(option.rect);
    QRect copy;

    // Draw highlight
    if (option.state & QStyle::State_Selected) {
        drawHighlight(painter, rect);
    }

    // Draw main
    toMainRect(rect);
    drawMain(painter, rect);

    toContentRect(rect);
    copy = rect;

    // Draw image
    QImage image = qvariant_cast<QImage>(index.data(Qt::DisplayRole));
    toImageRect(rect);
    drawImage(painter, rect, image);

    // Draw sign
    QString sign = qvariant_cast<QString>(index.data(Qt::DecorationRole));
    toSignRect(copy);
    drawSign(painter, copy, sign);
}

QWidget *SignedImageItemDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &/*option*/,
        const QModelIndex &/*index*/
        ) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(true);
    editor->setAlignment(Qt::AlignCenter);

    return editor;
}

void SignedImageItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data(Qt::DecorationRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void SignedImageItemDelegate::setModelData(
        QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index
        ) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();

    model->setData(index, value, Qt::EditRole);
}

void SignedImageItemDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &/*index*/
        ) const
{
    QRect rect(option.rect);
    toMainRect(rect);
    toContentRect(rect);
    toSignRect(rect);

    editor->setGeometry(rect);
}

void SignedImageItemDelegate::setImageHeight(int height)
{
    m_imageHeight = height;
}

void SignedImageItemDelegate::setSignHeight(int height)
{
    m_signHeight = height;
}

void SignedImageItemDelegate::setOffset(int offset)
{
    m_offset = offset;
}

void SignedImageItemDelegate::setMargin(int margin)
{
    m_margin = margin;
}

void SignedImageItemDelegate::setPadding(int padding)
{
    m_padding = padding;
}

void SignedImageItemDelegate::setBorderWidth(int width)
{
    m_borderWidth = width;
}

void SignedImageItemDelegate::setAspectRatio(Qt::AspectRatioMode flag)
{
    m_aspectRatio = flag;
}

void SignedImageItemDelegate::setAlignment(Qt::Alignment flag)
{
    m_alignment = flag;
}

void SignedImageItemDelegate::setBorderColor(const QColor &color)
{
    m_borderColor = color;
}

void SignedImageItemDelegate::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
}

void SignedImageItemDelegate::setSignColor(const QColor &color)
{
    m_signColor = color;
}

void SignedImageItemDelegate::setHighlightColor(const QColor &color)
{
    m_highlightColor = color;
}

void SignedImageItemDelegate::setFont(const QFont &font)
{
    m_font = font;
}

void SignedImageItemDelegate::setScaleEnabled(bool enabled)
{
    m_isScaled = enabled;
}

void SignedImageItemDelegate::zoom(float factor)
{
    m_imageHeight *= factor;
    if (m_imageHeight < IMAGE_HEIGHT_MIN) {
        m_imageHeight = IMAGE_HEIGHT_MIN;
    }
}

void SignedImageItemDelegate::drawHighlight(QPainter *painter, const QRect &rect) const
{
    painter->save();

    painter->fillRect(rect, m_highlightColor);

    painter->restore();
}

void SignedImageItemDelegate::drawMain(QPainter *painter, const QRect &rect) const
{
    painter->save();

    painter->setPen(QPen(m_borderColor, m_borderWidth));
    painter->setBrush(QBrush(m_backgroundColor));
    painter->drawRect(rect);

    painter->restore();
}

void SignedImageItemDelegate::drawImage(QPainter *painter, const QRect &rect, const QImage &image) const
{
    painter->save();

    QImage scaledImage = scaleImage(rect, image);

    // Compute offsets
    int offseth = computeImageHOffset(rect, scaledImage);
    int offestv = computeImageVOffest(rect, scaledImage);

    // Draw
    painter->drawImage(
                rect.left() + offseth,
                rect.top() + offestv,
                scaledImage);

    painter->restore();
}

void SignedImageItemDelegate::drawSign(QPainter *painter, const QRect &rect, const QString &sign) const
{
    painter->save();

    // Fill background
    painter->setBrush(QBrush(m_signColor));
    painter->fillRect(rect, m_signColor);

    // Compute offsets
    int offseth = computeSignHOffest(rect, sign);
    int offsetv = computeSignVOffest(rect, sign);

    // Draw sign
    painter->setFont(m_font);
    painter->drawText(
        rect.bottomLeft().x() + offseth,
        rect.bottomLeft().y() - offsetv,
        sign
    );

    painter->restore();
}

QImage SignedImageItemDelegate::scaleImage(const QRect &rect, const QImage &image) const
{
    QImage scaledImage;

    if (m_isScaled) {
        scaledImage = image.scaled(rect.size(), m_aspectRatio);
    }
    else {
        double aspectRatio = static_cast<double>(image.width() / image.height());
        scaledImage = image.scaled(
                    m_imageHeight * aspectRatio,
                    m_imageHeight,
                    m_aspectRatio
                    );
    }

    return scaledImage;
}

int SignedImageItemDelegate::computeImageHOffset(const QRect &rect, const QImage &image) const
{
    if (m_alignment & Qt::AlignLeft) {
        return 0;
    }
    else if (m_alignment & Qt::AlignRight) {
        return rect.width() - image.width();
    }
    else if (m_alignment & Qt::AlignHCenter) {
        return (rect.width() - image.width()) / 2.0;
    }

    return 0;
}

int SignedImageItemDelegate::computeImageVOffest(const QRect &rect, const QImage &image) const
{
    if (m_alignment & Qt::AlignTop) {
        return 0;
    }
    else if (m_alignment & Qt::AlignBottom) {
        return rect.height() - image.height();
    }
    else if (m_alignment & Qt::AlignVCenter) {
        return (rect.height() - image.height()) / 2.0;
    }

    return 0;
}

int SignedImageItemDelegate::computeSignHOffest(const QRect &rect, const QString &sign) const
{
    QFontMetrics metrics(m_font);

    if (m_alignment & Qt::AlignLeft) {
        return 0;
    }
    else if (m_alignment & Qt::AlignRight) {
        return rect.width() - metrics.width(sign);
    }
    else if (m_alignment & Qt::AlignHCenter) {
        return (rect.width() - metrics.width(sign)) / 2.0;
    }

    return 0;
}

int SignedImageItemDelegate::computeSignVOffest(const QRect &rect, const QString &/*sign*/) const
{
    QFontMetrics metrics(m_font);

    if (m_alignment & Qt::AlignTop) {
        return 0;
    }
    else if (m_alignment & Qt::AlignBottom) {
        return rect.height() - metrics.height();
    }
    else if (m_alignment & Qt::AlignVCenter) {
        return (rect.height() - metrics.height()) / 2.0;
    }

    return 0;
}

void SignedImageItemDelegate::toMainRect(QRect &rect) const
{
    rect.setTop(rect.top() + m_margin);
    rect.setLeft(rect.left() + m_margin);
    rect.setBottom(rect.bottom() - m_margin);
    rect.setRight(rect.right() - m_margin);
}

void SignedImageItemDelegate::toContentRect(QRect &rect) const
{
    rect.setTop(rect.top() + m_borderWidth + m_padding);
    rect.setLeft(rect.left() + m_borderWidth + m_padding);
    rect.setBottom(rect.bottom() - m_borderWidth - m_padding);
    rect.setRight(rect.right() - m_borderWidth - m_padding);
}

void SignedImageItemDelegate::toImageRect(QRect &rect) const
{
    rect.setBottom(rect.bottom() - m_signHeight - m_offset);
}

void SignedImageItemDelegate::toSignRect(QRect &rect) const
{
    rect.setTop(rect.bottom() - m_signHeight);
}
