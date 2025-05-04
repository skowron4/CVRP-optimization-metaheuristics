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

        self._file_name_without_extension(output_filepath)
        
        plt.figure(figsize=(10, 6))
        plt.plot(iterations, best, label='Best neighbour', color='blue')
        plt.plot(iterations, worst, label='Worst neighbour', color='red')
        plt.plot(iterations, avg, label='Average neighbour', color='green')
        plt.plot(iterations, cumulative_best, label=f'Best (Overall: {best_overall})', color='purple')

        plt.xlabel('Iterations')
        plt.ylabel('Fitness')
        plt.title(f'Fitness Statistics Over Iterations')
        plt.legend()
        plt.savefig(output_filepath)
        plt.close()
