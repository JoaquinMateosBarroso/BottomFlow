import tkinter as tk
from tkinter import filedialog
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from datetime import datetime

class CSVPlotterApp:
    def __init__(self, master):
        self.master = master
        self.master.title("BottomFlow")

        self.file_path = str()
        self.df = None
        self.fig, self.ax = plt.subplots(figsize=(8, 4))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

        self.confidence_level = 0.95
        self.confidence_interval_label = tk.Label(master, text="Space for confidence interval")
        self.confidence_interval_label.pack(side=tk.BOTTOM, pady=10)

        self.load_button = tk.Button(master, text="Load CSV", command=self.load_csv)
        self.load_button.pack(side=tk.BOTTOM, pady=10)

        self.column_label = tk.Label(master, text="Select Column:")
        self.column_label.pack(side=tk.BOTTOM, pady=5)

        self.column_var = tk.StringVar()
        self.column_dropdown = tk.OptionMenu(master, self.column_var, "")
        self.column_dropdown.pack(side=tk.BOTTOM, pady=5)

        self.plot_button = tk.Button(master, text="Plot", command=self.plot)
        self.plot_button.pack(side=tk.BOTTOM, pady=10)

        self.hide_label_button = tk.Button(master, text="Hide legend", command=self.hide_label_plot)
        self.hide_label_button.pack(side=tk.BOTTOM, pady=10)

    def load_csv(self):
        self.file_path = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
        if self.file_path:
            self.df = pd.read_csv(self.file_path)
            columns = self.df.columns
            self.column_var.set(columns[0])  # Set default column
            menu = self.column_dropdown["menu"]
            menu.delete(0, "end")  # Clear previous options
            for column in columns:
                menu.add_command(label=column, command=tk._setit(self.column_var, column))
    
    def createLegend(self):
        PIDs = self.df["PID"].unique()
        if "Name" in self.df.columns:
            return [str(pid) + " " + str(self.df[self.df["PID"]==pid]["Name"].unique()[0]) for pid in PIDs]
        else:
            return [str(pid) for pid in PIDs]

    def plot(self, show_legend=True):
        if self.df is not None:
            self.ax.clear()
            for PID in self.df["PID"].unique():
                self.ax.plot(self.df[self.df["PID"] == PID]["Time"],
                             self.df[self.df["PID"] == PID][self.column_var.get()])
            if (show_legend):
                self.ax.legend(self.createLegend())

            self.ax.set_xlabel("Time(s)")
            self.ax.set_ylabel(self.column_var.get())
            self.ax.set_title(f"{self.column_var.get()} vs Time")
            self.canvas.draw()

            self.confidence_interval_label.config(text=self.getConfidenceIntervalText())

    def hide_label_plot(self):
        self.plot(False)

    
    def getConfidenceIntervalText(self) -> str:
        text = ""
        i = 1
        for PID in self.df["PID"].unique():
            if i > 10:
                break
            series = self.df[self.df["PID"] == PID][self.column_var.get()]
            mean = series.mean()
            std_dev = series.std()

            margin_of_error = stats.t.ppf((1 + self.confidence_level) / 2, len(series) - 1) * (std_dev / np.sqrt(len(series)))
            lower_bound = mean - margin_of_error
            upper_bound = mean + margin_of_error

            name = self.df[self.df["PID"]==PID]["Name"].unique()[0]
            text += f"Confidence interval for {name} ({self.confidence_level:.0%}): [{lower_bound:.3f}, {upper_bound:.3f}]\n"
            i += 1
        return text
                             


if __name__ == "__main__":
    root = tk.Tk()
    app = CSVPlotterApp(root)
    root.mainloop()