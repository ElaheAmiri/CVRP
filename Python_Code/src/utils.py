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

