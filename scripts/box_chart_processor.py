import matplotlib.pyplot as plt
from csv_processor import CSVProcessor


class BoxChartProcessor(CSVProcessor):
    def _extract_data(self, df):
        data = {}
        for column in df.columns:
            data[column] = df[column].dropna().tolist()
        return data

    def _plot_chart(self, data, output_filepath):
        plt.figure(figsize=(10, 6))
        plt.boxplot(data.values(), labels=data.keys())
        plt.xlabel('Methods')
        plt.ylabel('Best Fitness')
        plt.title('Box Plot of Best Fitness Values for Each Method')
        plt.savefig(output_filepath)
        plt.close()
