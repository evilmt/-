#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QFileIconProvider>
#include <QTextCodec>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void show_menu(const QPoint pos);
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_pushButton_5_clicked();
    void addItemContent(int row, int column, QString content);
    void on_pushButton_6_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    quint64 dirFileSize(const QString &path);
    void chakan(QString path,int encryption);
    void clickgoose();
    void clickmms();
    void clickdxx();
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();
    int TestAesEncryptFile(std::string in_file_path, std::string out_file_path,
                           const char *rc4_encrypt_key, int encrypt_chunk_size);
    int AESencryptfile(QString in_file_path, QString out_file_path,
                       const char *rc4_encrypt_key, int encrypt_chunk_size,int hang);
    int AesDecryptFile(QString in_file_path, QString out_file_path,
                           const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang);
    void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);

        void on_pushButton_clicked();

        void on_commandLinkButton_clicked();

        void on_commandLinkButton_2_clicked();

        void on_pushButton_2_clicked();
        int AESXencryptfile(QString in_file_path, QString out_file_path,
                           const char *rc4_encrypt_key, int encrypt_chunk_size,int hang);
        int AesXDecryptFile(QString in_file_path, QString out_file_path,
                               const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang);
        int AES_S_encryptfile(QString in_file_path, QString out_file_path,
                           const char *rc4_encrypt_key, int encrypt_chunk_size,int hang);
        int Aes_S_DecryptFile(QString in_file_path, QString out_file_path,
                               const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang);
        int AES_SS_encryptfile(QString in_file_path, QString out_file_path,
                           const char *rc4_encrypt_key, int encrypt_chunk_size,int hang);
        int Aes_SS_DecryptFile(QString in_file_path, QString out_file_path,
                               const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang);

        
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
