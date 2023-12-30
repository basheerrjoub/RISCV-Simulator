#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QCoreApplication>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <vector>

class RiscvSimulator : public QWidget {

public:
    RiscvSimulator(QWidget *parent = nullptr) : QWidget(parent) {
        setupUi();
    }

private slots:
    void executeInstruction() {
        QString instruction = instructionLineEdit->text().trimmed().toLower();
        QString sideEffects = simulateInstruction(instruction);
        sideEffectsTextEdit->setText(sideEffects);
    }

private:
    void setupUi() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        instructionLineEdit = new QLineEdit(this);
        layout->addWidget(instructionLineEdit);

        executeButton = new QPushButton("Execute", this);
        connect(executeButton, &QPushButton::clicked, this, &RiscvSimulator::executeInstruction);
        layout->addWidget(executeButton);

        sideEffectsTextEdit = new QTextEdit(this);
        layout->addWidget(sideEffectsTextEdit);

        setLayout(layout);
    }

    QString simulateInstruction(const QString &instruction) {
        QString sideEffects;

        // Initialize 32 registers with zero values
        std::vector<int> registers(32, 0);

        // Split the instruction into components
        QStringList parts = instruction.split(' ');

        if (parts.isEmpty()) {
            return "Invalid instruction";
        }

        QString opcode = parts[0];

        // Process different instructions
        if (opcode == "add" && parts.size() == 4) {
            // Format: add rd, rs1, rs2
            int rd = parts[1].toInt();
            int rs1 = registers[parts[2].toInt()];
            int rs2 = registers[parts[3].toInt()];

            registers[rd] = rs1 + rs2;

            sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
        } else if (opcode == "sub" && parts.size() == 4) {
            // Format: sub rd, rs1, rs2
            int rd = parts[1].toInt();
            int rs1 = registers[parts[2].toInt()];
            int rs2 = registers[parts[3].toInt()];

            registers[rd] = rs1 - rs2;

            sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
        } else if (opcode == "mul" && parts.size() == 4) {
            // Format: mul rd, rs1, rs2
            int rd = parts[1].toInt();
            int rs1 = registers[parts[2].toInt()];
            int rs2 = registers[parts[3].toInt()];

            registers[rd] = rs1 * rs2;

            sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
        } else if (opcode == "addi" && parts.size() == 4) {
            // Format: addi rd rs1 immediate
            int rd = parts[1].toInt();
            int rs1 = registers[parts[2].toInt()];
            int immediate = parts[3].toInt();

            registers[rd] = rs1 + immediate;

            sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
        } else {
            return "Unsupported or invalid instruction";
        }

        // Display the state of all 32 registers
        sideEffects += "\nRegister State:\n";
        for (int i = 0; i < 32; ++i) {
            sideEffects += "x" + QString::number(i) + ": " + QString::number(registers[i]) + "\n";
        }

        return sideEffects;
    }

    QLineEdit *instructionLineEdit;
    QPushButton *executeButton;
    QTextEdit *sideEffectsTextEdit;
};

int main(int argc, char *argv[]) {
    QApplication  a(argc, argv);

    RiscvSimulator simulator;
    simulator.show();

    return a.exec();
}
