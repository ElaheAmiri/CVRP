from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Dict, List, Tuple


@dataclass(frozen=True)
class ProblemInstance:
    """Container for the CVRP input data."""

    problem_name: str
    problem_description: str
    total_nodes: int
    depot: int
    delivery_locations: List[int]
    vehicle_count: int
    capacity_per_vehicle: int
    demands: List[int]
    distance_matrix: List[List[int]]
    constraints: Dict[str, Any] = field(default_factory=dict)

    @property
    def customer_nodes(self) -> List[int]:
        return list(self.delivery_locations)

    @property
    def total_demand(self) -> int:
        return sum(self.demands[node] for node in self.customer_nodes)

    @property
    def vehicle_capacity(self) -> int:
        return self.capacity_per_vehicle

    @property
    def num_customers(self) -> int:
        return len(self.delivery_locations)

@dataclass(frozen=True)
class Route:
    """Represents one vehicle route."""

    vehicle_id: int
    nodes: List[int]
    load: int
    distance: int


@dataclass(frozen=True)
class Solution:
    routes: List[Route]
    total_distance: int
    method: str
    metadata: Dict[str, Any] = field(default_factory=dict)
