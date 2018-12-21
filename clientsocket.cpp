#include "clientsocket.h"
#include "clistmodel.h"

#include <QTcpSocket>
#include <QTimer>

const int ClientSocket::timeout = 7000;

ClientSocket::ClientSocket(CListModel *model,
                           const QString &usuario,
                           const QString &senha,
                           QObject *parent)
    : QObject(parent)
    , m_soket(new QTcpSocket(this))
    , m_timer(new QTimer(this))
    , m_buffer(model)
    , m_usuario(usuario)
    , m_senha(senha)
{
    connect(m_soket, &QTcpSocket::connected,
            this, &ClientSocket::connected);
    connect(m_soket, &QTcpSocket::disconnected,
            this, &ClientSocket::disconnected);
    connect(m_soket, &QTcpSocket::bytesWritten,
            this, &ClientSocket::bytesWritten);
    connect(m_soket, &QTcpSocket::readyRead,
            this, &ClientSocket::readyRead);
    connect(m_timer, &QTimer::timeout,
            this, &ClientSocket::timeoutOcurred);
}

bool ClientSocket::receberModelo(const QString &host,
                                 const quint16 &port)
{
    if (!conectar(host, port)) {
        qDebug() << "<ClientSocket> falha na conexao";
        return false;
    }

    return enviar("login;" + m_usuario + ';' + m_senha);
}

bool ClientSocket::conectar(const QString &host, const quint16 &port)
{
    if (!m_soket) {
        qDebug() << "<ClientSocket> socket inexistente";
        return false;
    }

    if (!m_soket->isOpen()) {
        qDebug() << "<ClientSocket> Conectando...";
        m_soket->connectToHost(host, port);

        if(!m_soket->waitForConnected(timeout)) {
            qDebug() << "<ClientSocket> Error: " + m_soket->errorString();
            emit erroConexao();
            return false;
        }

        if (m_timer) {
            m_timer->start(timeout);
        }
    }

    return true;
}

bool ClientSocket::enviar(const QString msg)
{
    if (!(m_soket && m_soket->isOpen())) {
        qDebug() << "<ClientSocket> socket nao existente ou fechado";
        return false;
    }

    bytesEnviados = m_soket->write(msg.toUtf8());

    if (m_timer && !m_timer->isActive()) {
        m_timer->start(timeout);
    }

    return true;
}

bool ClientSocket::fecharConexao()
{
    if (m_soket && m_soket->isOpen()) {
        m_soket->close();
        qDebug() << "<ClientSocket> socket fechado com sucesso";

        return true;
    }

    qDebug() << "<ClientSocket> erro ao tentar fechar o socket";

    return false;
}

void ClientSocket::socketError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    qDebug() << "<ClientSocket> socket error: " << socketError;

    m_soket->close();
}

void ClientSocket::connected()
{
    qDebug() << "<ClientSocket> conectado";
}

void ClientSocket::disconnected()
{
    qDebug() << "<ClientSocket> desconectado";

    if (m_soket && m_soket->isOpen())
        m_soket->close();
}

void ClientSocket::bytesWritten(qint64 bytes)
{
    if (bytes == bytesEnviados) {
        qDebug() << "<ClientSocket> " << bytes << " bytes written...";
    } else {
        qDebug() << "<ClientSocket> falha no envio de" << bytes << " bytes";
    }

    bytesEnviados = 0;
}

void ClientSocket::readyRead()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }

    bool reenviar = false;
    QString rcv = QString(m_soket->readAll());
    qDebug() << "<ClientSocket> recebido " << rcv;

    if (rcv.size()) {
        QStringList rows = rcv.split('\n');

        if (rows.size()) {
            for (auto row : rows) {
                if (row.contains("finito")) {
                    reenviar = false;
                    fecharConexao();
                    emit novoModelo(m_buffer);
                } else {
                    QStringList aux = row.split(';');

                    if (aux.size()) {
                        int coluna = 0;
                        TableRow table;

                        for (auto i : aux) {
                            switch (coluna) {
                            case 0:
                                table.docentry = i;
                                break;
                            case 1:
                                table.StcCode = i;
                                break;
                            case 2:
                                table.porcent = i;
                                break;
                            case 3:
                                table.ItemCode = i;
                                break;
                            case 4:
                                table.State = i;
                                break;
                            case 5:
                                table.ItemName = i;
                                break;
                            default:
                                break;
                            }

                            ++coluna;
                        }

                        m_buffer->addData(table);
                    }

                    reenviar = true;
                }
            }
        }
    }

    if (reenviar) {
        reenviar = false;
        enviar("login;" + m_usuario + ';' + m_senha);
    }
}

void ClientSocket::timeoutOcurred()
{
    qDebug() << "<ClientSocket> timeout";

    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }

    if (m_soket && m_soket->isOpen()){
        m_soket->close();
        emit erroTimeout();
    }
}
