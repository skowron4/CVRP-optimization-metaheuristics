import argparse
import os
import subprocess
from concurrent.futures import ProcessPoolExecutor
from csv_processors.box_chart_processor import BoxChartProcessor
from csv_processors.single_chart_processor import SingleChartProcessor

# Use non-interactive backend for matplotlib
import matplotlib
matplotlib.use('agg')


def process_single_chart(location):
    src = os.path.join(location, 'results', 'single')
    tgt = os.path.join(location, 'results', 'charts', 'single')
    SingleChartProcessor(src, tgt).process_csv_files()


def process_box_chart(location):
    src = os.path.join(location, 'results', 'box')
    tgt = os.path.join(location, 'results', 'charts', 'box')
    BoxChartProcessor(src, tgt).process_csv_files()


def start_cpp_program(exe_path, config_file, data_file, dest_dir):
    cmd = [exe_path, config_file, data_file, dest_dir]
    try:
        result = subprocess.run(cmd,
                                check=True,
                                text=True,
                                capture_output=True)
        print("C++ stdout:\n", result.stdout)
        print("C++ stderr:\n", result.stderr)
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] C++ exited {e.returncode}")
        print("Stdout:", e.stdout)
        print("Stderr:", e.stderr)
        raise


def main():
    parser = argparse.ArgumentParser(description='(Re)run C++ and/or generate charts from CSV results.')
    parser.add_argument('--exe',
                        type=str,
                        required=False,
                        help='Path to C++ binary in case of additionally running it in advance. Defaults to None.')
    parser.add_argument('--charts',
                        type=str,
                        default='single,box',
                        help='Comma-separated charts to generate (single,box). Defaults to all.')
    parser.add_argument('--location', "--loc",
                        type=str,
                        default='./',
                        help="Location of 'results' folder with at least one subfolder: \
                              'results/single' or 'results/box', created previously by the C++ program. \
                              Charts will be saved in 'charts/single' or 'charts/box' subfolders. \
                              Defaults to the current working directory ('./').")
    parser.add_argument('config',
                        type=str,
                        help='Path to the config file for the C++ program')
    parser.add_argument('data',
                        type=str,
                        help='Path to the data file for the C++ program')
    args = parser.parse_args()

    location = os.path.abspath(args.location)

    if args.exe:
        print(f"Running C++ program with config: {args.config} and data: {args.data}")
        print(f"Results will be saved in: {location}")
        start_cpp_program(args.exe, args.config, args.data, location)

    charts = {chart.strip().lower() for chart in args.charts.split(',')}
    with ProcessPoolExecutor() as executor:
        if 'single' in charts:
            executor.submit(process_single_chart, location)
        if 'box' in charts:
            executor.submit(process_box_chart, location)

if __name__ == '__main__':
    main()
