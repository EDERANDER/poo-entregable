import tkinter as tk
from tkinter import scrolledtext

class EditorUndoRedo:
    def __init__(self, root):
        self.root = root
        self.root.title("Editor con Undo/Redo")
        self.root.geometry("500x400")

        # Frame para los botones (parte inferior)
        button_frame = tk.Frame(root)
        button_frame.pack(side=tk.BOTTOM, fill=tk.X, padx=5, pady=5)

        # Botón Deshacer (Undo)
        btn_undo = tk.Button(button_frame, text="Deshacer", command=self.undo)
        btn_undo.pack(side=tk.LEFT, padx=5)

        # Botón Rehacer (Redo)
        btn_redo = tk.Button(button_frame, text="Rehacer", command=self.redo)
        btn_redo.pack(side=tk.LEFT, padx=5)

        # Área de texto con scroll (ocupa el resto del espacio)
        self.text_area = scrolledtext.ScrolledText(root, wrap=tk.WORD)
        self.text_area.pack(fill=tk.BOTH, expand=True)

        # Pilas para Undo y Redo
        self.pila_undo = []
        self.pila_redo = []

        # Guardar estado inicial
        self.save_state()

        # Evento para guardar cambios al soltar una tecla
        self.text_area.bind("<KeyRelease>", lambda e: self.save_state())

    def save_state(self, event=None):
        """Guarda el estado actual en la pila Undo y limpia Redo"""
        current_text = self.text_area.get("1.0", tk.END)
        if not self.pila_undo or self.pila_undo[-1] != current_text:  # Evita duplicados
            self.pila_undo.append(current_text)
            self.pila_redo.clear()  # Limpia Redo al hacer un nuevo cambio

    def undo(self):
        """Deshace el último cambio"""
        if len(self.pila_undo) > 1:  # Siempre deja al menos un estado
            # Guarda el estado actual en Redo
            self.pila_redo.append(self.pila_undo.pop())
            # Recupera el estado anterior
            estado_anterior = self.pila_undo[-1]
            self.text_area.delete("1.0", tk.END)
            self.text_area.insert("1.0", estado_anterior)

    def redo(self):
        """Rehace un cambio deshecho"""
        if self.pila_redo:
            # Guarda el estado actual en Undo
            self.pila_undo.append(self.text_area.get("1.0", tk.END))
            # Recupera el último estado de Redo
            estado_recuperado = self.pila_redo.pop()
            self.text_area.delete("1.0", tk.END)
            self.text_area.insert("1.0", estado_recuperado)

if __name__ == "__main__":
    root = tk.Tk()
    app = EditorUndoRedo(root)
    root.mainloop()