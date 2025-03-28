import argparse
import os
import subprocess
from concurrent.futures import ProcessPoolExecutor
from csv_processors.box_chart_processor import BoxChartProcessor
from csv_processors.single_chart_processor import SingleChartProcessor


# Use non-interactive backend for matplotlib
import matplotlib
matplotlib.use('agg')


def process_single_chart():
    single_chart_processor = SingleChartProcessor('../data/results/single/', '../data/charts/single/')
    single_chart_processor.process_csv_files()


def process_box_chart():
    box_chart_processor = BoxChartProcessor('../data/results/box/', '../data/charts/box/')
    box_chart_processor.process_csv_files()


def compile_cpp_program():
    """Compiles the program using CMake."""

    build_dir = r"C:\Users\User\CLionProjects\CVRP-optimization-metaheuristics\cmake-build-debug"

    cmake_path = r"C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin"

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    try:
        print("Running CMake configuration...")
        result = subprocess.run([cmake_path, ".."], check=True, text=True, capture_output=True, cwd=build_dir)
        print(result.stdout)

        print("Building the project...")
        result = subprocess.run([cmake_path, "--build", "."], check=True, text=True, capture_output=True, cwd=build_dir)
        print(result.stdout)

        print("Compilation completed successfully!")

    except subprocess.CalledProcessError as e:
        print("Error during compilation!")
        print("STDOUT:", e.stdout)
        print("STDERR:", e.stderr)
        exit(1)


def start_cpp_program(config_file, data_file):
    """Runs a C++ program with a config file"""
    cpp_program_path = '../cmake-build-debug/CVRP_optimization_metaheuristics.exe'
    mingw_dll_path = "C://Program Files//JetBrains//CLion 2023.3.4//bin//mingw//bin"
    env = os.environ.copy()
    env["PATH"] = mingw_dll_path + ";" + env["PATH"]

    try:
        result = subprocess.run([cpp_program_path, config_file, data_file],
                                env=env,
                                check=True,
                                text=True,
                                capture_output=True)

        print("Program output:", result.stdout)
        print("Program error (if any):", result.stderr)

    except subprocess.CalledProcessError as e:
        print(f"Error during execution: {e}")
        print("STDOUT:", e.stdout)
        print("STDERR:", e.stderr)


def main():
    parser = argparse.ArgumentParser(description='Process and plot CSV files.')
    parser.add_argument('--charts', type=str, default='single,box', help='Comma-separated list of charts to process (single, box)')
    parser.add_argument('config_file', type=str, help='Path to the C++ config file')
    parser.add_argument('data_file', type=str, help='Path to the data file')
    args = parser.parse_args()

    charts = args.charts.split(',')

    # compile_cpp_program()

    start_cpp_program(args.config_file, args.data_file)

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
