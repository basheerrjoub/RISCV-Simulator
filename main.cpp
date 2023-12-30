#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QString>
#include <QChar>
#include <vector>
#include <QRegularExpression>
#include <QTableWidget>

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
        updateRegisterTable();
    }

private:
    void setupUi() {
        QVBoxLayout *main_layout = new QVBoxLayout(this);
        QHBoxLayout *result_layout = new QHBoxLayout(this);

        instructionLineEdit = new QLineEdit(this);
        main_layout->addWidget(instructionLineEdit);

        executeButton = new QPushButton("Execute", this);
        executeButton->setDefault(true);

        connect(executeButton, &QPushButton::clicked, this, &RiscvSimulator::executeInstruction);
        main_layout->addWidget(executeButton);

        sideEffectsTextEdit = new QTextEdit(this);
        sideEffectsTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sideEffectsTextEdit->setMinimumHeight(100);
        result_layout->addWidget(sideEffectsTextEdit);


        registerTable = new QTableWidget(32, 2, this);
        registerTable->setHorizontalHeaderLabels({"Register", "Value"});
        result_layout->addWidget(registerTable);

        main_layout->addLayout(result_layout);

        setLayout(main_layout);
    }

    QString simulateInstruction(const QString &instruction) {
        QString sideEffects;

        // Split the instruction into components
        QRegularExpression rx("(\\,)");
        QStringList parts = instruction.simplified().split(rx);
        QStringList op_operand = parts[0].simplified().split(' ');


        if (parts.isEmpty() || parts.size() != 3 ||op_operand.isEmpty() || op_operand.size() != 2) {
            return  "Invalid instruction";
        }

        QString opcode = op_operand[0].trimmed();
        QString op1 = op_operand[1].trimmed();
        QString op2 = parts[1].trimmed();
        QString op3 = parts[2].trimmed();

        if (op1.at(0) != 'x' || op2.at(0) != 'x' || op3.at(0) != 'x')
            return  "Invalid register name";



        sideEffects += "OPCODE: " + opcode + "\n";
        sideEffects += "Operand1: " + op1 + "\n";
        sideEffects += "Operand2: " + op2 + "\n";
        sideEffects += "Operand3: " + op3 + "\n";


        // R-TYPE
        auto is_r_type = std::find(rv32i_r_type.begin(), rv32i_r_type.end(), opcode.toStdString());
        if ((is_r_type != rv32i_r_type.end()) && (op_operand.size() == 2 && parts.size() == 3)) {

            int rd = op1.remove(0,1).toInt();
            int rs1 = registers[op2.remove(0,1).toInt()];
            int rs2 = op3.remove(0,1).toInt();

            if (rd < 0 || rs1 < 0 || rs2 < 0 || rd > 31 || rs1 > 31 || rs2 > 31)
                return "Registers should be between 0 & 31";

            if (opcode == "add") {
                // Format: add rd, rs1, rs2

                registers[rd] = registers[rs1] + registers[rs2];

                sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
            } else if (opcode == "sub") {
                // Format: sub rd, rs1, rs2

                registers[rd] = registers[rs1] - registers[rs2];

                sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
            } else if (opcode == "sll") {
                // Format: sll rd, rs1, rs2

                registers[rd] = registers[rs1] << registers[rs2];

                sideEffects += "Register " + QString::number(rd) + " set to " + QString::number(registers[rd]) + "\n";
            }
        }else {
            return "Unsupported or invalid instruction";
        }

        return sideEffects;
    }

    void updateRegisterTable() {
        // Clear the existing content in the table
        registerTable->clearContents();

        // Display the state of all 32 registers in the table
        for (int i = 0; i < 32; ++i) {
            QTableWidgetItem *registerItem = new QTableWidgetItem("x" + QString::number(i));
            QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(registers[i]));

            registerTable->setItem(i, 0, registerItem);
            registerTable->setItem(i, 1, valueItem);
        }
    }

    QLineEdit *instructionLineEdit;
    QPushButton *executeButton;
    QTextEdit *sideEffectsTextEdit;
    QTableWidget *registerTable;

    // Static member for global registers
    static std::vector<int> registers;
    std::vector<std::string> rv32i_r_type = {"add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and"};
};

// Initialize the static member
std::vector<int> RiscvSimulator::registers(32, 0);

int main(int argc, char *argv[]) {
    QApplication  a(argc, argv);

    RiscvSimulator simulator;
    simulator.resize(1080, 720);
    simulator.show();

    return a.exec();
}
