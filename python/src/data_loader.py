from __future__ import annotations

import json
from pathlib import Path

from src.models import ProblemInstance


class DataValidationError(ValueError):
    """Raised when the input data does not match the expected CVRP schema."""



def load_problem_instance(path: str | Path) -> ProblemInstance:
    path = Path(path)
    with path.open("r", encoding="utf-8") as file:
        raw = json.load(file)

    nodes = raw["nodes"]
    vehicles = raw["vehicles"]

    instance = ProblemInstance(
        problem_name=raw["problem_name"],
        problem_description=raw.get("problem_description", ""),
        total_nodes=nodes["total"],
        depot=nodes["depot"],
        delivery_locations=list(nodes["delivery_locations"]),
        vehicle_count=vehicles["count"],
        capacity_per_vehicle=vehicles["capacity_per_vehicle"],
        demands=list(raw["demands"]),
        distance_matrix=[list(row) for row in raw["distance_matrix"]],
        constraints=dict(raw.get("constraints", {})),
    )

    validate_problem_instance(instance)
    return instance



def validate_problem_instance(instance: ProblemInstance) -> None:
    n = instance.total_nodes

    if len(instance.demands) != n:
        raise DataValidationError(
            f"Expected {n} demands, found {len(instance.demands)}."
        )

    if len(instance.distance_matrix) != n:
        raise DataValidationError(
            f"Expected {n} rows in distance matrix, found {len(instance.distance_matrix)}."
        )

    for row_index, row in enumerate(instance.distance_matrix):
        if len(row) != n:
            raise DataValidationError(
                f"Row {row_index} of distance matrix has length {len(row)} instead of {n}."
            )

    if instance.depot < 0 or instance.depot >= n:
        raise DataValidationError(f"Depot index {instance.depot} is out of range.")

    expected_customers = [node for node in range(n) if node != instance.depot]
    if sorted(instance.customer_nodes) != expected_customers:
        raise DataValidationError(
            "Delivery locations do not match the non-depot nodes in the instance."
        )

    if instance.vehicle_count <= 0:
        raise DataValidationError("Vehicle count must be positive.")

    if instance.capacity_per_vehicle <= 0:
        raise DataValidationError("Vehicle capacity must be positive.")

    if any(demand < 0 for demand in instance.demands):
        raise DataValidationError("Demands must be non-negative.")

    if any(distance < 0 for row in instance.distance_matrix for distance in row):
        raise DataValidationError("Distances must be non-negative.")
