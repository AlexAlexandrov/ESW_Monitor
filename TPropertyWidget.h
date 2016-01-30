#ifndef TPROPERTYWIDGET_H
#define TPROPERTYWIDGET_H

#include <QWidget>
#include <QtGui>
#include "MonitorScreen.h"

typedef enum{
    item_ComboBox = 0,
    item_LineEdit
}ItemType;

class TPropertyWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TPropertyWidget(QWidget *parent = 0);
    void    setActiveObject(QGraphicsItem* actObject = 0);
private:
    TGrIndexes colorIndex;
    QStringList headers;
    QStringList buttonPropertyList, EnterAreaPropertyList, BarPropertyList,
                TextPropertyList, ImagePropertyList, ShapePropertyList, AnimationPropertyList;
    QGraphicsItem*  activeObject;
    quint16         objectType;

    QListWidget* lwItemList;
    QDialog* w, *d;
    QDialog* addtextDialog;
    QLineEdit* leAddText;
    int     lwItemsCount, selectedTextNum;

    QPushButton* pbDelete, *pbNext, *pbPrevious;

    QLineEdit *leXY0, *leXY1, *leXY2, *leWidth, *leHeight;
    QLabel* labelWidth, *labelHeight;
    QList<QLineEdit*> leList;
    QComboBox *cbType;
    QGridLayout* gridlay;
    QVBoxLayout* sh_mainlay;


    //----------------------- Functions ---------------------
    void    showButtonProperty();
    void    showEnterAreaProperty();
    void    showBarProperty();
    void    showTextProperty();
    void    showImageProperty();
    void    showShapeProperty();
    void    showAnimationProperty();
    bool    changingAllowed;

    void    configTextList();
    void    configShapePath();
    void    configImageList();
    void    configColorList( TGrIndexes colorType );
    void    configAnimation();

public slots:
    void onItemDoubleClicked(QTableWidgetItem *item);
    void onItemChanged(QTableWidgetItem *item);
private slots:

    void onConfigTextAddClicked();
    void onConfigTextDeleteClicked();
    void onConfigTextDoubleClicked(QListWidgetItem *item);
    void onChangeTextOkClicked();
    void onAddTextOkClicked();
    void onConfigTextNextClicked();
    void onConfigTextPrevClicked();

    void onConfigShapeTypeChanged(int index);
    void onConfigShapeWidthChanged(QString str);
    void onConfigShapeHeightChanged(QString str);
    void onConfigShapePathPosChanged(QString str);

    void onConfigImageAddClicked();
    void onConfigImageDelClicked();
    void onConfigImageNextClicked();
    void onConfigImagePrevClicked();
    void onConfigImageDoubleClicked( QListWidgetItem *item );
    void onConfigImageItemClicked( QListWidgetItem *item );

    void onConfigColorAddClicked();
    void onConfigColorDelClicked();
    void onConfigColorNextClicked();
    void onConfigColorPrevClicked();
    void onConfigColorDoubleClicked( QListWidgetItem *item );
    void onConfigColorItemClicked( QListWidgetItem *item );

    void onConfigAnimeAddClicked();
    void onConfigAnimeDelClicked();
    void onConfigAnimeUpClicked();
    void onConfigAnimeDownClicked();
    void onConfigAnimeDoubleClicked(QListWidgetItem *item);
signals:
    void ZValueChanged();
};

typedef enum{
    but_name = 0,
    but_tooltipRow,
    but_posXRow,
    but_posYRow,
    but_posZRow,
    but_widthRow,
    but_heightRow,
    but_fontRow,
    but_buttonTextRow,
    but_backColorRow,
    but_bordColorRow,
    but_textColorRow
}TButtonPropNumbers;

typedef enum{
    text_name = 0,
    text_tooltipRow,
    text_posXRow,
    text_posYRow,
    text_posZRow,
    text_fontRow,
    text_textRow,
    text_textColorRow,
    text_textSizeRow
}TTextPropNumbers;

typedef enum{
    img_name = 0,
    img_tooltipRow,
    img_posXRow,
    img_posYRow,
    img_posZRow,
    img_widthRow,
    img_heightRow,
    img_pathRow
}TImagePropNumbers;

typedef enum{
    anime_name = 0,
    anime_tooltipRow,
    anime_posXRow,
    anime_posYRow,
    anime_posZRow,
    anime_widthRow,
    anime_heightRow,
    anime_frequencyRow,
    anime_repeatRow,
    anime_framesRow
}TAnimePropNumbers;

typedef enum{
    bar_name = 0,
    bar_tooltipRow,
    bar_posXRow,
    bar_posYRow,
    bar_posZRow,
    bar_widthRow,
    bar_heightRow,
    bar_backColorRow,
    bar_bordColorRow
}TBarPropNumbers;

typedef enum{
    ea_name = 0,
    ea_tooltipRow,
    ea_posXRow,
    ea_posYRow,
    ea_posZRow,
    ea_fontRow,
    ea_textRow,
    ea_widthRow,
    ea_heightRow,
    ea_backColorRow,
    ea_borderColorRow,
    ea_validColorRow,
    ea_invalidColorRow,
    ea_validatorMinRow,
    ea_validatorMaxRow
}TEnterAreaPropNumbers;

typedef enum{
    sh_nameRow = 0,
    sh_tooltipRow,
    sh_posXRow,
    sh_posYRow,
    sh_posZRow,
    sh_widthRow,
    sh_heightRow,
    sh_pathRow,
    sh_backColorRow,
    sh_borderColorRow
}TShapePropNumbers;

#endif // TPROPERTYWIDGET_H
