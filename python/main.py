from __future__ import annotations

import argparse
from pathlib import Path
import time

from src.data_loader import load_problem_instance
from src.baseline_solver import BaselineExactSolver
from src.utils import print_solution_summary, save_solution_output
from src.validator import validate_solution



def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Baseline exact solver for the CVRP assessment instance."
    )
    parser.add_argument(
        "--input",
        type=Path,
        default=Path("data/cvrp_problem_data.json"),
        help="Path to the JSON problem instance.",
    )
    parser.add_argument(
        "--output-json",
        type=Path,
        default=Path("output.json"),
        help="Path to save the solution JSON output.",
    )
    parser.add_argument(
        "--plot-output",
        type=Path,
        default=Path("output_routes.svg"),
        help="Path to save the plotted routes image.",
    )
    return parser



def main() -> None:
    start_time = time.perf_counter()
    parser = build_parser()
    args = parser.parse_args()

    instance = load_problem_instance(args.input)
    solver = BaselineExactSolver(instance)
    solution = solver.solve()
    validation = validate_solution(instance, solution)
    output_json_path = save_solution_output(instance, solution, validation, args.output_json)
    print_solution_summary(
        instance,
        solution,
        validation,
        output_json_path,
        time.perf_counter() - start_time,
    )


if __name__ == "__main__":
    main()
