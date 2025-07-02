package com.example.editor;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class SimuladorHospital extends JFrame {
    private JTextArea salida;
    private JButton iniciarBtn;
    private JComboBox<String> unidadTiempoCombo;
    private PriorityQueue<Evento> colaEventos;
    private Queue<Paciente> colaPacientes;
    private boolean servidorOcupado = false;
    private double reloj = 0;
    private int pacientesAtendidos = 0;
    private Random rand = new Random();
    private String unidadTiempo = "minutos";

    public SimuladorHospital() {
        super("Simulación de Atención Hospitalaria (M/M/1)");
        setSize(600, 450);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        salida = new JTextArea();
        salida.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(salida);

        iniciarBtn = new JButton("Iniciar Simulación");

        iniciarBtn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                unidadTiempo = (String) unidadTiempoCombo.getSelectedItem();
                iniciarSimulacion();
            }
        });

        unidadTiempoCombo = new JComboBox<>(new String[]{"segundos", "minutos", "horas"});
        JPanel controlPanel = new JPanel(new FlowLayout());
        controlPanel.add(new JLabel("Unidad de tiempo:"));
        controlPanel.add(unidadTiempoCombo);
        controlPanel.add(iniciarBtn);
        add(scrollPane, BorderLayout.CENTER);
        add(controlPanel, BorderLayout.SOUTH);
    }

    private void iniciarSimulacion() {
        salida.setText("");
        reloj = 0;
        pacientesAtendidos = 0;
        servidorOcupado = false;
        colaEventos = new PriorityQueue<>(Comparator.comparingDouble(e -> e.tiempo));
        colaPacientes = new LinkedList<>();
        colaEventos.add(new Evento("llegada", reloj + generarExponencial(2.0)));

        while (reloj < 100) {
            Evento evento = colaEventos.poll();
            if (evento == null) break;
            reloj = evento.tiempo;
            procesarEvento(evento);
        }

        salida.append("\nTotal pacientes atendidos: " + pacientesAtendidos);
    }

    private void procesarEvento(Evento evento) {
        salida.append(String.format("Tiempo %.2f %s - Evento: %s\n", reloj, unidadTiempo, evento.tipo));

        if (evento.tipo.equals("llegada")) {
            Paciente p = new Paciente();
            colaPacientes.add(p);

            colaEventos.add(new Evento("llegada", reloj + generarExponencial(2.0)));

            if (!servidorOcupado) {
                iniciarAtencion();
            }
        } else if (evento.tipo.equals("salida")) {
            pacientesAtendidos++;
            servidorOcupado = false;
            if (!colaPacientes.isEmpty()) {
                iniciarAtencion();
            }
        }
    }

    private void iniciarAtencion() {
        if (!colaPacientes.isEmpty()) {
            Paciente p = colaPacientes.poll();
            servidorOcupado = true;
            colaEventos.add(new Evento("salida", reloj + generarExponencial(3.0)));
        }
    }

    private double generarExponencial(double media) {
        return -media * Math.log(1 - rand.nextDouble());
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new SimuladorHospital().setVisible(true);
        });
    }

    class Evento {
        private String tipo;
        private double tiempo;

        Evento(String tipo, double tiempo) {
            this.tipo = tipo;
            this.tiempo = tiempo;
        }
    }

    class Paciente {
        static int contador = 0;
        int id;

        Paciente() {
            id = ++contador;
        }
    }
}

