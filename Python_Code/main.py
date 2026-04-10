from __future__ import annotations

import argparse
from pathlib import Path

from src.data_loader import load_problem_instance



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
    return parser



def main() -> None:
    parser = build_parser()
    args = parser.parse_args()

    instance = load_problem_instance(args.input)

    print(f"Problem: {instance.problem_name}")
    print(f"Vehicles: {instance.vehicle_count}")
    print(f"Capacity per vehicle: {instance.capacity_per_vehicle}")
    print(f"Customers: {len(instance.customer_nodes)}")
    print()



if __name__ == "__main__":
    main()
