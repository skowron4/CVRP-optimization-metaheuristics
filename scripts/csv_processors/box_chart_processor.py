import matplotlib.pyplot as plt
from .csv_processor import CSVProcessor


class BoxChartProcessor(CSVProcessor):
    def _extract_data(self, df):
        data = {}
        for column in df.columns:
            data[column] = df[column].dropna().tolist()
        return data

    def _plot_chart(self, data, output_filepath):
        file_name = self._file_name_without_extension(output_filepath)

        plt.figure(figsize=(10, 6))
        plt.boxplot(data.values(), labels=data.keys())
        plt.xlabel('methods')
        plt.ylabel('Best Fitness')
        plt.title(f'Box Plot of Best Fitness Values for Each Method\n{file_name}')
        plt.savefig(output_filepath)
        plt.close()
