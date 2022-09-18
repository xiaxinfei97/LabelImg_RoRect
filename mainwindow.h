#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_fileDir;//标注图片文件夹
    void ImportFile();//刷新待标注图片
    void SetPixmap();

    bool m_copy = false;//记录是否自动复制上一帧结果至本帧
private slots:
    void SaveXml();
    void Next();//下一张
    void Back();//上一张
};
#endif // MAINWINDOW_H
