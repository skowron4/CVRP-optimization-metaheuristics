import argparse
from concurrent.futures import ProcessPoolExecutor
from single_chart_processor import SingleChartProcessor
from box_chart_processor import BoxChartProcessor

# Use non-interactive backend for matplotlib
import matplotlib
matplotlib.use('agg')


def process_single_chart():
    single_chart_processor = SingleChartProcessor('../data/results/single/', '../data/charts/single/')
    single_chart_processor.process_csv_files()


def process_box_chart():
    box_chart_processor = BoxChartProcessor('../data/results/box/', '../data/charts/box/')
    box_chart_processor.process_csv_files()


def main():
    parser = argparse.ArgumentParser(description='Process and plot CSV files.')
    parser.add_argument('--charts', type=str, default='single,box', help='Comma-separated list of charts to process (single, box)')
    args = parser.parse_args()

    charts = args.charts.split(',')
    with ProcessPoolExecutor() as executor:
        futures = []
        if 'single' in charts:
            futures.append(executor.submit(process_single_chart))
        if 'box' in charts:
            futures.append(executor.submit(process_box_chart))

        for future in futures:
            future.result()


if __name__ == '__main__':
    main()
