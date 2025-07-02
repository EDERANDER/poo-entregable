import tkinter as tk
import math
import random
import heapq
from tkinter import scrolledtext
class Evento:
    def __init__(self, tipo, tiempo):
        self.tipo = tipo
        self.tiempo = tiempo
class Paciente:
    contador = 0

    def __init__(self):
        Paciente.contador += 1
        self.id = Paciente.contador
class SimuladorHospital(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Simulación de Atención Hospitalaria (M/M/1)")
        self.geometry("600x450")

        self.salida = scrolledtext.ScrolledText(self, wrap=tk.WORD)
        self.salida.pack(fill=tk.BOTH, expand=True)

        self.unidadTiempoCombo = tk.StringVar(value="minutos")
        combo = tk.OptionMenu(self, self.unidadTiempoCombo, "segundos", "minutos", "horas")
        combo.pack(side=tk.LEFT, padx=5)

        btnIniciar = tk.Button(self, text="Iniciar Simulación", command=self.iniciarSimulacion)
        btnIniciar.pack(side=tk.LEFT, padx=5)

        self.reloj = 0.0
        self.servidorOcupado = False
        self.pacientesAtendidos = 0
        self.colaEventos = []
        self.colaPacientes = []
        self.rand = random.Random()

    def iniciarSimulacion(self):
        self.salida.delete("1.0", tk.END)
        self.reloj = 0.0
        self.pacientesAtendidos = 0
        self.servidorOcupado = False
        self.colaEventos = []
        self.colaPacientes = []
        self.agregarEvento("llegada", self.reloj + self.generarExponencial(2.0))

        while self.reloj < 100:
            if not self.colaEventos:
                break
            tiempo, tipo = heapq.heappop(self.colaEventos)
            self.reloj = tiempo
            self.procesarEvento(tipo)

        self.salida.insert(tk.END, f"\nTotal pacientes atendidos: {self.pacientesAtendidos}\n")

    def procesarEvento(self, tipo):
        self.salida.insert(tk.END, f"Tiempo {self.reloj:.2f} {self.unidadTiempoCombo.get()} - Evento: {tipo}\n")
        if tipo == "llegada":
            p = Paciente()
            self.colaPacientes.append(p)
            self.agregarEvento("llegada", self.reloj + self.generarExponencial(2.0))
            if not self.servidorOcupado:
                self.iniciarAtencion()
        elif tipo == "salida":
            self.pacientesAtendidos += 1
            self.servidorOcupado = False
            if self.colaPacientes:
                self.iniciarAtencion()

    def iniciarAtencion(self):
        if self.colaPacientes:
            self.colaPacientes.pop(0)
            self.servidorOcupado = True
            self.agregarEvento("salida", self.reloj + self.generarExponencial(3.0))

    def generarExponencial(self, media):
        return -media * math.log(1 - self.rand.random())

    def agregarEvento(self, tipo, tiempo):
        heapq.heappush(self.colaEventos, (tiempo, tipo))

if __name__ == "__main__":
    app = SimuladorHospital()
    app.mainloop()