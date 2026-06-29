from __future__ import annotations

import json
import math
import re
from pathlib import Path
from typing import TYPE_CHECKING, Iterable, List
from src.models import ProblemInstance, Solution
if TYPE_CHECKING:
    from src.validator import ValidationSummary


def route_distance(route: List[int], distance_matrix: List[List[int]]) -> int:
    return sum(distance_matrix[route[i]][route[i + 1]] for i in range(len(route) - 1))


def route_load(route: List[int], demands: List[int], depot: int = 0) -> int:
    return sum(demands[node] for node in route if node != depot)


def format_route(route: Iterable[int]) -> str:
    return " -> ".join(map(str, route))


def print_route_details(instance: ProblemInstance, route_id: int, route_nodes: List[int]) -> None:
    cumulative_load = 0
    cumulative_distance = 0

    print("# ----------------------------------------------------------------------------")
    print(f"#\t- Route ID              : {route_id}")
    print(f"#\t- Route Size            : {len(route_nodes)}")
    print(f"#\t- Distance              : {route_distance(route_nodes, instance.distance_matrix)}")
    print(f"#\t- Load                  : {route_load(route_nodes, instance.demands, instance.depot)}")
    print("# ----------------------------------------------------------------------------")
    print("#     Node Type    Location ID  Node IDX   Node Load   Cum. Load   Cum. Dist")
    print("# ----------------------------------------------------------------------------")

    for stop_index, node in enumerate(route_nodes, start=1):
        if stop_index == 1:
            node_type = "(SOURCE)"
            node_idx = instance.depot
            node_load = 0
        elif stop_index == len(route_nodes):
            node_type = "( SINK )"
            node_idx = instance.total_nodes
            node_load = 0
        else:
            node_type = "(DEMAND)"
            node_idx = node
            node_load = instance.demands[node]
            cumulative_load += node_load

        if stop_index > 1:
            previous_node = route_nodes[stop_index - 2]
            cumulative_distance += instance.distance_matrix[previous_node][node]

        print(
            f"# {stop_index:>3}  {node_type:<8}  "
            f"{node:>11}  {node_idx:>8}  {node_load:>10}  "
            f"{cumulative_load:>10}  {cumulative_distance:>10}"
        )

    print("==============================================================================")
    print()


def print_solution_summary(
    instance: ProblemInstance,
    solution: Solution,
    validation: ValidationSummary,
    output_json_path: str | Path,
    runtime_seconds: float,
) -> None:
    print(f"Problem: {instance.problem_name}")
    print(f"Vehicles: {instance.vehicle_count}")
    print(f"Capacity per vehicle: {instance.capacity_per_vehicle}")
    print(f"Customers: {len(instance.customer_nodes)}")
    print(f"Method: {solution.method}")
    print(f"Feasible partitions evaluated: {solution.metadata['partitions_evaluated']}")
    print()

    print("============================= Final Routes ================================")
    for route in solution.routes:
        print_route_details(instance, route.vehicle_id, route.nodes)

    print(f"Total distance: {solution.total_distance}")
    print(f"Runtime: {runtime_seconds:.4f} seconds")
    print()
    print(f"All customers visited exactly once: {validation.all_customers_visited_once}")
    print(f"Routes start/end at depot: {validation.routes_start_end_at_depot}")
    print(f"Capacity constraints respected: {validation.capacities_respected}")
    print(f"Reported distances validated: {validation.reported_distances_match}")
    print(f"Vehicle count matches instance: {validation.vehicle_count_matches}")
    print(f"Overall solution valid: {validation.is_valid}")



def _compute_node_positions(
    distance_matrix: List[List[int]],
    depot: int = 0,
) -> dict[str, dict[str, float]]:
    node_count = len(distance_matrix)
    if node_count == 0:
        return {}

    positions: dict[str, dict[str, float]] = {str(depot): {"x": 0.0, "y": 0.0}}
    customer_nodes = [node for node in range(node_count) if node != depot]

    if not customer_nodes:
        return positions

    depot_distances = [distance_matrix[depot][node] for node in customer_nodes]
    min_distance = min(depot_distances)
    max_distance = max(depot_distances)
    distance_span = max(max_distance - min_distance, 1)

    for index, node in enumerate(customer_nodes):
        angle = (2 * math.pi * index) / len(customer_nodes)
        normalized_distance = (distance_matrix[depot][node] - min_distance) / distance_span
        radius = 0.65 + 0.45 * normalized_distance
        positions[str(node)] = {
            "x": radius * math.cos(angle),
            "y": radius * math.sin(angle),
        }

    return positions


def _format_output_json(payload: dict) -> str:
    formatted = json.dumps(payload, indent=2)

    def compact_nodes(match: re.Match[str]) -> str:
        numbers = re.findall(r"\d+", match.group(1))
        return f'"nodes": [{", ".join(numbers)}]'

    return re.sub(
        r'"nodes": \[\n(.*?)\n\s*\]',
        compact_nodes,
        formatted,
        flags=re.DOTALL,
    )


def save_solution_output(
    instance: ProblemInstance,
    solution: Solution,
    validation: ValidationSummary,
    output_path: str | Path = "output.json",
) -> Path:
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    payload = {
        "problem": {
            "name": instance.problem_name,
            "description": instance.problem_description,
            "total_nodes": instance.total_nodes,
            "depot": instance.depot,
            "vehicle_count": instance.vehicle_count,
            "capacity_per_vehicle": instance.capacity_per_vehicle,
            "constraints": instance.constraints,
        },
        "solution": {
            "method": solution.method,
            "total_distance": solution.total_distance,
            "metadata": solution.metadata,
            "routes": [
                {
                    "vehicle_id": route.vehicle_id,
                    "nodes": route.nodes,
                    "load": route.load,
                    "distance": route.distance,
                }
                for route in solution.routes
            ],
        },
        "validation": {
            "all_customers_visited_once": validation.all_customers_visited_once,
            "routes_start_end_at_depot": validation.routes_start_end_at_depot,
            "capacities_respected": validation.capacities_respected,
            "reported_distances_match": validation.reported_distances_match,
            "vehicle_count_matches": validation.vehicle_count_matches,
            "is_valid": validation.is_valid,
        },
    }

    with output_path.open("w", encoding="utf-8") as file:
        file.write(_format_output_json(payload))

    return output_path

