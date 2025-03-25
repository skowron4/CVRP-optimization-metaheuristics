import matplotlib.pyplot as plt
import pandas as pd
import os
import logging

logging.basicConfig(level=logging.INFO, format='%(levelname)s - %(message)s')


def extract_data(df):
    """
    Extracts data from the DataFrame.

    Parameters:
    df (DataFrame): The input DataFrame containing the data.

    Returns:
    tuple: A tuple containing iterations, best, worst, and avg series.
    """
    iterations = range(len(df))
    best = df['best']
    worst = df['worst']
    avg = df['avg']
    return iterations, best, worst, avg


def plot_data(iterations, best, worst, avg, output_filepath):
    """
    Plots the data and saves the plot to a file.

    Parameters:
    iterations (range): The range of iterations.
    best (Series): The best values.
    worst (Series): The worst values.
    avg (Series): The average values.
    output_filepath (str): The file path to save the plot.
    """
    best_overall = best.min()
    cumulative_best = best.cummin()

    plt.figure(figsize=(10, 6))
    plt.plot(iterations, best, label=f'Best (Overall: {best_overall})', color='blue')
    plt.plot(iterations, worst, label='Worst', color='red')
    plt.plot(iterations, avg, label='Average', color='green')
    plt.plot(iterations, cumulative_best, label='Best Currently Found', color='purple')

    plt.xlabel('Iterations')
    plt.ylabel('Fitness')
    plt.title('Fitness Statistics Over Iterations')
    plt.legend()
    plt.savefig(output_filepath)
    plt.close()


def process_csv_file(input_filepath, output_filepath):
    """
    Processes a CSV file to extract data and plot it.

    Parameters:
    input_filepath (str): The file path of the input CSV file.
    output_filepath (str): The file path to save the plot.
    """
    try:
        df = pd.read_csv(input_filepath)
        iterations, best, worst, avg = extract_data(df)
        plot_data(iterations, best, worst, avg, output_filepath)
        logging.info(f"Processed and saved plot for {input_filepath}")
    except Exception as e:
        logging.error(f"Failed to process {input_filepath}: {e}")


def traverse_directory(input_directory, output_directory):
    """
    Traverses the input directory to process all CSV files.

    Parameters:
    input_directory (str): The input directory containing CSV files.
    output_directory (str): The output directory to save the plots.
    """
    if not os.path.exists(input_directory):
        logging.error(f"Input directory '{input_directory}' does not exist.")
    else:
        os.makedirs(output_directory, exist_ok=True)

        for filename in os.listdir(input_directory):
            if filename.endswith('.csv'):
                input_filepath = os.path.join(input_directory, filename)
                output_filepath = os.path.join(output_directory, os.path.splitext(filename)[0] + '.png')

                if os.path.exists(output_filepath):
                    logging.info(f"Output file {output_filepath} already exists. Skipping.")
                    continue

                process_csv_file(input_filepath, output_filepath)


if __name__ == '__main__':
    input_directory = '../data/results/single/'
    output_directory = '../data/charts/single/'

    traverse_directory(input_directory, output_directory)
