#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QAbstractSocket>

#include "clistmodel.h"

class QTcpSocket;
class QTimer;

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(CListModel *model,
                          const QString &usuario,
                          const QString &senha,
                          QObject *parent = nullptr);

    Q_INVOKABLE bool receberModelo(const QString &host,
                                   const quint16 &port);
signals:
    void erroConexao();
    void erroTimeout();
    void erroRecebimento();
    void novoModelo(CListModel *);

private slots:
    void socketError(QAbstractSocket::SocketError);
    void connected();
    void disconnected();
    void bytesWritten(qint64);
    void readyRead();
    void timeoutOcurred();

private:
    QTcpSocket *m_soket;
    QTimer *m_timer;
    CListModel *m_buffer;
    QString m_usuario;
    QString m_senha;
    quint64 currentCol = 0;
    quint64 totalCol = 0;
    qint64 bytesEnviados = 0;
    const static int timeout;

    bool conectar(const QString &host, const quint16 &port);
    bool enviar(const QString msg);
    bool fecharConexao();
};

#endif // CLIENTSOCKET_H
