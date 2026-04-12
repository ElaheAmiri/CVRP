from __future__ import annotations

import argparse
from pathlib import Path
import time

from src.data_loader import load_problem_instance
from src.baseline_solver import BaselineExactSolver
from src.utils import format_route, save_solution_output
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

    print(f"Problem: {instance.problem_name}")
    print(f"Vehicles: {instance.vehicle_count}")
    print(f"Capacity per vehicle: {instance.capacity_per_vehicle}")
    print(f"Customers: {len(instance.customer_nodes)}")
    print(f"Method: {solution.method}")
    print(f"Feasible partitions evaluated: {solution.metadata['partitions_evaluated']}")
    print()

    for route in solution.routes:
        print(f"Truck {route.vehicle_id} route: {format_route(route.nodes)}")
        print(f"Truck {route.vehicle_id} load: {route.load} / {instance.capacity_per_vehicle}")
        print(f"Truck {route.vehicle_id} distance: {route.distance}")
        print()

    print(f"Total distance: {solution.total_distance}")
    print(f"All customers visited exactly once: {validation.all_customers_visited_once}")
    print(f"Routes start/end at depot: {validation.routes_start_end_at_depot}")
    print(f"Capacity constraints respected: {validation.capacities_respected}")
    print(f"Reported distances validated: {validation.reported_distances_match}")
    print(f"Vehicle count matches instance: {validation.vehicle_count_matches}")
    print(f"Overall solution valid: {validation.is_valid}")
    print(f"Saved solution JSON: {output_json_path}")
    print(f"Runtime: {time.perf_counter() - start_time:.4f} seconds")


if __name__ == "__main__":
    main()
