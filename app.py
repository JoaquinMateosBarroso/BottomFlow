import tkinter as tk
from tkinter import filedialog
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from datetime import datetime

class CSVPlotterApp:
    def __init__(self, master):
        self.master = master
        self.master.title("BottomFlow")

        self.file_path = ""
        self.df = None
        self.fig, self.ax = plt.subplots(figsize=(8, 4))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

        self.load_button = tk.Button(master, text="Load CSV", command=self.load_csv)
        self.load_button.pack(side=tk.BOTTOM, pady=10)

        self.column_label = tk.Label(master, text="Select Column:")
        self.column_label.pack(side=tk.BOTTOM, pady=5)

        self.column_var = tk.StringVar()
        self.column_dropdown = tk.OptionMenu(master, self.column_var, "")
        self.column_dropdown.pack(side=tk.BOTTOM, pady=5)

        self.plot_button = tk.Button(master, text="Plot", command=self.plot)
        self.plot_button.pack(side=tk.BOTTOM, pady=10)

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
        return [str(pid) + " " + self.df[self.df["PID"]==pid]["Name"].unique()[0] for pid in PIDs]



    def plot(self):
        if self.df is not None:
            self.ax.clear()
            interval = self.getInterval()
            times = pd.Series([interval*i for i in range(len(self.df["Time"].unique()))])
            for PID in self.df["PID"].unique():
                times = pd.Series([interval*i for i in range(len(self.df[self.df["PID"] == PID]["Time"]))])
                self.ax.plot(times,
                             self.df[self.df["PID"] == PID][self.column_var.get()])

            self.ax.legend(self.createLegend())
            self.ax.set_xlabel("Time(s)")
            self.ax.set_ylabel(self.column_var.get())
            self.ax.set_title(f"{self.column_var.get()} vs Time")
            self.canvas.draw()

    def getInterval(self) -> int:
        date_format = "%Y-%m-%d %H:%M:%S"
        datetime1 = datetime.strptime(self.df["Time"].unique()[1], date_format)
        datetime2 = datetime.strptime(self.df["Time"].unique()[0], date_format)

        # Calculate the time difference in seconds
        return (datetime1 - datetime2).total_seconds()

if __name__ == "__main__":
    root = tk.Tk()
    app = CSVPlotterApp(root)
    root.mainloop()
