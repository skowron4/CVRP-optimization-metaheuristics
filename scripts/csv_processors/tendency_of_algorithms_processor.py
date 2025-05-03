import os
import re
import pandas as pd
import matplotlib.pyplot as plt


class TendencyOfAlgorithmsProcessor:
    def __init__(self, folder_path: str, output_path: str):
        self.folder_path = folder_path
        self.output_path = output_path
        self.file_pattern = re.compile(r'([a-zA-Z]-[a-zA-Z]\d+-k\d+)')
        self.data = []

    def load_data(self):
        """Reads CSV files and computes average values for each optimization method."""
        for filename in os.listdir(self.folder_path):
            if filename.endswith('.csv'):
                match = self.file_pattern.search(filename)
                if not match:
                    continue
                instance_name = match.group(1)
                file_path = os.path.join(self.folder_path, filename)

                try:
                    df = pd.read_csv(file_path)
                    avg_ts = df['TS'].mean()
                    avg_sa = df['SA'].mean()
                    avg_htsa = df['HTSA'].mean()

                    self.data.append({
                        'Instance': instance_name,
                        'TS': avg_ts,
                        'SA': avg_sa,
                        'HTSA': avg_htsa
                    })
                except Exception as e:
                    print(f"Error processing file {filename}: {e}")

    def plot(self):
        """Generates and saves a trend plot of average method values per instance (with flipped axes)."""
        if not self.data:
            print("No data loaded. Make sure to run `load_data()` first.")
            return

        df = pd.DataFrame(self.data)
        df = df.sort_values('Instance')

        plt.figure(figsize=(10, 6))
        plt.plot(df['Instance'], df['TS'], 'o-', label='Tabu Search (TS)')
        plt.plot(df['Instance'], df['SA'], 's-', label='Simulated Annealing (SA)')
        plt.plot(df['Instance'], df['HTSA'], '^-', label='Hybrid TS+SA (HTSA)')

        plt.ylabel('Average Value')
        plt.xlabel('Instance')
        plt.title('Average Optimization Results per Instance')
        plt.xticks(rotation=90)
        plt.legend()
        plt.grid(True)
        plt.tight_layout()

        os.makedirs(self.output_path, exist_ok=True)
        output_file = os.path.join(self.output_path, 'tendency_chart.png')
        plt.savefig(output_file)
        plt.close()
        print(f"[INFO] Tendency chart saved to {output_file}")
