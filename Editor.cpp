#include <QApplication>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QStack>
#include <QHBoxLayout>

class EditorUndoRedo : public QWidget {
    Q_OBJECT

public:
    EditorUndoRedo(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Editor con Undo/Redo");
        resize(500, 400);

        textEdit = new QTextEdit(this);
        QPushButton *btnUndo = new QPushButton("Deshacer", this);
        QPushButton *btnRedo = new QPushButton("Rehacer", this);

        QHBoxLayout *botonesLayout = new QHBoxLayout;
        botonesLayout->addWidget(btnUndo);
        botonesLayout->addWidget(btnRedo);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(textEdit);
        mainLayout->addLayout(botonesLayout);
        setLayout(mainLayout);

        connect(textEdit, &QTextEdit::textChanged, this, &EditorUndoRedo::guardarEstado);
        connect(btnUndo, &QPushButton::clicked, this, &EditorUndoRedo::deshacer);
        connect(btnRedo, &QPushButton::clicked, this, &EditorUndoRedo::rehacer);

        guardarEstado(); // Guardar estado inicial
    }

private slots:
    void guardarEstado() {
        pilaUndo.push(textEdit->toPlainText());
        pilaRedo.clear();
    }

    void deshacer() {
        if (pilaUndo.size() > 1) {
            pilaRedo.push(pilaUndo.pop());
            textEdit->blockSignals(true);
            textEdit->setPlainText(pilaUndo.top());
            textEdit->blockSignals(false);
        }
    }

    void rehacer() {
        if (!pilaRedo.isEmpty()) {
            QString texto = pilaRedo.pop();
            pilaUndo.push(texto);
            textEdit->blockSignals(true);
            textEdit->setPlainText(texto);
            textEdit->blockSignals(false);
        }
    }

private:
    QTextEdit *textEdit;
    QStack<QString> pilaUndo;
    QStack<QString> pilaRedo;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    EditorUndoRedo editor;
    editor.show();
    return app.exec();
}
