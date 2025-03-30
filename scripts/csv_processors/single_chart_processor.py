import os
import matplotlib.pyplot as plt
from .csv_processor import CSVProcessor


class SingleChartProcessor(CSVProcessor):
    def _extract_data(self, df):
        iterations = range(len(df))
        best, worst, avg = df['best'], df['worst'], df['avg']
        return iterations, best, worst, avg

    def _plot_chart(self, data, output_filepath):
        iterations, best, worst, avg = data

        best_overall = best.min()
        cumulative_best = best.cummin()

        # Extract file name without extension
        file_name = os.path.splitext(os.path.basename(output_filepath))[0]
        
        plt.figure(figsize=(10, 6))
        plt.plot(iterations, best, label=f'Best (Overall: {best_overall})', color='blue')
        plt.plot(iterations, worst, label='Worst', color='red')
        plt.plot(iterations, avg, label='Average', color='green')
        plt.plot(iterations, cumulative_best, label='Best Currently Found', color='purple')

        plt.xlabel('Iterations')
        plt.ylabel('Fitness')
        plt.title(f'Fitness Statistics Over Iterations\n{file_name}')
        plt.legend()
        plt.savefig(output_filepath)
        plt.close()
