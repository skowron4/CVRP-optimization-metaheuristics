import os
from concurrent.futures import ProcessPoolExecutor
import pandas as pd
import logging

logging.basicConfig(level=logging.INFO, format='%(levelname)s - %(message)s')


class CSVProcessor:
    def __init__(self, input_directory, output_directory):
        self.input_directory = input_directory
        self.output_directory = output_directory

    def process_csv_files(self):
        if not os.path.exists(self.input_directory):
            logging.error(f"Input directory {self.input_directory} does not exist.")
            return

        os.makedirs(self.output_directory, exist_ok=True)
        self._traverse_directory()

    def _traverse_directory(self):
        with ProcessPoolExecutor() as executor:
            futures = []
            for filename in os.listdir(self.input_directory):
                if filename.endswith('.csv'):
                    output_filepath = os.path.join(self.output_directory, os.path.splitext(filename)[0] + '.png')

                    if os.path.exists(output_filepath):
                        logging.info(f"Skipping {filename} as {output_filepath} already exists.")
                    else:
                        input_filepath = os.path.join(self.input_directory, filename)
                        futures.append(executor.submit(self._process_file, input_filepath, output_filepath))

            for future in futures:
                future.result()

    def _process_file(self, input_filepath, output_filepath):
        try:
            df = pd.read_csv(input_filepath)
        except Exception as e:
            logging.error(f"Error reading {input_filepath}: {e}")
            return

        try:
            data = self._extract_data(df)
            self._plot_chart(data, output_filepath)
            logging.info(f"Processed and saved plot for {input_filepath}")
        except Exception as e:
            logging.error(f"Error processing {input_filepath}: {e}")

    def _file_name_without_extension(self, file_path):
        return os.path.splitext(os.path.basename(file_path))[0]

    def _extract_data(self, df):
        raise NotImplementedError("Subclasses must implement this method")

    def _plot_chart(self, data, output_filepath):
        raise NotImplementedError("Subclasses must implement this method")
