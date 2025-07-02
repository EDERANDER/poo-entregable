package com.example.editor;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Stack;

public class EditorUndoRedo extends JFrame {
    private JTextArea textArea;
    private Stack<String> pilaUndo = new Stack<>();
    private Stack<String> pilaRedo = new Stack<>();

    public EditorUndoRedo() {
        setTitle("Editor con Undo/Redo");
        setSize(500, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        textArea = new JTextArea();
        JScrollPane scroll = new JScrollPane(textArea);

        JButton btnUndo = new JButton("Deshacer");
        JButton btnRedo = new JButton("Rehacer");

        JPanel panelBotones = new JPanel();
        panelBotones.add(btnUndo);
        panelBotones.add(btnRedo);

        add(scroll, BorderLayout.CENTER);
        add(panelBotones, BorderLayout.SOUTH);

        // Guardar el estado en cada tecla presionada
        textArea.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                pilaUndo.push(textArea.getText());
                pilaRedo.clear();
            }
        });

        btnUndo.addActionListener(e -> {
            if (!pilaUndo.isEmpty()) {
                pilaRedo.push(textArea.getText());
                String estadoAnterior = pilaUndo.pop();
                textArea.setText(estadoAnterior);
            }
        });

        btnRedo.addActionListener(e -> {
            if (!pilaRedo.isEmpty()) {
                pilaUndo.push(textArea.getText());
                String estadoRecuperado = pilaRedo.pop();
                textArea.setText(estadoRecuperado);
            }
        });
    }
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new EditorUndoRedo().setVisible(true));
    }
}