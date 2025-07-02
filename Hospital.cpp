#include <QApplication>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTimer>
#include <queue>
#include <vector>
#include <random>
#include <functional>

struct Evento {
    QString tipo;
    double tiempo;

    Evento(QString tipo, double tiempo) : tipo(tipo), tiempo(tiempo) {}

    bool operator>(const Evento &other) const {
        return tiempo > other.tiempo;
    }
};

struct Paciente {
    static int contador;
    int id;
    Paciente() { id = ++contador; }
};
int Paciente::contador = 0;

class SimuladorHospital : public QWidget {
    Q_OBJECT

public:
    SimuladorHospital(QWidget *parent = nullptr) : QWidget(parent), servidorOcupado(false), reloj(0), pacientesAtendidos(0) {
        setWindowTitle("Simulación de Atención Hospitalaria (M/M/1)");
        resize(600, 450);

        salida = new QTextEdit(this);
        salida->setReadOnly(true);

        iniciarBtn = new QPushButton("Iniciar Simulación", this);
        unidadTiempoCombo = new QComboBox(this);
        unidadTiempoCombo->addItems({"segundos", "minutos", "horas"});

        QHBoxLayout *controles = new QHBoxLayout;
        controles->addWidget(new QLabel("Unidad de tiempo:"));
        controles->addWidget(unidadTiempoCombo);
        controles->addWidget(iniciarBtn);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(salida);
        layout->addLayout(controles);
        setLayout(layout);

        connect(iniciarBtn, &QPushButton::clicked, this, &SimuladorHospital::iniciarSimulacion);
    }

private slots:
    void iniciarSimulacion() {
        salida->clear();
        reloj = 0;
        pacientesAtendidos = 0;
        servidorOcupado = false;
        unidadTiempo = unidadTiempoCombo->currentText();
        while (!colaEventos.empty()) colaEventos.pop();
        std::queue<Paciente>().swap(colaPacientes);

        agregarEvento("llegada", reloj + generarExponencial(2.0));

        while (reloj < 100) {
            if (colaEventos.empty()) break;

            Evento evento = colaEventos.top();
            colaEventos.pop();
            reloj = evento.tiempo;
            procesarEvento(evento);
        }

        salida->append(QString("\nTotal pacientes atendidos: %1").arg(pacientesAtendidos));
    }

private:
    QTextEdit *salida;
    QPushButton *iniciarBtn;
    QComboBox *unidadTiempoCombo;
    std::priority_queue<Evento, std::vector<Evento>, std::greater<Evento>> colaEventos;
    std::queue<Paciente> colaPacientes;
    bool servidorOcupado;
    double reloj;
    int pacientesAtendidos;
    QString unidadTiempo;

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> dis{0.0, 1.0};

    double generarExponencial(double media) {
        return -media * std::log(1 - dis(gen));
    }

    void agregarEvento(const QString &tipo, double tiempo) {
        colaEventos.emplace(tipo, tiempo);
    }

    void procesarEvento(const Evento &evento) {
        salida->append(QString("Tiempo %.2f %1 - Evento: %2")
                           .arg(reloj, 0, 'f', 2)
                           .arg(unidadTiempo)
                           .arg(evento.tipo));

        if (evento.tipo == "llegada") {
            colaPacientes.push(Paciente());
            agregarEvento("llegada", reloj + generarExponencial(2.0));
            if (!servidorOcupado)
                iniciarAtencion();
        } else if (evento.tipo == "salida") {
            pacientesAtendidos++;
            servidorOcupado = false;
            if (!colaPacientes.empty())
                iniciarAtencion();
        }
    }

    void iniciarAtencion() {
        if (!colaPacientes.empty()) {
            colaPacientes.pop();
            servidorOcupado = true;
            agregarEvento("salida", reloj + generarExponencial(3.0));
        }
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SimuladorHospital ventana;
    ventana.show();
    return app.exec();
}
